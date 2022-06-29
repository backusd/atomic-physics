#include "Drawable.h"

Drawable::Drawable(std::shared_ptr<MoveLookController> mlc) noexcept :
	m_moveLookController(mlc),
	m_mesh(nullptr),
	m_position{},
	m_velocity{},
	m_roll(0.0f),
	m_pitch(0.0f),
	m_yaw(0.0f)
{
	//std::shared_ptr<ConstantBuffer> cb = std::make_shared<ConstantBuffer>();
	//cb->CreateBuffer<ModelViewProjectionConstantBuffer>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
    //
	//std::unique_ptr<ConstantBufferArray> cba = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	//cba->AddBuffer(cb);
	//m_bindables.push_back(std::move(cba));



	/*
	ColorConstantBuffer ccb = { DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	std::shared_ptr<ConstantBuffer> cb = std::make_shared<ConstantBuffer>();
	cb->CreateBuffer<ColorConstantBuffer>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&ccb));

	std::unique_ptr<ConstantBufferArray> cba = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	cba->AddBuffer(cb);
	m_bindables.push_back(std::move(cba));



	
	std::unique_ptr<DepthStencilState> dss = std::make_unique<DepthStencilState>(1);
	m_bindables.push_back(std::move(dss));

	std::unique_ptr<RasterizerState> rs = std::make_unique<RasterizerState>();
	m_bindables.push_back(std::move(rs));

	std::unique_ptr<InputLayout> il = std::make_unique<InputLayout>(L"SolidVS.cso");
	il->AddDescription("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	il->AddDescription(  "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	il->CreateLayout();
	std::unique_ptr<VertexShader> vs = std::make_unique<VertexShader>(il->GetVertexShaderFileBlob());
	m_bindables.push_back(std::move(vs));
	m_bindables.push_back(std::move(il));

	std::unique_ptr<PixelShader> ps = std::make_unique<PixelShader>(L"SolidPS.cso");
	m_bindables.push_back(std::move(ps));

	std::unique_ptr<SphereMesh> sphere = std::make_unique<SphereMesh>();
	m_bindables.push_back(std::move(sphere));
	*/
}

void Drawable::Position(float x, float y, float z) noexcept
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Drawable::Velocity(float x, float y, float z) noexcept
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}