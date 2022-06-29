#include "Sphere.h"

using DirectX::XMMATRIX;
using DirectX::XMVECTORF32;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;

Sphere::Sphere(std::shared_ptr<MoveLookController> mlc) noexcept :
	Drawable(mlc),
	m_radius(1.0f)
{

	/*
	LightProperties lightProps = LightProperties();
	lightProps.GlobalAmbient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirectX::XMStoreFloat4(&lightProps.EyePosition, m_moveLookController->Position());

	static const XMVECTORF32 LightColors[MAX_LIGHTS] = {
		DirectX::Colors::White,
		DirectX::Colors::Orange,
		DirectX::Colors::Yellow,
		DirectX::Colors::Green,
		DirectX::Colors::Blue,
		DirectX::Colors::Indigo,
		DirectX::Colors::Violet,
		DirectX::Colors::White
	};

	static const LightType LightTypes[MAX_LIGHTS] = {
		PointLight, SpotLight, SpotLight, PointLight, SpotLight, SpotLight, SpotLight, PointLight
	};

	static const bool LightEnabled[MAX_LIGHTS] = {
		true, false, false, false, false, false, false, false
	};

	const int numLights = MAX_LIGHTS;
	for (int i = 0; i < numLights; ++i)
	{
		Light light;
		light.Enabled = static_cast<int>(LightEnabled[i]);
		light.LightType = LightTypes[i];
		light.Color = XMFLOAT4(LightColors[i]);
		light.SpotAngle = DirectX::XMConvertToRadians(45.0f);
		light.ConstantAttenuation = 1.0f;
		light.LinearAttenuation = 0.08f;
		light.QuadraticAttenuation = 0.0f;

		XMFLOAT4 LightPosition = XMFLOAT4(0.0f, 10.0f, 10.0f, 1.0f);
		light.Position = LightPosition;
		XMVECTOR LightDirection = DirectX::XMVectorSet(-LightPosition.x, -LightPosition.y, -LightPosition.z, 0.0f);
		XMStoreFloat4(&light.Direction, DirectX::XMVector3Normalize(LightDirection));

		lightProps.Lights[i] = light;
	}

	std::shared_ptr<ConstantBuffer> lightingBuffer = std::make_shared<ConstantBuffer>();
	lightingBuffer->CreateBuffer<LightProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&lightProps));
	*/
	// ---

	PhongMaterialProperties materialProps;

	std::shared_ptr<ConstantBuffer> materialBuffer = std::make_shared<ConstantBuffer>();
	materialBuffer->CreateBuffer<PhongMaterialProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&materialProps));

	std::unique_ptr<ConstantBufferArray> psBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	// psBufferArray->AddBuffer(lightingBuffer);
	psBufferArray->AddBuffer(materialBuffer);
	m_bindables.push_back(std::move(psBufferArray));






}

void Sphere::Update() const noexcept
{

}

void Sphere::Draw() const noexcept_release_only
{
	// Hold static variables that will always be used to render spheres
	static std::unique_ptr<InputLayout>			inputLayout			= std::make_unique<InputLayout>(L"PhongVS.cso", BasicGeometry::SPHERE);
	static std::unique_ptr<VertexShader>		vertexShader		= std::make_unique<VertexShader>(inputLayout->GetVertexShaderFileBlob());
	static std::unique_ptr<PixelShader>			pixelShader			= std::make_unique<PixelShader>(L"PhongPS.cso");
	static std::unique_ptr<SphereMesh>			mesh				= std::make_unique<SphereMesh>();
	static std::unique_ptr<ConstantBufferArray> vsBuffers			= std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER, BasicGeometry::SPHERE );
	static std::unique_ptr<RasterizerState>		rasterizerState		= std::make_unique<RasterizerState>();
	static std::unique_ptr<DepthStencilState>	depthStencilState	= std::make_unique<DepthStencilState>(1);

	inputLayout->Bind();
	vertexShader->Bind();
	pixelShader->Bind();
	mesh->Bind();
	vsBuffers->Bind();
	rasterizerState->Bind();
	depthStencilState->Bind();

	for (const std::unique_ptr<Bindable>& bindable : m_bindables)
		bindable->Bind();

	// In order to update the model-view-projection constant buffer, it must first be bound to the pipeline in
	// order to Map to it. Therefore, this kind of update must occur here as opposed to in the actual Update() function
	UpdateModelViewProjectionBuffer();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DrawIndexed(mesh->IndexCount(), 0u, 0u)
	);
}

void Sphere::UpdateModelViewProjectionBuffer() const
{
	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Update VS constant buffer with model/view/projection info
	Microsoft::WRL::ComPtr<ID3D11Buffer> vsBuffer;
	GFX_THROW_INFO_ONLY(
		context->VSGetConstantBuffers(0, 1, vsBuffer.ReleaseAndGetAddressOf())
	);

	GFX_THROW_INFO(
		context->Map(vsBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)
	);

	ModelViewProjectionPreMultiplied* mappedBuffer = (ModelViewProjectionPreMultiplied*)ms.pData;

	XMMATRIX viewProjection = m_moveLookController->ViewMatrix() * m_moveLookController->ProjectionMatrix();
	XMMATRIX model = GetRotationMatrix() * GetScaleMatrix() * GetTranslationMatrix();

	DirectX::XMStoreFloat4x4(&(mappedBuffer->model), model);
	DirectX::XMStoreFloat4x4(&(mappedBuffer->modelViewProjection), model * viewProjection);
	DirectX::XMStoreFloat4x4(&(mappedBuffer->inverseTransposeModel), DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, model)));


	/*
	ModelViewProjection* mappedBuffer = (ModelViewProjection*)ms.pData;
	DirectX::XMStoreFloat4x4(&(mappedBuffer->model), DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->view), m_moveLookController->ViewMatrix());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->projection), m_moveLookController->ProjectionMatrix());
	*/

	GFX_THROW_INFO_ONLY(
		context->Unmap(vsBuffer.Get(), 0)
	);
}