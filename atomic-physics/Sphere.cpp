#include "Sphere.h"

using DirectX::XMMATRIX;
using DirectX::XMVECTORF32;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;

Sphere::Sphere(std::shared_ptr<MoveLookController> mlc) noexcept :
	Drawable(mlc),
	m_radius(1.0f),
	m_materialIndexBufferArray(nullptr)
{
	/*
	PhongMaterialProperties materialProps;

	std::shared_ptr<ConstantBuffer> materialBuffer = std::make_shared<ConstantBuffer>();
	materialBuffer->CreateBuffer<PhongMaterialProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&materialProps));

	std::unique_ptr<ConstantBufferArray> psBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	psBufferArray->AddBuffer(materialBuffer);
	m_bindables.push_back(std::move(psBufferArray));
	*/





}

void Sphere::SetAtomType(int elementNumber) noexcept
{
	// Radius
	m_radius = Constants::AtomicRadii[elementNumber];

	// Material
	/*
	PhongMaterialProperties materials[11];

	materials[1].Material.Emissive = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	materials[1].Material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materials[1].Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materials[1].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[1].Material.SpecularPower = 6.0f;

	materials[2].Material.Emissive = XMFLOAT4(0.4f, 0.14f, 0.14f, 1.0f);
	materials[2].Material.Ambient = XMFLOAT4(1.0f, 0.75f, 0.75f, 1.0f);
	materials[2].Material.Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	materials[2].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[2].Material.SpecularPower = 6.0f;

	materials[3].Material.Emissive = XMFLOAT4(0.15f, 0.0f, 0.15f, 1.0f);
	materials[3].Material.Ambient = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	materials[3].Material.Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	materials[3].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[3].Material.SpecularPower = 6.0f;

	materials[4].Material.Emissive = XMFLOAT4(0.15f, 0.15f, 0.0f, 1.0f);
	materials[4].Material.Ambient = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	materials[4].Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	materials[4].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[4].Material.SpecularPower = 6.0f;

	materials[5].Material.Emissive = XMFLOAT4(0.45f, 0.22f, 0.22f, 1.0f);
	materials[5].Material.Ambient = XMFLOAT4(1.0f, 0.45f, 0.45f, 1.0f);
	materials[5].Material.Diffuse = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);
	materials[5].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[5].Material.SpecularPower = 6.0f;

	materials[6].Material.Emissive = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	materials[6].Material.Ambient = XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f);
	materials[6].Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	materials[6].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[6].Material.SpecularPower = 6.0f;

	materials[7].Material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.3f, 1.0f);
	materials[7].Material.Ambient = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	materials[7].Material.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	materials[7].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[7].Material.SpecularPower = 6.0f;

	materials[8].Material.Emissive = XMFLOAT4(0.3f, 0.0f, 0.0f, 1.0f);
	materials[8].Material.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	materials[8].Material.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	materials[8].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[8].Material.SpecularPower = 6.0f;

	materials[9].Material.Emissive = XMFLOAT4(0.0f, 0.12f, 0.12f, 1.0f);
	materials[9].Material.Ambient = XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f);
	materials[9].Material.Diffuse = XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f);
	materials[9].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[9].Material.SpecularPower = 6.0f;

	materials[10].Material.Emissive = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	materials[10].Material.Ambient = XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
	materials[10].Material.Diffuse = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	materials[10].Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materials[10].Material.SpecularPower = 6.0f;

	std::shared_ptr<ConstantBuffer> materialBuffer = std::make_shared<ConstantBuffer>();
	materialBuffer->CreateBuffer<PhongMaterialProperties>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&materials[elementNumber]));

	psMaterialBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	psMaterialBufferArray->AddBuffer(materialBuffer);
	*/

	PhongMaterialIndexBuffer materialIndex = {};
	materialIndex.materialIndex = elementNumber - 1; // Have to subtract 1 because materials array is zero indexed and we start with Hydrogen as element 1
	
	std::shared_ptr<ConstantBuffer> materialIndexBuffer = std::make_shared<ConstantBuffer>();
	materialIndexBuffer->CreateBuffer<PhongMaterialIndexBuffer>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&materialIndex));

	m_materialIndexBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	m_materialIndexBufferArray->AddBuffer(materialIndexBuffer);
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

	// Good idea to assert that the material index buffer array is not null, because it is only created in SetAtomType()
	assert(m_materialIndexBufferArray != nullptr);
	m_materialIndexBufferArray->Bind();


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

	GFX_THROW_INFO_ONLY(
		context->Unmap(vsBuffer.Get(), 0)
	);
}