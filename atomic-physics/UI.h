#pragma once
#include "pch.h"

// ImGui ------------------------
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
// ------------------------------

class UI
{
public:
	UI() noexcept;
	UI(const UI&) = delete;
	void operator=(const UI&) = delete;

	void Render() noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }

private:
	void CreateDockSpaceAndMenuBar() noexcept;
	void MenuBar() noexcept;
	void SimulationDetails() noexcept;
	void LogWindow() noexcept;
	void PerformanceWindow() noexcept;
	void SimulationViewport() noexcept;


	ImGuiIO& m_io;
	D3D11_VIEWPORT m_viewport;

	float m_leftScreenSpace;
	float m_topScreenSpace;
	float m_rightScreenSpace;
	float m_bottomScreenSpace;

	float m_leftWindowSpace;
	float m_topWindowSpace;
	float m_rightWindowSpace;
	float m_bottomWindowSpace;
};