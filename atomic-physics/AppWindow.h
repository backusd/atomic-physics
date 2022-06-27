#pragma once
#include "pch.h"
#include "AppWindowTemplate.h"
#include "Renderer.h"

#include <optional>
#include <memory>

class AppWindow : public AppWindowTemplate<AppWindow>
{
public:
	AppWindow(int width, int height, const char* name) noexcept;
	AppWindow(AppWindow&) = delete;
	void operator=(const AppWindow&) = delete;


	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept override;

	void Update() noexcept;
	bool Render() const noexcept;
	void Present() const;

	std::optional<int> ProcessMessages() const noexcept;

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
	LRESULT OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

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


	std::unique_ptr<Renderer> m_renderer;
};