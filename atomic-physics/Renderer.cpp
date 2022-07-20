#include "Renderer.h"

using DirectX::XMFLOAT4;
using DirectX::XMMATRIX;

Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp),
	m_drawLights(false)
{
	PROFILE_FUNCTION();

	m_moveLookController = std::make_shared<MoveLookController>(m_viewport);

	m_box = std::make_unique<Box>(m_moveLookController);
	m_box->SetBoxSize(SimulationManager::GetSimulationDimensions());

	// lighting
	m_lighting = std::make_unique<Lighting>();
	m_lighting->GlobalAmbient(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	m_lighting->EditLight(0, { -5.0f, 0.0f, 10.0f, 1.0f }, {  5.0f, 0.0f, -10.0f, 1.0f });
	// m_lighting->EditLight(1, {  5.0f, 0.0f, 10.0f, 1.0f }, { -5.0f, 0.0f, -10.0f, 1.0f });

	// eye position - will always bind second slot in Pixel Shader
	m_eyePositionBufferArray = std::make_unique<EyePositionBufferArray>(m_moveLookController);

	// materials
	m_materialBufferArray = std::make_unique<MaterialBufferArray>();

	// Initialize data that will be used when rendering in "All Sphere" mode
	InitializeAllSphereData();

	// Initialize data that will be used when render lights in the scene
	InitializeLightingData();
}

void Renderer::InitializeAllSphereData() noexcept
{
	PROFILE_FUNCTION();

	m_allSphere_InputLayout = std::make_unique<InputLayout>(L"PhongInstancedVS.cso", BasicGeometry::SPHERE);
	m_allSphere_VertexShader = std::make_unique<VertexShader>(m_allSphere_InputLayout->GetVertexShaderFileBlob());
	m_allSphere_PixelShader = std::make_unique<PixelShader>(L"PhongInstancedPS.cso");
	m_allSphere_Mesh = std::make_unique<SphereMesh>();
	m_allSphere_RasterizerState = std::make_unique<RasterizerState>();
	m_allSphere_DepthStencilState = std::make_unique<DepthStencilState>(1);

	// VS constant buffer
	std::shared_ptr<ConstantBuffer> mvpBuffer = std::make_shared<ConstantBuffer>();
	mvpBuffer->CreateBuffer<ModelViewProjectionPreMultipliedArray>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_allSphere_ModelViewProjectionInstanceBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	m_allSphere_ModelViewProjectionInstanceBufferArray->AddBuffer(mvpBuffer);

	// PS constant buffer
	std::shared_ptr<ConstantBuffer> materialIndexArrayBuffer = std::make_shared<ConstantBuffer>();
	materialIndexArrayBuffer->CreateBuffer<PhongMaterialIndexArray>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_allSphere_MaterialIndexInstanceBufferArray = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	m_allSphere_MaterialIndexInstanceBufferArray->AddBuffer(materialIndexArrayBuffer);
}

void Renderer::InitializeLightingData() noexcept
{
	PROFILE_FUNCTION();

	m_lighting_InputLayout = std::make_unique<InputLayout>(L"PhongVS.cso", BasicGeometry::SPHERE);
	m_lighting_VertexShader = std::make_unique<VertexShader>(m_lighting_InputLayout->GetVertexShaderFileBlob());
	m_lighting_PixelShader = std::make_unique<PixelShader>(L"PhongPS.cso");
	m_lighting_Mesh = std::make_unique<SphereMesh>();
	m_lighting_RasterizerState = std::make_unique<RasterizerState>();
	m_lighting_DepthStencilState = std::make_unique<DepthStencilState>(1);

	// VS constant buffer
	std::shared_ptr<ConstantBuffer> mvpBuffer = std::make_shared<ConstantBuffer>();
	mvpBuffer->CreateBuffer<ModelViewProjectionPreMultiplied>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_lighting_ModelViewProjectionBuffer = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	m_lighting_ModelViewProjectionBuffer->AddBuffer(mvpBuffer);

	// PS constant buffer
	std::shared_ptr<ConstantBuffer> materialIndexBuffer = std::make_shared<ConstantBuffer>();
	materialIndexBuffer->CreateBuffer<PhongMaterialIndexBuffer>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
	m_lighting_MaterialIndexBuffer = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::PIXEL_SHADER);
	m_lighting_MaterialIndexBuffer->AddBuffer(materialIndexBuffer);
}

