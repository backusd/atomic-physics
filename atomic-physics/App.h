#pragma once
#include "pch.h"
#include "AppWindow.h"
#include "DeviceResources.h"
#include "SimulationManager.h"

// ImGui ------------------------
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>
// ------------------------------

#include <memory>

class App
{
public:
	App() noexcept;
	App(const App&) = delete;
	void operator=(const App&) = delete;

	int Run() const;

private:
	std::unique_ptr<AppWindow> m_window;
};