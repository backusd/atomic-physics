#include "SimulationManager.h"

const std::vector<std::string> SimulationManager::m_particleNames =
	{ "Electron", "Hydrogen", "Helium", "Lithium", "Beryllium", "Boron", "Carbon",
	"Nitrogen", "Oxygen", "Flourine", "Neon" };

std::vector<std::unique_ptr<Simulation>> SimulationManager::m_simulations;
unsigned int SimulationManager::m_activeSimulationIndex = 0;
std::optional<unsigned int> SimulationManager::m_temporaryParticleIndex = std::nullopt;

PlayPauseEvent				SimulationManager::e_PlayPause;
ParticleAddedEvent			SimulationManager::e_ParticleAdded;
ParticleRemovedEvent		SimulationManager::e_ParticleRemoved;
ParticleTypeChangedEvent	SimulationManager::e_ParticleTypeChanged;

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

void SimulationManager::SwitchPlayPause() noexcept
{ 
	// Delete the temporary particle (if there is one)
	DeleteTemporaryParticle();

	// Switch the play/pause state and then trigger the play pause event
	e_PlayPause(
		m_simulations[m_activeSimulationIndex]->SwitchPlayPause()
	);
}

Particle& SimulationManager::AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept
{
	PROFILE_FUNCTION();

	// Add the particle to the simulation and then trigger the ParticleAdded event
	Particle& p = m_simulations[m_activeSimulationIndex]->AddParticle(type, mass, p_x, p_y, p_z, v_x, v_y, v_z);
	e_ParticleAdded(p, m_simulations[m_activeSimulationIndex]->ParticleCount() - 1);
	return p;
}

void SimulationManager::RemoveParticle(unsigned int index) noexcept
{ 
	m_simulations[m_activeSimulationIndex]->RemoveParticle(index);
	e_ParticleRemoved(index);
}




Particle& SimulationManager::GetOrCreateTemporaryParticle(int type) noexcept
{
	if (m_temporaryParticleIndex.has_value())
	{
		Particle& p = m_simulations[m_activeSimulationIndex]->GetParticle(m_temporaryParticleIndex.value());

		if (p.type != type)
		{
			p.type = type;

			// Trigger particle type changed event
			// Params: particle index, new particle type
			e_ParticleTypeChanged(m_temporaryParticleIndex.value(), type);
		}
		return p;
	}
	
	AddParticle(type, GetDefaultMass(type), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_temporaryParticleIndex = m_simulations[m_activeSimulationIndex]->ParticleCount() - 1;
	return m_simulations[m_activeSimulationIndex]->GetParticle(m_temporaryParticleIndex.value());
}

void SimulationManager::DeleteTemporaryParticle() noexcept
{
	if (m_temporaryParticleIndex.has_value())
	{
		RemoveParticle(m_temporaryParticleIndex.value());
		m_temporaryParticleIndex = std::nullopt;
	}
}

