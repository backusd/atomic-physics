#include "AppWindow.h"
#include "WindowsMessageMap.h"


AppWindow::AppWindow(int width, int height, const char* name) noexcept :
	AppWindowTemplate(width, height, name)
{
	m_renderer = std::make_unique<Renderer>();

	// Create keyboard/mouse classes for processing inuput
	//... Unless they become singletons in which case there is no need for explicit initialization
}

void AppWindow::Update() noexcept
{
	// At this point for the current frame, the Simulation has already been updated
	// All we need to do on the window side is query the Simulation to render it
	m_renderer->Update();

	// Update data for ImGui ... ???
	//
	// ... ??
}

bool AppWindow::Render() const noexcept
{ 
	// Draw 3D scene
	m_renderer->Render();

	// Draw ImGui controls


	return true; 
}

void AppWindow::Present() const
{
	DeviceResources::Present();
}








// Forward declare message handler from imgui_impl_win32.cpp
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT AppWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	//static WindowsMessageMap mm;
	//OutputDebugString(mm(msg, wParam, lParam).c_str());	

	//if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	//	return true;

	switch (msg)
	{
	case WM_CREATE:			return OnCreate(hWnd, msg, wParam, lParam);
	case WM_CLOSE:			return OnClose(hWnd, msg, wParam, lParam);
	case WM_DESTROY:		return OnDestroy(hWnd, msg, wParam, lParam);
	case WM_LBUTTONDOWN:	return OnLButtonDown(hWnd, msg, wParam, lParam);
	case WM_LBUTTONUP:		return OnLButtonUp(hWnd, msg, wParam, lParam);
	case WM_LBUTTONDBLCLK:	return OnLButtonDoubleClick(hWnd, msg, wParam, lParam);
	case WM_MBUTTONDOWN:	return OnMButtonDown(hWnd, msg, wParam, lParam);
	case WM_MBUTTONUP:		return OnMButtonUp(hWnd, msg, wParam, lParam);
	case WM_RBUTTONDOWN:	return OnRButtonDown(hWnd, msg, wParam, lParam);
	case WM_RBUTTONUP:		return OnRButtonUp(hWnd, msg, wParam, lParam);
	case WM_PAINT:			return OnPaint(hWnd, msg, wParam, lParam);
	case WM_SIZE:			return OnResize(hWnd, msg, wParam, lParam);
	case WM_MOUSEMOVE:		return OnMouseMove(hWnd, msg, wParam, lParam);
	case WM_MOUSELEAVE:		return OnMouseLeave(hWnd, msg, wParam, lParam);
	case WM_MOUSEWHEEL:		return OnMouseWheel(hWnd, msg, wParam, lParam);
	case WM_GETMINMAXINFO:	return OnGetMinMaxInfo(hWnd, msg, wParam, lParam);
	case WM_CHAR:			return OnChar(hWnd, msg, wParam, lParam);
	case WM_SYSKEYUP:		return OnSysKeyUp(hWnd, msg, wParam, lParam);
	case WM_KEYUP:			return OnKeyUp(hWnd, msg, wParam, lParam);
	case WM_SYSKEYDOWN:		return OnSysKeyDown(hWnd, msg, wParam, lParam);
	case WM_KEYDOWN:		return OnKeyDown(hWnd, msg, wParam, lParam);
	case WM_KILLFOCUS:		return OnKillFocus(hWnd, msg, wParam, lParam);
	case WM_DPICHANGED:
		/*
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		*/
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> AppWindow::ProcessMessages() const noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return static_cast<int>(msg.wParam);
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);

		LRESULT result = DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

LRESULT AppWindow::OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	// we don't want the DefProc to handle this message because
	// we want our destructor to destroy the window, so return 0 instead of break

	// Don't post quit - Just set the destroy window flag
	// WindowManager will worry about posting WM_QUIT
	// m_destroy = true;

	/* Perform any clean up work here */

	PostQuitMessage(0);

	return 0;
}

LRESULT AppWindow::OnDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}