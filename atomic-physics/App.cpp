#include "App.h"

App::App()
{
	// Create the window
	m_window = std::make_unique<AppWindow>(1000, 800, "Main Window");

	// Initialize the device resources
	DeviceResources::Initialize(m_window->GetHWND());

	// Initialize the Simulation Manager
	SimulationManager::Initialize();

	// The content within AppWindow does NOT get created in the AppWindow constructor. This is to
	// allow the AppWindowTemplate the ability to create itself and give us a window, however,
	// the window contents need access to DeviceResources, so you MUST initialize DeviceResources 
	// in between constructing AppWindow and calling AppWindow->Initialize()
	m_window->Initialize();
}

int App::Run() const
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = m_window->ProcessMessages())
		{
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