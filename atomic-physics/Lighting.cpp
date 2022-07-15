#include "Lighting.h"
#include "MacroHelper.h"

using DirectX::XMFLOAT4;
using DirectX::XMVECTORF32;
using DirectX::XMVECTOR;


Lighting::Lighting() noexcept :
	ConstantBufferArray(ConstantBufferBindingLocation::PIXEL_SHADER)
{
	PROFILE_FUNCTION();

	BindFunc = [this]() { this->BindLightingBuffer(); };

	CreateLightProperties();
}

void Lighting::CreateLightProperties() noexcept
{
	PROFILE_FUNCTION();

	m_properties = LightProperties();
	m_properties.GlobalAmbient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	const int numLights = MAX_LIGHTS;
	for (int i = 0; i < numLights; ++i)
	{
		Light light;
		light.Enabled = static_cast<int>(false);				// default: light off
		light.LightType = PointLight;							// default: PointLight
		light.Color = XMFLOAT4(DirectX::Colors::White);			// default: White
		light.SpotAngle = DirectX::XMConvertToRadians(45.0f);	// default: 45 degree angle
		light.ConstantAttenuation = 1.0f;
		light.LinearAttenuation = 0.08f;
		light.QuadraticAttenuation = 0.0f;

		light.Position = XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f); // default: (0, 0, 10)
		light.Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f); // default: (0, 0, -1) - should be normalized I believe and we want it pointed at the origin

		m_properties.Lights[i] = light;
	}

	std::shared_ptr<ConstantBuffer> lightingBuffer = std::make_shared<ConstantBuffer>();
	lightingBuffer->CreateBuffer<LightProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&m_properties));

	AddBuffer(lightingBuffer);
}

void Lighting::BindLightingBuffer() const noexcept_release_only
{
	PROFILE_FUNCTION();

	// Call PSSetConstantBuffers to bind the buffer to slot 0
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void Lighting::EditLight(int index, DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 direction, 
						bool enabled, int lightType, DirectX::XMFLOAT4 color, float spotAngle, 
						float constantAttenuation, float linearAttenuation, float quadraticAttenuation) noexcept_release_only
{
	PROFILE_FUNCTION();

	m_properties.Lights[index].Enabled = enabled;
	m_properties.Lights[index].LightType = lightType;
	m_properties.Lights[index].Color = color;
	m_properties.Lights[index].SpotAngle = spotAngle;
	m_properties.Lights[index].ConstantAttenuation = constantAttenuation;
	m_properties.Lights[index].LinearAttenuation = linearAttenuation;
	m_properties.Lights[index].QuadraticAttenuation = quadraticAttenuation;
	m_properties.Lights[index].Position = position;

	// Good idea to normalize the direction
	DirectX::XMStoreFloat4(&m_properties.Lights[index].Direction, DirectX::XMVector3Normalize(DirectX::XMLoadFloat4(&direction)));

	UpdateLightingProperties();
}

void Lighting::UpdateLightingProperties() noexcept_release_only
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->UpdateSubresource(
			m_buffers[0]->GetRawBufferPointer(),
			0, nullptr,
			static_cast<void*>(&m_properties),
			0, 0)
	);
}