void Renderer::SetViewport(D3D11_VIEWPORT viewport) noexcept 
{ 
	PROFILE_FUNCTION();

	// If they are different, then we need to update the viewport and also the projection matrix
	if (m_viewport.Height != viewport.Height ||
		m_viewport.Width != viewport.Width ||
		m_viewport.TopLeftX != viewport.TopLeftX ||
		m_viewport.TopLeftY != viewport.TopLeftY)
	{
		m_viewport = viewport;

		m_moveLookController->CreateProjectionMatrix(m_viewport);
	}	 
}

void Renderer::Update() noexcept
{
	PROFILE_FUNCTION();

	const std::vector<Particle>& particles = SimulationManager::GetParticles();
	std::unique_ptr<Sphere> sphere;

	if (particles.size() != m_drawables.size())
	{
		PROFILE_SCOPE("Renderer Update - Resize");

		m_drawables.clear();
		m_drawables.reserve(particles.size());

		size_t size = particles.size();
		for (unsigned int iii = 0; iii < size; ++iii)
		{
			sphere = std::make_unique<Sphere>(m_moveLookController);
			sphere->Position(particles[iii].p_x, particles[iii].p_y, particles[iii].p_z);
			sphere->Velocity(particles[iii].v_x, particles[iii].v_y, particles[iii].v_z);
			sphere->SetAtomType(particles[iii].type);

			m_drawables.push_back(std::move(sphere));
		}
	}
	else
	{
		PROFILE_SCOPE("Renderer Update - NO Resize");

		size_t size = particles.size();
		for (unsigned int iii = 0; iii < size; ++iii)
		{
			m_drawables[iii]->Position(particles[iii].p_x, particles[iii].p_y, particles[iii].p_z);
			m_drawables[iii]->Velocity(particles[iii].v_x, particles[iii].v_y, particles[iii].v_z);
		}
	}

	// I don't think this does anything right now...
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Update();

	// Update the MoveLookController
	m_moveLookController->Update(m_viewport);
}

void Renderer::ProcessMouseEvents() noexcept
{
	PROFILE_FUNCTION();

	while (!Mouse::IsEmpty())
	{
		Mouse::Event e = Mouse::Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::LPress:		OnLPress(e); break;
		case Mouse::Event::Type::LRelease:		OnLRelease(e); break;
		case Mouse::Event::Type::LDoubleClick:	OnLDoubleClick(e); break;
		case Mouse::Event::Type::RPress:		OnRPress(e); break;
		case Mouse::Event::Type::RRelease:		OnRRelease(e); break;
		case Mouse::Event::Type::MPress:		OnMPress(e); break;
		case Mouse::Event::Type::MRelease:		OnMRelease(e); break;
		case Mouse::Event::Type::WheelUp:		OnWheelUp(e); break;
		case Mouse::Event::Type::WheelDown:		OnWheelDown(e); break;
		case Mouse::Event::Type::Move:			OnMouseMove(e); break;
		case Mouse::Event::Type::Enter:			OnMouseEnter(e); break;
		case Mouse::Event::Type::Leave:			OnMouseLeave(e); break;
		case Mouse::Event::Type::Invalid:		OnMouseInvalidEvent(e); break;
		}
	}
}

void Renderer::ProcessKeyboardEvents() noexcept
{
	PROFILE_FUNCTION();

	// Read Key events and only process non-character keys
	while (!Keyboard::KeyIsEmpty())
	{
		Keyboard::Event e = Keyboard::ReadKey();

		switch (e.GetCode())
		{
		case VK_UP:      OnUpArrowKeyEvent(e); break;
		case VK_DOWN:    OnDownArrowKeyEvent(e); break;
		case VK_LEFT:    OnLeftArrowKeyEvent(e); break;
		case VK_RIGHT:	 OnRightArrowKeyEvent(e); break;
		case VK_SHIFT:	 OnShiftKeyEvent(e); break;
		case VK_CONTROL: OnCtrlKeyEvent(e); break;
		}
	}

	while (!Keyboard::CharIsEmpty())
	{
		OnCharEvent(Keyboard::ReadChar());

		switch (Keyboard::ReadChar())
		{
		//case 'p': Play/Pause the simulation
		case 'w': break;
		case 'a': break;
		case 's': break;
		case 'd': break;
		case 'c': break;
		}
	}

}

