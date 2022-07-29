#pragma once
#include "pch.h"
#include "StepTimer.h"

#include <vector>
#include <memory>

struct Particle
{
	unsigned int type; // 0 --> electron, N > 0 --> element number (number of protons)
	unsigned int mass; // [only for non-electron] #Proton + #Neutron
	float p_x;		   // Position
	float p_y;
	float p_z;
	float v_x;	       // Velocity
	float v_y;
	float v_z;
};

class Simulation
{
public:
	Simulation() noexcept;
	Simulation(const Simulation&) = delete;
	void operator=(const Simulation&) = delete;
	// ~Simulation --> automatically gets called because Simulation does get destructed on program close

	void Update() noexcept;

	Particle& AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;
	const std::vector<Particle>& GetParticles() const noexcept { return m_particles; }
	Particle& GetParticle(int index) noexcept { return m_particles[index]; }
	unsigned int ParticleCount() const noexcept { return static_cast<unsigned int>(m_particles.size()); }
	void RemoveParticle(unsigned int index) noexcept;
	
	DirectX::XMFLOAT3 GetSimulationDimensions() const noexcept { return { 2 * m_boxMaxX, 2 * m_boxMaxY, 2 * m_boxMaxZ }; }

	double TotalSeconds() const noexcept { return m_timer->GetTotalSeconds(); }

	bool IsPlaying() const noexcept { return m_isPlaying; }
	bool SwitchPlayPause() noexcept { m_isPlaying = !m_isPlaying; return m_isPlaying; }

	DirectX::XMFLOAT3 GetBoxSize() const noexcept;
	void SetBoxSize(float xyz) noexcept;
	void SetBoxSize(DirectX::XMFLOAT3 size) noexcept;

private:
	
	std::unique_ptr<StepTimer> m_timer;
	std::vector<Particle> m_particles;
	float m_boxMaxX, m_boxMaxY, m_boxMaxZ;
	double m_elapsedTime;
	bool m_isPlaying;
};