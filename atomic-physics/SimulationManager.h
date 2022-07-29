#pragma once
#include "pch.h"
#include "Event.h"
#include "Simulation.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Play/Pause
using PlayPauseEvent = Event<bool>;
using PlayPauseEventHandler = std::function<void(bool)>;
// Particle Added
using ParticleAddedEvent = Event<const Particle&, unsigned int>;
using ParticleAddedEventHandler = std::function<void(const Particle&, unsigned int)>;
// Particle Removed
using ParticleRemovedEvent = Event<unsigned int>;
using ParticleRemovedEventHandler = std::function<void(unsigned int)>;
// ParticleTypeChanged
using ParticleTypeChangedEvent = Event<unsigned int, unsigned int>; // particle index, new type
using ParticleTypeChangedEventHandler = std::function<void(unsigned int, unsigned int)>;

class SimulationManager
{
public:
	SimulationManager(const SimulationManager&) = delete;
	void operator=(const SimulationManager&) = delete;

	static void Initialize() noexcept;
	static void Update() noexcept;

	static Particle& AddParticle(int type, int mass, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z) noexcept;
	static void RemoveParticle(unsigned int index) noexcept;

	static const std::vector<Particle>& GetParticles() noexcept { return m_simulations[m_activeSimulationIndex]->GetParticles(); }
	static Particle& GetParticle(int index) noexcept { return m_simulations[m_activeSimulationIndex]->GetParticle(index); }
	static DirectX::XMFLOAT3 GetSimulationDimensions() noexcept { return m_simulations[m_activeSimulationIndex]->GetSimulationDimensions(); }
	static unsigned int ParticleCount() noexcept { return m_simulations[m_activeSimulationIndex]->ParticleCount(); }

	// Methods to query the StepTimer
	static double TotalSeconds() noexcept { return m_simulations[m_activeSimulationIndex]->TotalSeconds(); }

	static bool SimulationIsPlaying() noexcept { return m_simulations[m_activeSimulationIndex]->IsPlaying(); }
	static void SwitchPlayPause() noexcept;

	static DirectX::XMFLOAT3 GetBoxSize() noexcept { return m_simulations[m_activeSimulationIndex]->GetBoxSize(); }
	static void SetBoxSize(float xyz) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(xyz); }
	static void SetBoxSize(DirectX::XMFLOAT3 size) noexcept { m_simulations[m_activeSimulationIndex]->SetBoxSize(size); }

	static const std::string& GetParticleName(unsigned int type) noexcept { return m_particleNames[type]; }
	static const std::vector<std::string>& GetParticleNames() noexcept { return m_particleNames; }
	static constexpr unsigned int GetDefaultMass(unsigned int type) noexcept;
	static void ChangeParticleType(unsigned int particleIndex, unsigned int type) noexcept;

	// Temporary Particle Functions
	static Particle& GetOrCreateTemporaryParticle(unsigned int type) noexcept;
	static unsigned int GetIndexOfTemporaryParticle() noexcept { return m_temporaryParticleIndex.value(); }
	static void DeleteTemporaryParticle() noexcept;
	static void PublishTemporaryParticle() noexcept;



	// Events
	static EventToken SetPlayPauseEventHandler(PlayPauseEventHandler handler) noexcept { return e_PlayPause.AddHandler(handler); }
	static bool RemovePlayPauseEventHandler(EventToken token) noexcept { return e_PlayPause.RemoveHandler(token); }

	static EventToken SetParticleAddedEventHandler(ParticleAddedEventHandler handler) noexcept { return e_ParticleAdded.AddHandler(handler); }
	static bool RemoveParticleAddedEventHandler(EventToken token) noexcept { return e_ParticleAdded.RemoveHandler(token); }

	static EventToken SetParticleRemovedEventHandler(ParticleRemovedEventHandler handler) noexcept { return e_ParticleRemoved.AddHandler(handler); }
	static bool RemoveParticleRemovedEventHandler(EventToken token) noexcept { return e_ParticleRemoved.RemoveHandler(token); }

	static EventToken SetParticleTypeChangedEventHandler(ParticleTypeChangedEventHandler handler) noexcept { return e_ParticleTypeChanged.AddHandler(handler); }
	static bool RemoveParticleTypeChangedEventHandler(EventToken token) noexcept { return e_ParticleTypeChanged.RemoveHandler(token); }

private:
	SimulationManager(); // Don't allow construction

	static unsigned int m_activeSimulationIndex;
	static std::vector<std::unique_ptr<Simulation>> m_simulations;

	static const std::vector<std::string> m_particleNames;

	// Index for temporary particle that is being added
	static std::optional<unsigned int> m_temporaryParticleIndex;

	// Events
	static PlayPauseEvent			e_PlayPause;
	static ParticleAddedEvent		e_ParticleAdded;
	static ParticleRemovedEvent		e_ParticleRemoved;
	static ParticleTypeChangedEvent	e_ParticleTypeChanged;
};

// Must be defined in the header file to avoid a compiler warning about
// "no definition for inline function"
constexpr unsigned int SimulationManager::GetDefaultMass(unsigned int type) noexcept
{
	switch (type)
	{
	case 0: return 0;	// electron
	case 1: return 1;	// Hydrogen
	case 2: return 4;	// Helium
	case 3: return 7;	// Lithium
	case 4: return 9;	// Beryllium
	case 5: return 11;	// Boron
	case 6: return 12;	// Carbon
	case 7: return 14;	// Nitrogen
	case 8: return 16;	// Oxygen
	case 9: return 19;	// Flourine
	case 10: return 20;	// Neon
	default:
		return 0;
	}
}