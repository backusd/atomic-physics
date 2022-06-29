#pragma once
#include "pch.h"
#include "StepTimer.h"

#include <vector>
#include <memory>

struct Particle
{
	int   type;	// 0 --> electron, N > 0 --> element number (number of protons)
	int	  mass; // [only for non-electron] #Proton + #Neutron
	float p_x;  // Position
	float p_y;
	float p_z;
	float v_x;	// Velocity
	float v_y;
	float v_z;
};

class Simulation
{
public:
	Simulation();
	Simulation(const Simulation&) = delete;
	void operator=(const Simulation&) = delete;
	// ~Simulation --> automatically gets called because Simulation does get destructed on program close

	void Update() noexcept;

	void AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;
	const std::vector<Particle>& GetParticles() noexcept { return m_particles; }

private:
	
	std::unique_ptr<StepTimer> m_timer;
	std::vector<Particle> m_particles;
	float m_boxMinX, m_boxMaxX, m_boxMinY, m_boxMaxY, m_boxMinZ, m_boxMaxZ;
	double m_elapsedTime;
};