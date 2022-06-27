#pragma once
#include "pch.h"
#include "AppWindow.h"
#include "DeviceResources.h"
#include "SimulationManager.h"

#include <memory>

class App
{
public:
	App();
	App(const App&) = delete;
	void operator=(const App&) = delete;

	int Run() const;

private:
	std::unique_ptr<AppWindow> m_window;
};