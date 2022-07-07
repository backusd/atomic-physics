#include "App.h"

App::App()
{
	// Initialize ImGui
	//		MUST set this here so that the AppWindow constructor can call ImGui::GetIO()
	//		Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Create the window
	m_window = std::make_unique<AppWindow>(1000, 800, "Main Window");

	// Initialize the device resources
	DeviceResources::Initialize(m_window->GetHWND());

	// Initialize the Simulation Manager
	SimulationManager::Initialize();

	SimulationManager::AddParticle(1, 1, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	SimulationManager::AddParticle(2, 4, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(3, 6, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(4, 8, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	SimulationManager::AddParticle(5, 10, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	SimulationManager::AddParticle(6, 12, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(7, 14, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(8, 16, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f);
	SimulationManager::AddParticle(9, 18, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f);
	SimulationManager::AddParticle(10, 20, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);

	// The content within AppWindow does NOT get created in the AppWindow constructor. This is to
	// allow the AppWindowTemplate the ability to create itself and give us a window, however,
	// the window contents need access to DeviceResources, so you MUST initialize DeviceResources 
	// in between constructing AppWindow and calling AppWindow->Initialize(). The same holds true
	// for initializing ImGui as well
	m_window->Initialize();
}

int App::Run() const
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = m_window->ProcessMessages())
		{
			// Shutdown ImGui
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		// Update the active simulation before updating the window
		SimulationManager::Update();

		// Update the window-side data and render
		m_window->Update();
		if (m_window->Render())
			m_window->Present();
	}
}