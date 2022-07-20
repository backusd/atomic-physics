#pragma once
#include "pch.h"
#include "Renderer.h"

#include <memory>

// ImGui ------------------------
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "implot.h"
// ------------------------------

class UI
{
public:
	UI() noexcept;
	UI(const UI&) = delete;
	void operator=(const UI&) = delete;

	void Render(const std::unique_ptr<Renderer>& renderer) noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }

private:
	void CreateDockSpaceAndMenuBar() noexcept;
	void MenuBar() noexcept;
	void SimulationDetailsWindow() noexcept;
	void LogWindow() noexcept;


	void PerformanceWindow() noexcept;
	void PerformanceFPS() noexcept;
	void PerformanceProfile() noexcept;

	void SceneEditWindow(const std::unique_ptr<Renderer>& renderer) noexcept;
	void SceneLighting(const std::unique_ptr<Renderer>& renderer) noexcept;

	ImGuiIO& m_io;
	D3D11_VIEWPORT m_viewport;

	float m_top, m_left;
	float m_height, m_width;
	float m_windowOffsetX, m_windowOffsetY;
};