#include "Renderer.h"

using DirectX::XMFLOAT4;

Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp)
{
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
}

void Renderer::Update()
{
	const std::vector<Particle>& particles = SimulationManager::GetParticles();
	std::unique_ptr<Sphere> sphere;

	if (particles.size() != m_drawables.size())
	{
		m_drawables.clear();
		m_drawables.reserve(particles.size());

		for (unsigned int iii = 0; iii < particles.size(); ++iii)
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
		for (unsigned int iii = 0; iii < particles.size(); ++iii)
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

void Renderer::Render()
{
	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Bind what will NOT change between drawables - i.e. Once per frame
	m_lighting->Bind();
	m_eyePositionBufferArray->Bind();
	m_eyePositionBufferArray->Update(); // This update should always come immediate AFTER the Bind call so we are guaranteed that the buffer is bound to the correct slot
	m_materialBufferArray->Bind();

	// Draw each drawable
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Draw();

	// Draw the box
	m_box->Draw();
}

void Renderer::OnLPress(Mouse::Event e) const noexcept
{
	m_moveLookController->OnLPress(e);
}

void Renderer::OnLRelease(Mouse::Event e) const noexcept
{
	m_moveLookController->OnLRelease(e);
}

void Renderer::OnLDoubleClick(Mouse::Event e) const noexcept
{
	m_moveLookController->OnLDoubleClick(e);
}

void Renderer::OnRPress(Mouse::Event e) const noexcept
{
	m_moveLookController->OnRPress(e);
}

void Renderer::OnRRelease(Mouse::Event e) const noexcept
{
	m_moveLookController->OnRRelease(e);
}

void Renderer::OnMPress(Mouse::Event e) const noexcept
{
	m_moveLookController->OnMPress(e);
}

void Renderer::OnMRelease(Mouse::Event e) const noexcept
{
	m_moveLookController->OnMRelease(e);
}

void Renderer::OnWheelUp(Mouse::Event e) const noexcept
{
	m_moveLookController->OnWheelUp(e);
}

void Renderer::OnWheelDown(Mouse::Event e) const noexcept
{
	m_moveLookController->OnWheelDown(e);
}

void Renderer::OnMouseMove(Mouse::Event e) const noexcept
{
	m_moveLookController->OnMouseMove(e);
}

void Renderer::OnMouseEnter(Mouse::Event /* e */) const noexcept
{

}

void Renderer::OnMouseLeave(Mouse::Event /* e */) const noexcept
{

}

void Renderer::OnMouseInvalidEvent(Mouse::Event /* e */) const noexcept
{

}