void Renderer::Render() noexcept
{
	PROFILE_FUNCTION();

	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Bind what will NOT change between drawables - i.e. Once per frame
	m_lighting->Bind();
	m_eyePositionBufferArray->Bind();
	m_eyePositionBufferArray->Update(); // This update should always come immediate AFTER the Bind call so we are guaranteed that the buffer is bound to the correct slot
	m_materialBufferArray->Bind();

	// This should be a functional/lambda that is SPECIFIC to the draw settings
	//		For example, when drawing every atom as a sphere, we can make certain
	//		improvements to the draw pipeline such as instanced rendering
	Render_AllSpheres();
	//Render_Generic();

	// Draw the box
	m_box->Draw();

	// If editing the lights, draw them in the scene
	if (m_drawLights)
		Render_Lights();
}

void Renderer::Render_Generic() const noexcept
{
	PROFILE_FUNCTION();

	// Draw each drawable
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Draw();
}

void Renderer::Render_AllSpheres() const noexcept
{
	PROFILE_FUNCTION();

	// Specialized Render function for use when all atoms are to be drawn as spheres

	m_allSphere_InputLayout->Bind();
	m_allSphere_VertexShader->Bind();
	m_allSphere_PixelShader->Bind();
	m_allSphere_Mesh->Bind();
	m_allSphere_RasterizerState->Bind();
	m_allSphere_DepthStencilState->Bind();
	m_allSphere_ModelViewProjectionInstanceBufferArray->Bind();
	m_allSphere_MaterialIndexInstanceBufferArray->Bind();

	// Must update the buffers AFTER they are bound to the pipeline
	UpdateAllSphereModelViewProjectionInstanceData();
	UpdateAllSphereMaterialIndexInstanceData();

	// Issue the DrawIndexedInstanced call
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DrawIndexedInstanced(m_allSphere_Mesh->IndexCount(), static_cast<UINT>(m_drawables.size()), 0u, 0u, 0u)
	);
}

void Renderer::Render_Lights() const noexcept
{
	PROFILE_FUNCTION();

	m_lighting_InputLayout->Bind();
	m_lighting_VertexShader->Bind();
	m_lighting_PixelShader->Bind();
	m_lighting_Mesh->Bind();
	m_lighting_RasterizerState->Bind();
	m_lighting_DepthStencilState->Bind();
	m_lighting_ModelViewProjectionBuffer->Bind();
	m_lighting_MaterialIndexBuffer->Bind();

	XMMATRIX viewProjection = m_moveLookController->ViewMatrix() * m_moveLookController->ProjectionMatrix();
	XMMATRIX model;
	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	D3D11_MAPPED_SUBRESOURCE ms;
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	LightProperties* properties = m_lighting->GetLightProperties();
	for (unsigned int iii = 0; iii < MAX_LIGHTS; ++iii)
	{
		if (properties->Lights[iii].Enabled)
		{
			// Update Model/View/Projection buffer ---------------------------------
			ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

			GFX_THROW_INFO_ONLY(
				context->VSGetConstantBuffers(0, 1, buffer.ReleaseAndGetAddressOf())
			);

			GFX_THROW_INFO(
				context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)
			);

			ModelViewProjectionPreMultiplied* mvpMappedBuffer = (ModelViewProjectionPreMultiplied*)ms.pData;

			model = DirectX::XMMatrixScaling(0.25f, 0.25f, 0.25f) * DirectX::XMMatrixTranslation(properties->Lights[iii].Position.x, properties->Lights[iii].Position.y, properties->Lights[iii].Position.z);

			DirectX::XMStoreFloat4x4(&(mvpMappedBuffer->model), model);
			DirectX::XMStoreFloat4x4(&(mvpMappedBuffer->modelViewProjection), model * viewProjection);
			DirectX::XMStoreFloat4x4(&(mvpMappedBuffer->inverseTransposeModel), DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, model)));

			GFX_THROW_INFO_ONLY(
				context->Unmap(buffer.Get(), 0)
			);

			// Update Pixel Shader buffer ---------------------------------
			ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

			GFX_THROW_INFO_ONLY(
				context->PSGetConstantBuffers(3, 1, buffer.ReleaseAndGetAddressOf())
			);

			GFX_THROW_INFO(
				context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)
			);

			PhongMaterialIndexBuffer* indexMappedBuffer = (PhongMaterialIndexBuffer*)ms.pData;
			indexMappedBuffer->materialIndex = 0; // Just use value same as hydrogen (white)

			GFX_THROW_INFO_ONLY(
				context->Unmap(buffer.Get(), 0)
			);

			GFX_THROW_INFO_ONLY(
				DeviceResources::D3DDeviceContext()->DrawIndexed(m_lighting_Mesh->IndexCount(), 0u, 0u)
			);
		}
	}
	
}

