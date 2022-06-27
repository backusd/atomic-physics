#include "SimulationManager.h"


std::vector<std::unique_ptr<Simulation>> SimulationManager::m_simulations;
unsigned int SimulationManager::m_activeSimulationIndex = 0;

void SimulationManager::Initialize() noexcept
{
	m_simulations.push_back(std::make_unique<Simulation>());
	m_activeSimulationIndex = 0;
}

void SimulationManager::Update() noexcept
{
	m_simulations[m_activeSimulationIndex]->Update();
}