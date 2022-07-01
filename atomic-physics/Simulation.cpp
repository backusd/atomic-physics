#include "Simulation.h"

Simulation::Simulation() :
	m_boxMinX(-2.0f),
	m_boxMaxX(2.0f),
	m_boxMinY(-2.0f),
	m_boxMaxY(2.0f),
	m_boxMinZ(-2.0f),
	m_boxMaxZ(2.0f),
	m_elapsedTime(0)
{
	m_timer = std::make_unique<StepTimer>();
}

void Simulation::Update() noexcept
{
	m_timer->Tick([&]()
		{
			double currentTime = m_timer->GetTotalSeconds();
			double timeDelta = currentTime - m_elapsedTime;
			m_elapsedTime = currentTime;

			if (timeDelta > 0.1) // if time delta 
				return;

			for (Particle& p : m_particles)
			{
				p.p_x += static_cast<float>(p.v_x * timeDelta);
				p.p_y += static_cast<float>(p.v_y * timeDelta);
				p.p_z += static_cast<float>(p.v_z * timeDelta);

				if (p.p_x > m_boxMaxX || p.p_x < m_boxMinX)
					p.v_x *= -1;

				if (p.p_y > m_boxMaxY || p.p_y < m_boxMinY)
					p.v_y *= -1;

				if (p.p_z > m_boxMaxZ || p.p_z < m_boxMinZ)
					p.v_z *= -1;
			}
		}
	);
}

void Simulation::AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept
{
	m_particles.emplace_back(type, mass, p_x, p_y, p_z, v_x, v_y, v_z);
}