void Renderer::UpdateAllSphereModelViewProjectionInstanceData() const noexcept
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

	ModelViewProjectionPreMultipliedArray* mappedBuffer = (ModelViewProjectionPreMultipliedArray*)ms.pData;

	XMMATRIX viewProjection = m_moveLookController->ViewMatrix() * m_moveLookController->ProjectionMatrix();
	XMMATRIX model;

	size_t size = m_drawables.size();
	for (unsigned int iii = 0; iii < size; ++iii)
	{
		model = m_drawables[iii]->GetModelMatrix();

		DirectX::XMStoreFloat4x4(&(mappedBuffer->instanceData[iii].model), model);
		DirectX::XMStoreFloat4x4(&(mappedBuffer->instanceData[iii].modelViewProjection), model * viewProjection);
		DirectX::XMStoreFloat4x4(&(mappedBuffer->instanceData[iii].inverseTransposeModel), DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, model)));
	}

	GFX_THROW_INFO_ONLY(
		context->Unmap(vsBuffer.Get(), 0)
	);
}

void Renderer::UpdateAllSphereMaterialIndexInstanceData() const noexcept
{
	PROFILE_FUNCTION();

	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Update VS constant buffer with model/view/projection info
	Microsoft::WRL::ComPtr<ID3D11Buffer> psBuffer;
	GFX_THROW_INFO_ONLY(
		context->PSGetConstantBuffers(3, 1, psBuffer.ReleaseAndGetAddressOf())
	);

	GFX_THROW_INFO(
		context->Map(psBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)
	);

	PhongMaterialIndexArray* mappedBuffer = (PhongMaterialIndexArray*)ms.pData;

	size_t size = m_drawables.size();
	for (unsigned int iii = 0; iii < size; ++iii)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(m_drawables[iii].get());
		mappedBuffer->materialIndex[iii].materialIndex = sphere->GetElementNumber() - 1;
	}

	GFX_THROW_INFO_ONLY(
		context->Unmap(psBuffer.Get(), 0)
	);
}

void Renderer::OnLPress(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnLPress(e);
}

void Renderer::OnLRelease(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnLRelease(e);
}

void Renderer::OnLDoubleClick(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnLDoubleClick(e);
}

void Renderer::OnRPress(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnRPress(e);
}

void Renderer::OnRRelease(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnRRelease(e);
}

void Renderer::OnMPress(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnMPress(e);
}

void Renderer::OnMRelease(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnMRelease(e);
}

void Renderer::OnWheelUp(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnWheelUp(e);
}

void Renderer::OnWheelDown(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnWheelDown(e);
}

void Renderer::OnMouseMove(const Mouse::Event& e) const noexcept
{
	m_moveLookController->OnMouseMove(e);
}

void Renderer::OnMouseEnter(const Mouse::Event& /* e */) const noexcept
{

}

void Renderer::OnMouseLeave(const Mouse::Event& /* e */) const noexcept
{

}

void Renderer::OnMouseInvalidEvent(const Mouse::Event& /* e */) const noexcept
{

}

void Renderer::OnUpArrowKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnUpArrowKeyEvent(e);
}

void Renderer::OnDownArrowKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnDownArrowKeyEvent(e);
}

void Renderer::OnLeftArrowKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnLeftArrowKeyEvent(e);
}

void Renderer::OnRightArrowKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnRightArrowKeyEvent(e);
}

void Renderer::OnCtrlKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnCtrlKeyEvent(e);
}

void Renderer::OnShiftKeyEvent(const Keyboard::Event& e) const noexcept
{
	m_moveLookController->OnShiftKeyEvent(e);
}

void Renderer::OnCharEvent(char c) const noexcept
{
	m_moveLookController->OnCharEvent(c);
}

void Renderer::OnResize(D3D11_VIEWPORT vp) noexcept
{
	m_viewport = vp;
	m_moveLookController->OnResize(m_viewport);
}