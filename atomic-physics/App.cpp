#include "App.h"
#include "MacroHelper.h"

#include "implot.h"

#include <format>

App::App() noexcept
{
	PROFILE_BEGIN_SESSION("Startup", "profile/Profile-Startup.json");

	// Initialize ImGui
	//		MUST set this here so that the AppWindow constructor can call ImGui::GetIO()
	//		Setup Dear ImGui context
	
	// Does not throw, but will call IM_ASSERT if it finds an error. However, the IM_ASSERT macro may be
	// updated at some point, so don't rely 100% on an assert actually being fired. Instead, check for
	// an error return code and handle it appropriately.
	if (!IMGUI_CHECKVERSION())
	{
		ERROR_POPUP(std::format("IMGUI_CHECKVERSION returned error status\nSee File: {} - Line: {}", __FILE__, __LINE__).c_str(), "IMGUI_CHECKVERSION ERROR");
		std::terminate();
	}

	// Run each function in a try-catch block and terminate with a popup window if any exception is thrown
	TERMINATE_ON_THROW(ImGui::CreateContext())
	TERMINATE_ON_THROW(ImPlot::CreateContext())

	// Create the window
	m_window = std::make_unique<AppWindow>(1000, 800, "Main Window");

	// Initialize the device resources
	DeviceResources::Initialize(m_window->GetHWND());

	// Initialize the Simulation Manager
	SimulationManager::Initialize();

	// The content within AppWindow does NOT get created in the AppWindow constructor. This is to
	// allow the AppWindowTemplate the ability to create itself and give us a window, however,
	// the window contents need access to DeviceResources, so you MUST initialize DeviceResources 
	// in between constructing AppWindow and calling AppWindow->Initialize(). The same holds true
	// for initializing ImGui as well
	m_window->Initialize();

	// Even though adding atoms here is temporary, it MUST be done AFTER window initialization because
	// Initialize() will create the Renderer which will create a ParticleAdded event handler
	SimulationManager::AddParticle(1, SimulationManager::GetDefaultMass(1), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	/*
	SimulationManager::AddParticle(2, SimulationManager::GetDefaultMass(2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(3, SimulationManager::GetDefaultMass(3), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(4, SimulationManager::GetDefaultMass(4), 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(5, SimulationManager::GetDefaultMass(5), 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	SimulationManager::AddParticle(6, SimulationManager::GetDefaultMass(6), 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(7, SimulationManager::GetDefaultMass(7), 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(8, SimulationManager::GetDefaultMass(8), 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(9, SimulationManager::GetDefaultMass(9), 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f);
	SimulationManager::AddParticle(10, SimulationManager::GetDefaultMass(10), 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 10.0f);
	*/

	PROFILE_END_SESSION();
}

int App::Run() const
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = m_window->ProcessMessages())
		{
			PROFILE_BEGIN_SESSION("Shutdown", "profile/Profile-Shutdown.json");

			// Shutdown ImGui
			{
				PROFILE_SCOPE("ImGui_ImplDX11_Shutdown");
				ImGui_ImplDX11_Shutdown();
			}
			{
				PROFILE_SCOPE("ImGui_ImplWin32_Shutdown");
				ImGui_ImplWin32_Shutdown();
			}
			{
				PROFILE_SCOPE("ImPlot::DestroyContext()");
				ImPlot::DestroyContext();
			}
			{
				PROFILE_SCOPE("ImGui::DestroyContext()");
				ImGui::DestroyContext();
			}

			PROFILE_END_SESSION(); // End shutdown

			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		// Inform the Instrumentor that we are starting the next frame
		PROFILE_NEXT_FRAME();

		// Update the active simulation before updating the window
		SimulationManager::Update();

		// Update the window-side data and render
		m_window->Update();
		if (m_window->Render())
			m_window->Present();
	}
}