#pragma once
#include "pch.h"
#include "WindowException.h"

template<typename T>
class AppWindowTemplate
{
public:
	AppWindowTemplate(int width, int height, const char* name);
	~AppWindowTemplate();

	virtual LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	HWND GetHWND() const noexcept { return m_hWnd; }


protected:
	static LRESULT CALLBACK HandleMsgSetupBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	// Window Class Data
	static constexpr const char* wndBaseClassName = "App Window";
	HINSTANCE m_hInst;

	// Window Data
	int m_width;
	int m_height;
	HWND m_hWnd;
};

template<typename T>
AppWindowTemplate<T>::AppWindowTemplate(int width, int height, const char* name) :
	m_height(height),
	m_width(width),
	m_hInst(GetModuleHandle(nullptr))
{
	// Register the window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = HandleMsgSetupBase;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wndBaseClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// calculate window size based on desired client region size
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;

	auto WS_options = WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX | WS_SIZEBOX;

	if (AdjustWindowRect(&rect, WS_options, FALSE) == 0)
	{
		throw WINDOW_LAST_EXCEPT();
	};

	// create window & get hWnd
	m_hWnd = CreateWindow(
		wndBaseClassName, 
		name,
		WS_options,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		rect.right - rect.left, 
		rect.bottom - rect.top,
		nullptr, 
		nullptr, 
		m_hInst, 
		this
	);

	if (m_hWnd == nullptr)
	{
		throw WINDOW_LAST_EXCEPT();
	}

	// show window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
};

template<typename T>
AppWindowTemplate<T>::~AppWindowTemplate()
{
	UnregisterClass(wndBaseClassName, m_hInst);
	DestroyWindow(m_hWnd);
};

template<typename T>
LRESULT CALLBACK AppWindowTemplate<T>::HandleMsgSetupBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

		AppWindowTemplate<T>* const pWnd = static_cast<AppWindowTemplate<T>*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&AppWindowTemplate<T>::HandleMsgBase));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

template<typename T>
LRESULT CALLBACK AppWindowTemplate<T>::HandleMsgBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	AppWindowTemplate<T>* const pWnd = reinterpret_cast<AppWindowTemplate<T>*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

