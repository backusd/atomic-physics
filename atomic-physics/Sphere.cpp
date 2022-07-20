#include "Sphere.h"

std::unique_ptr<InputLayout>			Sphere::m_inputLayout = nullptr;
std::unique_ptr<VertexShader>			Sphere::m_vertexShader = nullptr;
std::unique_ptr<PixelShader>			Sphere::m_pixelShader = nullptr;
std::unique_ptr<SphereMesh>				Sphere::m_mesh = nullptr;
std::unique_ptr<ConstantBufferArray>	Sphere::m_vsBuffers = nullptr;
std::unique_ptr<RasterizerState>		Sphere::m_rasterizerState = nullptr;
std::unique_ptr<DepthStencilState>		Sphere::m_depthStencilState = nullptr;

using DirectX::XMMATRIX;
using DirectX::XMVECTORF32;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;

Sphere::Sphere(std::shared_ptr<MoveLookController> mlc) noexcept :
	Drawable(mlc),
	m_elementNumber(0),
	m_radius(1.0f),
	m_materialIndexBufferArray(nullptr)
{
	PROFILE_FUNCTION();

	// Initialize static variables (can NOT do this in the global static definition because we do not yet have access to Device Resources)
	if (!m_inputLayout)			m_inputLayout = std::make_unique<InputLayout>(L"PhongVS.cso", BasicGeometry::SPHERE);
	if (!m_vertexShader)		m_vertexShader = std::make_unique<VertexShader>(m_inputLayout->GetVertexShaderFileBlob());
	if (!m_pixelShader)			m_pixelShader = std::make_unique<PixelShader>(L"PhongPS.cso");
	if (!m_mesh)				m_mesh = std::make_unique<SphereMesh>();
	if (!m_vsBuffers)			m_vsBuffers = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER, BasicGeometry::SPHERE);
	if (!m_rasterizerState)		m_rasterizerState = std::make_unique<RasterizerState>();
	if (!m_depthStencilState)	m_depthStencilState = std::make_unique<DepthStencilState>(1);
}

void Sphere::SetAtomType(int elementNumber) noexcept
{
	PROFILE_FUNCTION();

	m_elementNumber = elementNumber;

	// Radius
	m_radius = Constants::AtomicRadii[elementNumber];

	// Material index into the material constant buffer array in the Pixel Shader
	PhongMaterialIndexBuffer materialIndex = {};
	materialIndex.materialIndex = elementNumber - 1; // Have to subtract 1 because materials array is zero indexed and we start with Hydrogen as element 1
	
	std::shared_ptr<ConstantBuffer> materialIndexBuffer = std::make_shared<ConstantBuffer>();
	materialIndexBuffer->CreateBuffer<PhongMaterialIndexBuffer>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&materialIndex));

	m_materialIndexBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	m_materialIndexBufferArray->AddBuffer(materialIndexBuffer);
}

void Sphere::Update() const noexcept
{
	// PROFILE_FUNCTION();
}

void Sphere::Draw() const noexcept
{
	PROFILE_FUNCTION();

	// Bind all static variables
	m_inputLayout->Bind();
	m_vertexShader->Bind();
	m_pixelShader->Bind();
	m_mesh->Bind();
	m_vsBuffers->Bind();
	m_rasterizerState->Bind();
	m_depthStencilState->Bind();

	// Good idea to assert that the material index buffer array is not null, because it is only created in SetAtomType()
	assert(m_materialIndexBufferArray != nullptr);
	m_materialIndexBufferArray->Bind();

	// In order to update the model-view-projection constant buffer, it must first be bound to the pipeline in
	// order to Map to it. Therefore, this kind of update must occur here as opposed to in the actual Update() function
	UpdateModelViewProjectionBuffer();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DrawIndexed(m_mesh->IndexCount(), 0u, 0u)
	);
}

void Sphere::UpdateModelViewProjectionBuffer() const
{
	PROFILE_FUNCTION();

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