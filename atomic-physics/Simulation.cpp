#include "Simulation.h"

using DirectX::XMFLOAT3;

Simulation::Simulation() noexcept :
	m_boxMaxX(2.0f),
	m_boxMaxY(2.0f),
	m_boxMaxZ(2.0f),
	m_elapsedTime(0),
	m_isPlaying(false)
{
	PROFILE_FUNCTION();

	m_timer = std::make_unique<StepTimer>();
}

void Simulation::Update() noexcept
{
	PROFILE_FUNCTION();

	m_timer->Tick([&]() noexcept
		{
			PROFILE_SCOPE("Simulation Update Physics");

			double currentTime = m_timer->GetTotalSeconds();
			double timeDelta = currentTime - m_elapsedTime;
			m_elapsedTime = currentTime;

			if (!m_isPlaying)
				return;

			if (timeDelta > 0.1)
				return;

			for (Particle& p : m_particles)
			{
				p.p_x += static_cast<float>(p.v_x * timeDelta);
				p.p_y += static_cast<float>(p.v_y * timeDelta);
				p.p_z += static_cast<float>(p.v_z * timeDelta);

				if (p.p_x > m_boxMaxX || p.p_x < -m_boxMaxX)
					p.v_x *= -1;

				if (p.p_y > m_boxMaxY || p.p_y < -m_boxMaxY)
					p.v_y *= -1;

				if (p.p_z > m_boxMaxZ || p.p_z < -m_boxMaxZ)
					p.v_z *= -1;
			}
		}
	);
}

Particle& Simulation::AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept
{
	PROFILE_FUNCTION();

	return m_particles.emplace_back(type, mass, p_x, p_y, p_z, v_x, v_y, v_z);
}

void Simulation::RemoveParticle(unsigned int index) noexcept
{
	m_particles.erase(m_particles.begin() + index);
}

XMFLOAT3 Simulation::GetBoxSize() const noexcept
{
	return { m_boxMaxX, m_boxMaxY, m_boxMaxZ };
}

void Simulation::SetBoxSize(float xyz) noexcept
{
	SetBoxSize({ xyz, xyz, xyz });
}

void Simulation::SetBoxSize(DirectX::XMFLOAT3 size) noexcept
{
	bool smallerBox = size.x < m_boxMaxX || size.y < m_boxMaxY || size.z < m_boxMaxZ;

	m_boxMaxX = size.x;
	m_boxMaxY = size.y;
	m_boxMaxZ = size.z;

	// If the box was made smaller, update any atoms that need to be moved inwards
	if (smallerBox)
	{
		for (Particle& particle : m_particles)
		{
			if (particle.p_x > m_boxMaxX)
				particle.p_x = m_boxMaxX;
			else if (particle.p_x < -m_boxMaxX)
				particle.p_x = -m_boxMaxX;

			if (particle.p_y > m_boxMaxY)
				particle.p_y = m_boxMaxY;
			else if (particle.p_y < -m_boxMaxY)
				particle.p_y = -m_boxMaxY;

			if (particle.p_z > m_boxMaxZ)
				particle.p_z = m_boxMaxZ;
			else if (particle.p_z < -m_boxMaxZ)
				particle.p_z = -m_boxMaxZ;
		}
	}
}