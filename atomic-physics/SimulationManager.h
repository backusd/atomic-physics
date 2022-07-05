#pragma once
#include "pch.h"
#include "Simulation.h"


#include <memory>
#include <vector>

class SimulationManager
{
public:
	static void Initialize();
	static void Update() noexcept;

	static void AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;

	static const std::vector<Particle>& GetParticles() noexcept { return m_simulations[m_activeSimulationIndex]->GetParticles(); }
	static DirectX::XMFLOAT3 GetSimulationDimensions() noexcept { return m_simulations[m_activeSimulationIndex]->GetSimulationDimensions(); }

	// Methods to query the StepTimer
	static double TotalSeconds() noexcept { return m_simulations[m_activeSimulationIndex]->TotalSeconds(); }

private:
	SimulationManager(); // Don't allow construction

	static unsigned int m_activeSimulationIndex;
	static std::vector<std::unique_ptr<Simulation>> m_simulations;
};

