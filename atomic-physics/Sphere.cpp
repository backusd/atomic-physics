#include "Sphere.h"

using DirectX::XMMATRIX;

Sphere::Sphere(std::shared_ptr<MoveLookController> mlc) noexcept :
	Drawable(mlc)
{
	// IA - Input Assembler --------------------------------------------------------------
	std::unique_ptr<InputLayout> inputLayout = std::make_unique<InputLayout>(L"SolidVS.cso");
	inputLayout->AddDescription("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	inputLayout->AddDescription(  "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	inputLayout->CreateLayout();

	// The mesh is not included in the list of bindables because we need to be able to easily access
	// it to call mesh->IndexCount() below
	m_mesh = std::make_unique<SphereMesh>();

	// VS - Vertex Shader ------------------------------------------------------------------
	m_bindables.push_back(std::make_unique<VertexShader>(inputLayout->GetVertexShaderFileBlob()));
	m_bindables.push_back(std::move(inputLayout));

	std::shared_ptr<ConstantBuffer> mvpBuffer = std::make_shared<ConstantBuffer>();
	mvpBuffer->CreateBuffer<ModelViewProjection>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);

	ColorConstantBuffer color = { DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	std::shared_ptr<ConstantBuffer> colorBuffer = std::make_shared<ConstantBuffer>();
	colorBuffer->CreateBuffer<ColorConstantBuffer>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&color));

	std::unique_ptr<ConstantBufferArray> vsBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	vsBufferArray->AddBuffer(mvpBuffer);
	vsBufferArray->AddBuffer(colorBuffer);
	m_bindables.push_back(std::move(vsBufferArray));


	// PS - Pixel Shader --------------------------------------------------------------------
	m_bindables.push_back(std::make_unique<PixelShader>(L"SolidPS.cso"));


	// RS - Rasterizer State ----------------------------------------------------------------
	std::unique_ptr<RasterizerState> rs = std::make_unique<RasterizerState>();
	m_bindables.push_back(std::move(rs));


	// DS - Depth Stencil State -------------------------------------------------------------
	std::unique_ptr<DepthStencilState> dss = std::make_unique<DepthStencilState>(1);
	m_bindables.push_back(std::move(dss));
}

void Sphere::Update() const noexcept
{

}

void Sphere::Draw() const noexcept_release_only
{
	for (const std::unique_ptr<Bindable>& bindable : m_bindables)
		bindable->Bind();

	// The mesh is not included in the list of bindables because we need to be able to easily access
	// it to call mesh->IndexCount() below
	m_mesh->Bind();

	// In order to update the model-view-projection constant buffer, it must first be bound to the pipeline in
	// order to Map to it. Therefore, this kind of update must occur here as opposed to in the actual Update() function
	UpdateModelViewProjectionBuffer();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DrawIndexed(m_mesh->IndexCount(), 0u, 0u)
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

	ModelViewProjection* mappedBuffer = (ModelViewProjection*)ms.pData;

	DirectX::XMStoreFloat4x4(&(mappedBuffer->model), DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->view), m_moveLookController->ViewMatrix());
	DirectX::XMStoreFloat4x4(&(mappedBuffer->projection), m_moveLookController->ProjectionMatrix());

	GFX_THROW_INFO_ONLY(
		context->Unmap(vsBuffer.Get(), 0)
	);
}