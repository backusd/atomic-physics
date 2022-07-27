#pragma once
#include "pch.h"
#include "Event.h"
#include "Simulation.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

using ParticleAddedEvent = Event<const Particle&, unsigned int>;
using ParticleAddedEventHandler = std::function<void(const Particle&, unsigned int)>;




class SimulationManager
{
public:
	SimulationManager(const SimulationManager&) = delete;
	void operator=(const SimulationManager&) = delete;

	static void Initialize() noexcept;
	static void Update() noexcept;

	static void AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;

	static const std::vector<Particle>& GetParticles() noexcept { return m_simulations[m_activeSimulationIndex]->GetParticles(); }
	static Particle& GetParticle(int index) noexcept { return m_simulations[m_activeSimulationIndex]->GetParticle(index); }
	static DirectX::XMFLOAT3 GetSimulationDimensions() noexcept { return m_simulations[m_activeSimulationIndex]->GetSimulationDimensions(); }
	static unsigned int ParticleCount() noexcept { return m_simulations[m_activeSimulationIndex]->ParticleCount(); }

	// Methods to query the StepTimer
	static double TotalSeconds() noexcept { return m_simulations[m_activeSimulationIndex]->TotalSeconds(); }

	static bool SimulationIsPlaying() noexcept { return m_simulations[m_activeSimulationIndex]->IsPlaying(); }
	static void SwitchPlayPause() noexcept { m_simulations[m_activeSimulationIndex]->SwitchPlayPause(); }

	static DirectX::XMFLOAT3 GetBoxSize() noexcept { return m_simulations[m_activeSimulationIndex]->GetBoxSize(); }
	static void SetBoxSize(float xyz) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(xyz); }
	static void SetBoxSize(DirectX::XMFLOAT3 size) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(size); }

	static const std::string& GetParticleName(unsigned int type) noexcept { return m_particleNames[type]; }
	static const std::vector<std::string>& GetParticleNames() noexcept { return m_particleNames; }

	// Events
	static EventToken SetParticleAddedEventHandler(ParticleAddedEventHandler handler) noexcept { return e_ParticleAdded.AddHandler(handler); }
	static void RemoveParticleAddedEventHandler(EventToken token) noexcept { e_ParticleAdded.RemoveHandler(token); }


private:
	SimulationManager(); // Don't allow construction

	static unsigned int m_activeSimulationIndex;
	static std::vector<std::unique_ptr<Simulation>> m_simulations;

	static const std::vector<std::string> m_particleNames;

	// Events
	static ParticleAddedEvent e_ParticleAdded;
};

