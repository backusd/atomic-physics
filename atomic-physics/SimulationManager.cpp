#include "SimulationManager.h"


std::vector<std::unique_ptr<Simulation>> SimulationManager::m_simulations;
unsigned int SimulationManager::m_activeSimulationIndex = 0;
const std::vector<std::string> SimulationManager::m_particleNames =
	{ "Electron", "Hydrogen", "Helium", "Lithium", "Beryllium", "Boron", "Carbon",
	"Nitrogen", "Oxygen", "Flourine", "Neon" };

ParticleAddedEvent		SimulationManager::e_ParticleAdded;
ParticleRemovedEvent	SimulationManager::e_ParticleRemoved;

void SimulationManager::Initialize() noexcept
{
	PROFILE_FUNCTION();

	m_simulations.push_back(std::make_unique<Simulation>());
	m_activeSimulationIndex = 0;
}

void SimulationManager::Update() noexcept
{
	m_simulations[m_activeSimulationIndex]->Update();
}

void SimulationManager::AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept
{
	PROFILE_FUNCTION();

	// Add the particle to the simulation and then trigger the ParticleAdded event
	Particle p = m_simulations[m_activeSimulationIndex]->AddParticle(type, mass, p_x, p_y, p_z, v_x, v_y, v_z);
	e_ParticleAdded(p, m_simulations[m_activeSimulationIndex]->ParticleCount());
}

void SimulationManager::RemoveParticle(unsigned int index) noexcept
{ 
	m_simulations[m_activeSimulationIndex]->RemoveParticle(index);
	e_ParticleRemoved(index);
}