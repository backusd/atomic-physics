#pragma once
#include "pch.h"
#include "AppWindowTemplate.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Renderer.h"
#include "UI.h"

// ImGui ------------------------
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
// ------------------------------

#include <optional>
#include <memory>

class AppWindow : public AppWindowTemplate<AppWindow>
{
public:
	AppWindow(int width, int height, const char* name) noexcept;
	AppWindow(AppWindow&) = delete;
	void operator=(const AppWindow&) = delete;
	void Initialize() noexcept;


	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	void Update() noexcept;
	bool Render() noexcept;
	void Present() const noexcept;

	std::optional<int> ProcessMessages() const noexcept;

	D3D11_VIEWPORT GetSimulationViewport() const noexcept { return m_simulationRenderer->GetViewport(); }

private:
	// Virtual functions to handle all input 
	LRESULT OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	LRESULT OnLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	LRESULT OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	LRESULT OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	LRESULT OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;
	LRESULT OnSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	LRESULT OnKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	void InitializeImGui() noexcept;

	int m_height;
	int m_width;

	const float m_clearColor[4];
	std::unique_ptr<Renderer> m_simulationRenderer;

	// ImGui -------------------
	ImGuiIO& m_io;
	std::unique_ptr<UI> m_ui;
	D3D11_VIEWPORT m_viewport;
};