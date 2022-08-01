#include "SimulationManager.h"

#include <algorithm>
#include <random>

const std::vector<std::string> SimulationManager::m_particleNames =
	{ "Electron", "Hydrogen", "Helium", "Lithium", "Beryllium", "Boron", "Carbon",
	"Nitrogen", "Oxygen", "Flourine", "Neon" };

const std::array<std::vector<IsotopeMassAbundance>, 11> SimulationManager::m_isotopeMassAbundanceList =
{ {
	{ { 0, 0.00000f } },									// electron
	{ { 1, 99.9700f }, { 2, 0.03000f },  { 3, 0.0f } },		// Hydrogen
	{ { 3, 0.00020f }, { 4, 99.9998f } },					// Helium
	{ { 6, 7.80000f }, { 7, 92.2000f } },					// Lithium
	{ { 9, 100.000f } },									// Beryllium
	{ { 10, 20.400f }, { 11, 79.600f } },					// Boron
	{ { 12, 98.840f }, { 13, 1.1600f } },					// Carbon
	{ { 14, 99.578f }, { 15, 0.4220f } },					// Nitrogen
	{ { 16, 99.738f }, { 17, 0.0400f }, { 18, 0.222f } },	// Oxygen
	{ { 19, 100.00f } },									// Flourine
	{ { 20, 90.480f }, { 21, 0.2700f }, { 22, 9.250f } }	// Neon
} };

std::vector<std::unique_ptr<Simulation>> SimulationManager::m_simulations;
unsigned int SimulationManager::m_activeSimulationIndex = 0;
std::optional<unsigned int> SimulationManager::m_firstTemporaryParticleIndex = std::nullopt;

PlayPauseEvent				SimulationManager::e_PlayPause;
ParticleAddedEvent			SimulationManager::e_ParticleAdded;
ParticleRemovedEvent		SimulationManager::e_ParticleRemoved;
ParticleTypeChangedEvent	SimulationManager::e_ParticleTypeChanged;
ParticleTypeChangedEvent	SimulationManager::e_ParticleMassChanged;

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
	// Delete any temporary particles (if they exist)
	DeleteTemporaryParticles();

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
	// If you are removing a NON-temporary particle but there are temporary particles, you must decrement the first temp particle index
	if (!IsParticleTemporary(index) && m_firstTemporaryParticleIndex.has_value()) m_firstTemporaryParticleIndex = m_firstTemporaryParticleIndex.value() - 1;

	m_simulations[m_activeSimulationIndex]->RemoveParticle(index);
	e_ParticleRemoved(index);
}

void SimulationManager::RemoveParticles(std::vector<unsigned int>& indices) noexcept
{
	// Sort the indices and then remove back to front
	std::sort(indices.begin(), indices.end());
	for (int iii = indices.size() - 1; iii >= 0; --iii)
		RemoveParticle(indices[iii]);
}

void SimulationManager::ChangeParticleType(unsigned int particleIndex, unsigned int type) noexcept
{
	// If the particle type was updated, trigger the event
	if (m_simulations[m_activeSimulationIndex]->ChangeParticleType(particleIndex, type))
	{
		e_ParticleTypeChanged(particleIndex, type);

		// Must now change the mass to be valid for the new type
		ChangeParticleMass(particleIndex, GetDefaultMass(type));
	}
}

void SimulationManager::ChangeParticleMass(unsigned int particleIndex, unsigned int mass) noexcept
{
	// If the particle mass was updated, trigger the event
	if (m_simulations[m_activeSimulationIndex]->ChangeParticleMass(particleIndex, mass))
		e_ParticleMassChanged(particleIndex, mass);
}



Particle& SimulationManager::GetFirstOrCreateTemporaryParticle(unsigned int type) noexcept
{
	if (m_firstTemporaryParticleIndex.has_value())
	{
		ChangeParticleType(m_firstTemporaryParticleIndex.value(), type);
		return m_simulations[m_activeSimulationIndex]->GetParticle(m_firstTemporaryParticleIndex.value());
	}
	
	AddParticle(type, GetDefaultMass(type), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_firstTemporaryParticleIndex = m_simulations[m_activeSimulationIndex]->ParticleCount() - 1;
	return m_simulations[m_activeSimulationIndex]->GetParticle(m_firstTemporaryParticleIndex.value());
}

void SimulationManager::DeleteTemporaryParticles() noexcept
{
	if (m_firstTemporaryParticleIndex.has_value())
	{
		int value = m_firstTemporaryParticleIndex.value();
		for (int iii = m_simulations[m_activeSimulationIndex]->ParticleCount() - 1; iii >= value; --iii)
			RemoveParticle(iii);

		m_firstTemporaryParticleIndex = std::nullopt;
	}
}

void SimulationManager::PublishTemporaryParticles() noexcept
{
	// The particles already exists in the simulation - simply make the temporaryindex nullopt
	m_firstTemporaryParticleIndex = std::nullopt;
}

bool SimulationManager::IsParticleTemporary(unsigned int particleIndex) noexcept
{
	return m_firstTemporaryParticleIndex.has_value() && particleIndex >= m_firstTemporaryParticleIndex.value();
}

void SimulationManager::PlaceRandomParticles(const std::vector<unsigned int>& allowedTypes, unsigned int numberOfParticlesToCreate, float maxVelocity) noexcept
{
	// If the first temporary particle index already has a value, it will be assumed that it is from creating 
	// 1+ random particles. Therefore, in this call to add more random particles, we don't want to update it
	// because that would make the previous random particles permanent
	if (!m_firstTemporaryParticleIndex.has_value() && numberOfParticlesToCreate > 0)
		m_firstTemporaryParticleIndex = ParticleCount();

	DirectX::XMFLOAT3 boxMaxXYZ = GetBoxSize();

	// Create distributions on the heap because they take up a lot of stack space and you will get a warning
	std::default_random_engine engine;
	auto typeIndexGenerator = std::make_unique<std::uniform_int_distribution<unsigned int>>(0, static_cast<unsigned int>(allowedTypes.size() - 1));
	auto positionGenerator = std::make_unique<std::uniform_real_distribution<float>>(-1.0f, 1.0f);
	auto velocityGenerator = std::make_unique<std::uniform_real_distribution<float>>(-maxVelocity, maxVelocity);

	for (unsigned int iii = 0; iii < numberOfParticlesToCreate; ++iii)
	{
		int typeIndex = (*typeIndexGenerator)(engine);
		unsigned int mass = GetDefaultMass(allowedTypes[typeIndex]);
		
		float p_x = (*positionGenerator)(engine) * boxMaxXYZ.x;
		float p_y = (*positionGenerator)(engine) * boxMaxXYZ.y;
		float p_z = (*positionGenerator)(engine) * boxMaxXYZ.z;

		float v_x = (*velocityGenerator)(engine);
		float v_y = (*velocityGenerator)(engine);
		float v_z = (*velocityGenerator)(engine);

		AddParticle(allowedTypes[typeIndex], mass, p_x, p_y, p_z, v_x, v_y, v_z);
	}
}