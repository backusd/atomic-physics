#pragma once
#include "pch.h"
#include "Simulation.h"


#include <memory>
#include <vector>

class SimulationManager
{
public:
	static void Initialize() noexcept;
	static void Update() noexcept;

private:
	SimulationManager(); // Don't allow construction

	static unsigned int m_activeSimulationIndex;
	static std::vector<std::unique_ptr<Simulation>> m_simulations;
};

