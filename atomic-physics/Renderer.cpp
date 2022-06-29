#include "Renderer.h"

using DirectX::XMFLOAT4;

Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp)
{
	m_moveLookController = std::make_shared<MoveLookController>(m_viewport);
	
	// lighting
	m_lighting = std::make_unique<Lighting>();
	m_lighting->GlobalAmbient(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	m_lighting->EditLight(0, { -5.0f, 0.0f, 10.0f, 1.0f }, {  5.0f, 0.0f, -10.0f, 1.0f });
	// m_lighting->EditLight(1, {  5.0f, 0.0f, 10.0f, 1.0f }, { -5.0f, 0.0f, -10.0f, 1.0f });

	// eye position - will always bind second slot in Pixel Shader
	m_eyePositionBufferArray = std::make_unique<EyePositionBufferArray>(m_moveLookController);
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
			sphere->Radius(Constants::AtomicRadii[particles[iii].type]); // FIX THIS

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
}

void Renderer::Render()
{
	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Bind what will NOT change between drawables - i.e. Once per frame
	m_lighting->Bind();
	m_eyePositionBufferArray->Bind();
	m_eyePositionBufferArray->Update(); // This update should always come immediate AFTER the Bind call so we are guaranteed that the buffer is bound to the correct slot

	// Draw each drawable
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Draw();
}

