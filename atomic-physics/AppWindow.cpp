#include "AppWindow.h"
#include "WindowsMessageMap.h"


AppWindow::AppWindow(int width, int height, const char* name) noexcept :
	AppWindowTemplate(width, height, name),
	m_height(height),
	m_width(width),
	m_clearColor{ 55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f },
	m_io(ImGui::GetIO()), // ImGui::GetIO uses IM_ASSERT, so no need to try-catch here
	m_viewport()
{
	PROFILE_FUNCTION();

	m_ui = std::make_unique<UI>();

	// DONT actually do any initialization here. Just allow the AppWindowTemplate to create itself
	// To build the contents within the AppWindow, call Initialize(). HOWEVER, the contents will
	// need access to DeviceResources, so you MUST initialize DeviceResources in between constructing
	// AppWindow and calling AppWindow->Initialize()
	// 
	// Also wait to initialize ImGui because we need access to DeviceResources for that as well
}

void AppWindow::Initialize() noexcept
{
	PROFILE_FUNCTION();

	// Once window space is allotted, create the viewport then the renderer
	CD3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));
	m_simulationRenderer = std::make_unique<Renderer>(vp);

	// Initialize ImGui
	InitializeImGui();
}

void AppWindow::InitializeImGui() noexcept
{
	PROFILE_FUNCTION();

	(void)m_io;
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	{
		PROFILE_SCOPE("ImGui_ImplWin32_Init");
		TERMINATE_ON_THROW(ImGui_ImplWin32_Init(m_hWnd))
	}
	{
		PROFILE_SCOPE("ImGui_ImplDX11_Init");
		TERMINATE_ON_THROW(ImGui_ImplDX11_Init(DeviceResources::D3DDevice(), DeviceResources::D3DDeviceContext()))
	}



	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
}

void AppWindow::Update() noexcept
{
	PROFILE_FUNCTION();

	// At this point for the current frame, the Simulation has already been updated
	// All we need to do on the window side is query the Simulation to render it
	m_simulationRenderer->Update();

	// Ideally, the window should keep some logic about whether or not the mouse is
	// over the simulation viewport and whether or not it needs to have the renderer
	// process mouse events, or if some other control needs to process the events. For
	// right now, we only have the renderer, so it can just process all mouse events
	m_simulationRenderer->ProcessMouseEvents();

	// Keyboard events only occur when the window is in focus, but if we have more than
	// just a single custom control, AppWindow will need to keep track of which control
	// should be the one to process keyboard events
	m_simulationRenderer->ProcessKeyboardEvents();


	// Update data for ImGui ... ???
	//
	// ... ??
}

bool AppWindow::Render() noexcept
{ 
	PROFILE_FUNCTION();

	// Clear the window
	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	{
		PROFILE_SCOPE("Clear DSV/RTV");
		
		context->ClearDepthStencilView(DeviceResources::DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->ClearRenderTargetView(DeviceResources::BackBufferRenderTargetView(), m_clearColor);
	}

	{
		PROFILE_SCOPE("Set Render Target");
		ID3D11RenderTargetView* const targets[1] = { DeviceResources::BackBufferRenderTargetView() };
		context->OMSetRenderTargets(1, targets, DeviceResources::DepthStencilView());
	}

	// Start the Dear ImGui frame
	{
		PROFILE_SCOPE("ImGui_ImplDX11_NewFrame");
		TERMINATE_ON_THROW(ImGui_ImplDX11_NewFrame())
	}
	{
		PROFILE_SCOPE("ImGui_ImplWin32_NewFrame");
		TERMINATE_ON_THROW(ImGui_ImplWin32_NewFrame())
	}
	{
		PROFILE_SCOPE("ImGui::NewFrame()");
		TERMINATE_ON_THROW(ImGui::NewFrame())
	}

	// Render the UI
	m_ui->Render();

	// Render 3D scene - MUST render here because we render on top of ImGui, but if we render
	//					 after finalizing the ImGui draw, then Present will be called by ImGui
	//					 which will discard the render target (I believe)
	m_simulationRenderer->SetViewport(m_ui->GetViewport());
	m_simulationRenderer->Render();

	// Render ImGui
	{
		PROFILE_SCOPE("ImGui::Render()");
		TERMINATE_ON_THROW(ImGui::Render())
	}



	{
		PROFILE_SCOPE("ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData())");
		TERMINATE_ON_THROW(ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()))
	}

	// Update and Render additional Platform Windows
	if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		TERMINATE_ON_THROW(ImGui::UpdatePlatformWindows())
		TERMINATE_ON_THROW(ImGui::RenderPlatformWindowsDefault())
	}

	m_viewport = m_ui->GetViewport();

	return true; 
}

void AppWindow::Present() const noexcept
{
	DeviceResources::Present();
}








// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT AppWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//static WindowsMessageMap mm;
	//OutputDebugString(mm(msg, wParam, lParam).c_str());	

	// Allow ImGui to process the message first
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

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
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}		
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

		// Can optionally obtain the LRESULT value that is returned, but from the Microsoft docs:
		// "The return value specifies the value returned by the window procedure. Although its 
		// meaning depends on the message being dispatched, the return value generally is ignored."
		// LRESULT result = DispatchMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

LRESULT AppWindow::OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnClose(HWND /* hWnd */, UINT /* msg */, WPARAM /* wParam */, LPARAM /* lParam */) const noexcept
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
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnLeftPressed(pt.x, pt.y);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			Mouse::OnMouseLeave();
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnLeftDoubleClick(pt.x, pt.y);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnMiddlePressed(pt.x, pt.y);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnMiddleReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			Mouse::OnMouseLeave();
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnRightPressed(pt.x, pt.y);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		Mouse::OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			Mouse::OnMouseLeave();
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_width = LOWORD(lParam);
	m_height = HIWORD(lParam);

	// For now, just create the viewport for the whole screen
	CD3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));

	DeviceResources::OnResize();
	m_simulationRenderer->OnResize(vp);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			Mouse::OnMouseMove(pt.x, pt.y);
			if (!Mouse::IsInWindow()) // IsInWindow() will tell you if it was PREVIOUSLY in the window or not
			{
				SetCapture(hWnd);
				Mouse::OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				Mouse::OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				Mouse::OnMouseLeave();
			}
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT AppWindow::OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		Mouse::OnWheelDelta(pt.x, pt.y, delta);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		Keyboard::OnChar(static_cast<unsigned char>(wParam));
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		Keyboard::OnKeyReleased(static_cast<unsigned char>(wParam));
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	if (!m_io.WantCaptureMouse)
	{
		if (!(lParam & 0x40000000) || Keyboard::AutorepeatIsEnabled()) // filter autorepeat
		{
			Keyboard::OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return this->OnKeyUp(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return this->OnKeyDown(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::OnKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	// clear keystate when window loses focus to prevent input getting "stuck"
	Keyboard::ClearState();

	return DefWindowProc(hWnd, msg, wParam, lParam);
}