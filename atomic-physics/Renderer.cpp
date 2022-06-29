#include "Renderer.h"


Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp)
{
	m_moveLookController = std::make_shared<MoveLookController>(m_viewport);
}

void Renderer::Update() noexcept
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
			sphere->Radius(particles[iii].type); // FIX THIS

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

	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Update();
}

void Renderer::Render() noexcept
{
	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Draw each drawable
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Draw();
}

