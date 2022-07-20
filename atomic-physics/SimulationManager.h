#pragma once
#include "pch.h"
#include "Simulation.h"


#include <memory>
#include <vector>

class SimulationManager
{
public:
	SimulationManager(const SimulationManager&) = delete;
	void operator=(const SimulationManager&) = delete;

	static void Initialize() noexcept;
	static void Update() noexcept;

	static void AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;

	static const std::vector<Particle>& GetParticles() noexcept { return m_simulations[m_activeSimulationIndex]->GetParticles(); }
	static DirectX::XMFLOAT3 GetSimulationDimensions() noexcept { return m_simulations[m_activeSimulationIndex]->GetSimulationDimensions(); }

	// Methods to query the StepTimer
	static double TotalSeconds() noexcept { return m_simulations[m_activeSimulationIndex]->TotalSeconds(); }

	static bool SimulationIsPlaying() noexcept { return m_simulations[m_activeSimulationIndex]->IsPlaying(); }
	static void SwitchPlayPause() noexcept { m_simulations[m_activeSimulationIndex]->SwitchPlayPause(); }

	static DirectX::XMFLOAT3 GetBoxSize() noexcept { return m_simulations[m_activeSimulationIndex]->GetBoxSize(); }
	static void SetBoxSize(float xyz) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(xyz); }
	static void SetBoxSize(DirectX::XMFLOAT3 size) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(size); }

private:
	SimulationManager(); // Don't allow construction

	static unsigned int m_activeSimulationIndex;
	static std::vector<std::unique_ptr<Simulation>> m_simulations;
};

