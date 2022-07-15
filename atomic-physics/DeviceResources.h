#pragma once
#include "pch.h"
#include "DeviceResourcesException.h"
#include "DirectXHelper.h"

class DeviceResources
{
public:
	DeviceResources(const DeviceResources&) = delete;
	void operator=(const DeviceResources&) = delete;


	static void Initialize(HWND hWnd) { return Get().InitializeImpl(hWnd); }
	static void SetViewport(D3D11_VIEWPORT viewport) noexcept { Get().SetViewportImpl(viewport); }
	static void Present() { Get().PresentImpl(); }

	static D3D11_VIEWPORT GetViewport() noexcept { return Get().m_viewport; }
	static ID3D11Device5* D3DDevice() noexcept { return Get().m_d3dDevice.Get(); }
	static ID3D11DeviceContext4* D3DDeviceContext() noexcept { return Get().m_d3dDeviceContext.Get(); }
	static ID3D11DepthStencilView* DepthStencilView() noexcept { return Get().m_d3dDepthStencilView.Get(); }
	static ID3D11RenderTargetView1* BackBufferRenderTargetView() noexcept { return Get().m_d3dRenderTargetView.Get(); }

	static void OnResize() { Get().OnResizeImpl(); }

#if defined(_DEBUG)
public:
	static DxgiInfoManager& GetInfoManager() noexcept { return Get().m_infoManager; }
private:
	DxgiInfoManager m_infoManager;
#endif

private:
	DeviceResources() noexcept;

	static DeviceResources& Get() noexcept
	{
		static DeviceResources deviceResources;
		return deviceResources;
	}

	void InitializeImpl(HWND hWnd);
	void CreateDeviceDependentResources();
	void CreateDeviceIndependentResources() noexcept;
	void CreateWindowSizeDependentResources();
	void HandleDeviceLost();
	void SetViewportImpl(D3D11_VIEWPORT viewport) noexcept;
	void PresentImpl();

	void OnResizeImpl();


	// Keep handle to window
	HWND m_hWnd;

	// DPI scale for the window
	float m_dpiScale;

	// ==========================================================

	// Direct 2D drawing components
	Microsoft::WRL::ComPtr<ID2D1Factory7>		m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device6>		m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext6>	m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dBitmap;

	// Direct Write drawing components
	Microsoft::WRL::ComPtr<IDWriteFactory7>		m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicImagingFactory;

	// Direct3D objects
	Microsoft::WRL::ComPtr<ID3D11Device5>		 m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>		 m_dxgiSwapChain;

	// Direct3D Rendering objects ------ THESE MAY END UP GETTING STORED PER WINDOW
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>	m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;

	// Keep one rasterstate for solid fill and another for wireframe
	// Microsoft::WRL::ComPtr<ID3D11RasterizerState> solidRasterState;
	// Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeRasterState;

	D3D11_VIEWPORT m_viewport;

	// Cached device properties
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;

	// Orientation transforms
	// D2D1::Matrix3x2F	m_orientationTransform2D;
	// DirectX::XMFLOAT4X4 m_orientationTransform3D;

};