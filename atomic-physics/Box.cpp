#include "Box.h"

std::unique_ptr<InputLayout>			Box::m_inputLayout = nullptr;
std::unique_ptr<VertexShader>			Box::m_vertexShader = nullptr;
std::unique_ptr<PixelShader>			Box::m_pixelShader = nullptr;
std::unique_ptr<BoxMesh>				Box::m_mesh = nullptr;
std::unique_ptr<ConstantBufferArray>	Box::m_vsBuffers = nullptr;
std::unique_ptr<RasterizerState>		Box::m_rasterizerState = nullptr;
std::unique_ptr<DepthStencilState>		Box::m_depthStencilState = nullptr;

using DirectX::XMMATRIX;

Box::Box(std::shared_ptr<MoveLookController> mlc) noexcept :
	Drawable(mlc),
	m_xLength(2.0f),
	m_yLength(2.0f),
	m_zLength(2.0f)
{
	PROFILE_FUNCTION();

	// Initialize static variables (can NOT do this in the global static definition because we do not yet have access to Device Resources)
	if (!m_inputLayout)			m_inputLayout = std::make_unique<InputLayout>(L"SolidVS.cso", BasicGeometry::BOX);
	if (!m_vertexShader)		m_vertexShader = std::make_unique<VertexShader>(m_inputLayout->GetVertexShaderFileBlob());
	if (!m_pixelShader)			m_pixelShader = std::make_unique<PixelShader>(L"SolidPS.cso");
	if (!m_mesh)				m_mesh = std::make_unique<BoxMesh>();
	if (!m_vsBuffers)			m_vsBuffers = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER, BasicGeometry::BOX);
	if (!m_rasterizerState)		m_rasterizerState = std::make_unique<RasterizerState>();
	if (!m_depthStencilState)	m_depthStencilState = std::make_unique<DepthStencilState>(1);
}

void Box::SetBoxSize(float x, float y, float z) noexcept
{
	m_xLength = x;
	m_yLength = y;
	m_zLength = z;
}

void Box::SetBoxSize(DirectX::XMFLOAT3 dimensions) noexcept
{
	m_xLength = dimensions.x;
	m_yLength = dimensions.y;
	m_zLength = dimensions.z;
}

void Box::Draw() const noexcept
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

	// In order to update the model-view-projection constant buffer, it must first be bound to the pipeline in
	// order to Map to it. Therefore, this kind of update must occur here as opposed to an Update() function
	UpdateModelViewProjectionBuffer();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DrawIndexed(m_mesh->IndexCount(), 0u, 0u)
	);
}

void Box::UpdateModelViewProjectionBuffer() const
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

	ModelViewProjection* mappedBuffer = (ModelViewProjection*)ms.pData;

	DirectX::XMStoreFloat4x4(&(mappedBuffer->model),      GetRotationMatrix() * GetScaleMatrix() * GetTranslationMatrix());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->view),       m_moveLookController->ViewMatrix());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->projection), m_moveLookController->ProjectionMatrix());

	GFX_THROW_INFO_ONLY(
		context->Unmap(vsBuffer.Get(), 0)
	);
}