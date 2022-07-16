#include "MaterialBufferArray.h"

using DirectX::XMFLOAT4;
using DirectX::XMMATRIX;

MaterialBufferArray::MaterialBufferArray() noexcept :
	ConstantBufferArray(ConstantBufferBindingLocation::PIXEL_SHADER)
{
	PROFILE_FUNCTION();

	BindFunc = [this]() { this->BindMaterialBuffer(); };

	m_materials = std::make_unique<PhongMaterialProperties>();

	// Hyrdogen
	m_materials->Materials[0].Emissive = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	m_materials->Materials[0].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_materials->Materials[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_materials->Materials[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[0].SpecularPower = 6.0f;
	m_materials->Materials[0].UseTexture = 0;

	// Helium
	m_materials->Materials[1].Emissive = XMFLOAT4(0.4f, 0.14f, 0.14f, 1.0f);
	m_materials->Materials[1].Ambient = XMFLOAT4(1.0f, 0.75f, 0.75f, 1.0f);
	m_materials->Materials[1].Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	m_materials->Materials[1].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[1].SpecularPower = 6.0f;

	// Lithium
	m_materials->Materials[2].Emissive = XMFLOAT4(0.15f, 0.0f, 0.15f, 1.0f);
	m_materials->Materials[2].Ambient = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	m_materials->Materials[2].Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	m_materials->Materials[2].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[2].SpecularPower = 6.0f;

	// Beryllium
	m_materials->Materials[3].Emissive = XMFLOAT4(0.15f, 0.15f, 0.0f, 1.0f);
	m_materials->Materials[3].Ambient = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_materials->Materials[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_materials->Materials[3].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[3].SpecularPower = 6.0f;

	// Boron
	m_materials->Materials[4].Emissive = XMFLOAT4(0.45f, 0.22f, 0.22f, 1.0f);
	m_materials->Materials[4].Ambient = XMFLOAT4(1.0f, 0.45f, 0.45f, 1.0f);
	m_materials->Materials[4].Diffuse = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);
	m_materials->Materials[4].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[4].SpecularPower = 6.0f;

	// Carbon
	m_materials->Materials[5].Emissive = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_materials->Materials[5].Ambient = XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f);
	m_materials->Materials[5].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_materials->Materials[5].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[5].SpecularPower = 6.0f;

	// Nitrogen
	m_materials->Materials[6].Emissive = XMFLOAT4(0.0f, 0.0f, 0.3f, 1.0f);
	m_materials->Materials[6].Ambient = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_materials->Materials[6].Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_materials->Materials[6].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[6].SpecularPower = 6.0f;

	// Oxygen
	m_materials->Materials[7].Emissive = XMFLOAT4(0.3f, 0.0f, 0.0f, 1.0f);
	m_materials->Materials[7].Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_materials->Materials[7].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_materials->Materials[7].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[7].SpecularPower = 6.0f;

	// Flourine
	m_materials->Materials[8].Emissive = XMFLOAT4(0.0f, 0.12f, 0.12f, 1.0f);
	m_materials->Materials[8].Ambient = XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[8].Diffuse = XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f);
	m_materials->Materials[8].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[8].SpecularPower = 6.0f;

	// Neon
	m_materials->Materials[9].Emissive = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	m_materials->Materials[9].Ambient = XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
	m_materials->Materials[9].Diffuse = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	m_materials->Materials[9].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials->Materials[9].SpecularPower = 6.0f;

	std::shared_ptr<ConstantBuffer> materialsBuffer = std::make_shared<ConstantBuffer>();
	materialsBuffer->CreateBuffer<PhongMaterialProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(m_materials.get()));

	AddBuffer(materialsBuffer);
}

void MaterialBufferArray::BindMaterialBuffer() const noexcept
{
	PROFILE_FUNCTION();

	// Call PSSetConstantBuffers to bind the buffer to slot 2
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetConstantBuffers(2u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void MaterialBufferArray::UpdateMaterialProperties() const noexcept
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->UpdateSubresource(
			m_buffers[0]->GetRawBufferPointer(),
			0, nullptr,
			static_cast<void*>(m_materials.get()),
			0, 0)
	);
}