#include "DeviceResources.h"
#include "MacroHelper.h"
#include "WindowException.h"

using Microsoft::WRL::ComPtr;

DeviceResources::DeviceResources() noexcept :
	m_hWnd(0),
	m_d2dFactory(nullptr),
	m_d2dDevice(nullptr),
	m_d2dDeviceContext(nullptr),
	m_d2dBitmap(nullptr),
	m_dwriteFactory(nullptr),
	m_wicImagingFactory(nullptr),
	m_d3dDevice(nullptr),
	m_d3dDeviceContext(nullptr),
	m_dxgiSwapChain(nullptr),
	m_d3dRenderTargetView(nullptr),
	m_d3dDepthStencilView(nullptr),
	m_viewport(CD3D11_VIEWPORT(0.0f, 0.0f, 0.0f, 0.0f)),
	m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
	m_dpiScale(96.0f)
{
}

void DeviceResources::InitializeImpl(HWND hWnd) noexcept
{
	PROFILE_FUNCTION();

	m_hWnd = hWnd;

	// Must initialize COM library
	GFX_THROW_INFO(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

	CreateDeviceIndependentResources();
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void DeviceResources::OnResizeImpl()
{
	PROFILE_FUNCTION();

	CreateWindowSizeDependentResources();
}

void DeviceResources::CreateDeviceIndependentResources() noexcept
{
	PROFILE_FUNCTION();

	// Initialize Direct2D Resources
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in debug, enable Direct2D debugging
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	GFX_THROW_INFO(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory7),
			&options,
			(void**)m_d2dFactory.ReleaseAndGetAddressOf()
		)
	);

	GFX_THROW_INFO(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory7),
			reinterpret_cast<IUnknown**>(m_dwriteFactory.ReleaseAndGetAddressOf())
		)
	);

	GFX_THROW_INFO(
		CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(m_wicImagingFactory.ReleaseAndGetAddressOf())
		)
	);
}

void DeviceResources::CreateDeviceDependentResources() noexcept
{
	PROFILE_FUNCTION();

	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		// If the project is in a debug build, enable debugging via SDK Layers with this flag
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// This array defines the set of DirectX hardware feature levels this app will support
	// Note: the ordering should be preserved
	// Don't forget to declare your application's minimum required feature level in its
	// description. All applications are assumed to support 9.1 unless otherwise stated
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	// Throw a DeviceResourceException if creating the device fails
	GFX_THROW_INFO(D3D11CreateDevice(
		nullptr,					// nullptr to use default adapter
		D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver
		0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE
		creationFlags,				// Set debug and Direct2D compatibility flags
		featureLevels,				// List of feature levels this app can support
		ARRAYSIZE(featureLevels),	// Size of feature levels list
		D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps
		device.ReleaseAndGetAddressOf(),		// Address of pointer to Direct3D device being created
		&m_d3dFeatureLevel,			// Address of feature level of device that was created
		context.ReleaseAndGetAddressOf()		// Address of pointer to device context being created
	));

	/* Don't try on a WARP device as I'm guessing it will be too slow for our purposes

	if (FAILED(hr))
	{
		// If the initialization fails, fall back to the WARP device
		// For more information on WARP, see:
		// http://go.microsoft.com/fwlink/?LinkId=286690
		ThrowIfFailed(
			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_WARP,		// Create a WARP device instead of hardware device
				0,
				creationFlags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				device.ReleaseAndGetAddressOf(),
				&m_d3dFeatureLevel,
				context.ReleaseAndGetAddressOf()
			)
		);
	}
	*/

	// Store pointers to the Direct3D 11.3 API device and immediate context
	GFX_THROW_INFO(device.As(&m_d3dDevice));

	GFX_THROW_INFO(context.As(&m_d3dDeviceContext));

	// Create the Direct2D device object and a corresponding context
	ComPtr<IDXGIDevice4> dxgiDevice;
	GFX_THROW_INFO(m_d3dDevice.As(&dxgiDevice));
	GFX_THROW_INFO(
		m_d2dFactory->CreateDevice(
			dxgiDevice.Get(),
			m_d2dDevice.ReleaseAndGetAddressOf()
		)
	);
	GFX_THROW_INFO(
		m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			m_d2dDeviceContext.ReleaseAndGetAddressOf()
		)
	);
}

void DeviceResources::CreateWindowSizeDependentResources() noexcept
{
	PROFILE_FUNCTION();

	// Get height, width, and dpi for the window
	RECT rect;
	if (GetClientRect(m_hWnd, &rect) == 0)
	{
		TERMINATE_ON_THROW(throw WINDOW_LAST_EXCEPT());
	}

	float height = static_cast<float>(rect.bottom);
	float width = static_cast<float>(rect.right);

	float dpi = 0.0f;
	unsigned int _dpi = GetDpiForWindow(m_hWnd);
	if (_dpi == 0)
	{
		TERMINATE_ON_THROW(
			throw std::exception("GetDpiForWindow returned 0\nFILE: " STRINGIFY(__FILE__) "\nLINE: " STRINGIFY(__LINE__))
		)
	}
	m_dpiScale = dpi / 96.0f;

	// Clear the previous window size specific context
	m_d3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr); // These are the appropriate parameters to clear the render targets
	m_d3dRenderTargetView = nullptr;
	m_d2dDeviceContext->SetTarget(nullptr);
	m_d2dBitmap = nullptr;
	m_d3dDepthStencilView = nullptr;
	m_d3dDeviceContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

	// if the swap chain already exists, resize it
	if (m_dxgiSwapChain.Get() != nullptr)
	{
		HRESULT hRESULT = m_dxgiSwapChain->ResizeBuffers(
			2, // Double-buffered swap chain
			lround(width),
			lround(height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
		);

		if (hRESULT == DXGI_ERROR_DEVICE_REMOVED || hRESULT == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created
			HandleDeviceLost();

			// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method
			// and correctly set up the new device
			return;
		}
		else
		{
			GFX_THROW_INFO(hRESULT);
		}
	}
	else
	{
		//DXGI_SCALING scaling = DO I Support High Resolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
		DXGI_SCALING scaling = DXGI_SCALING_NONE;

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = static_cast<UINT>(width);					// Match the size of the window
		swapChainDesc.Height = static_cast<UINT>(height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// This is the most common swap chain format
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;									// Use double-buffering to minimize latency
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All Windows Store apps must use this SwapEffect
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = scaling;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// This sequence obtains the DXGI factory that was used to create the Direct3D device above
		ComPtr<IDXGIDevice4> dxgiDevice;
		GFX_THROW_INFO(m_d3dDevice.As(&dxgiDevice));

		ComPtr<IDXGIAdapter> dxgiAdapter;
		GFX_THROW_INFO(
			dxgiDevice->GetAdapter(dxgiAdapter.ReleaseAndGetAddressOf())
		);

		ComPtr<IDXGIFactory5> dxgiFactory;
		GFX_THROW_INFO(
			dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()))
		);


		// THIS NEEDS VERIFICATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		ComPtr<IDXGISwapChain1> swapChain;
		GFX_THROW_INFO(
			dxgiFactory->CreateSwapChainForHwnd(
				m_d3dDevice.Get(),
				m_hWnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				swapChain.ReleaseAndGetAddressOf()
			)
		);
		GFX_THROW_INFO(swapChain.As(&m_dxgiSwapChain));

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		// ensures that the application will only render after each Vsync, minimizing power consumption
		GFX_THROW_INFO(dxgiDevice->SetMaximumFrameLatency(1));
	}

	GFX_THROW_INFO(
		m_dxgiSwapChain->SetRotation(DXGI_MODE_ROTATION_IDENTITY)
	);

	// Create a render target view of the swap chain back buffer
	ComPtr<ID3D11Texture2D1> backBuffer;
	GFX_THROW_INFO(
		m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.ReleaseAndGetAddressOf()))
	);

	GFX_THROW_INFO(
		m_d3dDevice->CreateRenderTargetView1(
			backBuffer.Get(),
			nullptr,
			m_d3dRenderTargetView.ReleaseAndGetAddressOf()
		)
	);

	// Create a depth stencil view for use with 3D rendering if needed
	CD3D11_TEXTURE2D_DESC1 depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT, // reserve 24 bits for the depth value and 8 bits for stencil value (used for outline effect)
		static_cast<UINT>(width),
		static_cast<UINT>(height),
		1, // This depth stencil view has only one texture
		1, // Use a single mipmap level
		D3D11_BIND_DEPTH_STENCIL
	);

	ComPtr<ID3D11Texture2D1> depthStencil;
	GFX_THROW_INFO(
		m_d3dDevice->CreateTexture2D1(
			&depthStencilDesc,
			nullptr,
			depthStencil.ReleaseAndGetAddressOf()
		)
	);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);
	GFX_THROW_INFO(
		m_d3dDevice->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			m_d3dDepthStencilView.ReleaseAndGetAddressOf()
		)
	);

	/*
	// Set the rasterizer state
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID; // or D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_NONE;
	rd.FrontCounterClockwise = true;	// This must be true for the outline effect to work properly
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;

	// Create and set the solid raster state
	GFX_THROW_INFO(
		m_d3dDevice->CreateRasterizerState(&rd, solidRasterState.ReleaseAndGetAddressOf())
	);
	m_d3dDeviceContext->RSSetState(solidRasterState.Get());

	// Also create the wireframe raster state
	rd.FillMode = D3D11_FILL_WIREFRAME;
	GFX_THROW_INFO(
		m_d3dDevice->CreateRasterizerState(&rd, wireframeRasterState.ReleaseAndGetAddressOf())
	);
	*/

	// Create a Direct2D target bitmap associated with the
	// swap cahin back buffer and set it as the current target
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi,
			dpi
		);

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	GFX_THROW_INFO(
		m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(dxgiBackBuffer.ReleaseAndGetAddressOf()))
	);

	GFX_THROW_INFO(
		m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
			dxgiBackBuffer.Get(),
			&bitmapProperties,
			m_d2dBitmap.ReleaseAndGetAddressOf()
		)
	);

	m_d2dDeviceContext->SetTarget(m_d2dBitmap.Get());

	m_d2dDeviceContext->SetDpi(dpi, dpi);

	// Grayscale text anti-aliasing is recommended for all Windows Store apps
	m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// Recreate all device resources and set them back to the current state
void DeviceResources::HandleDeviceLost() noexcept
{
	m_dxgiSwapChain = nullptr;
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void DeviceResources::SetViewportImpl(D3D11_VIEWPORT viewport) noexcept
{
	m_viewport = viewport;
	GFX_THROW_INFO_ONLY(
		m_d3dDeviceContext->RSSetViewports(1, &m_viewport)
	)
}

void DeviceResources::PresentImpl() noexcept
{
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	HRESULT hRESULT = m_dxgiSwapChain->Present1(1, 0, &parameters);

	GFX_THROW_INFO_ONLY(
		m_d3dDeviceContext->DiscardView1(m_d3dRenderTargetView.Get(), nullptr, 0)
	)
	GFX_THROW_INFO_ONLY(
		m_d3dDeviceContext->DiscardView1(m_d3dDepthStencilView.Get(), nullptr, 0)
	)

	if (hRESULT == DXGI_ERROR_DEVICE_REMOVED || hRESULT == DXGI_ERROR_DEVICE_RESET)
	{
		HandleDeviceLost();
	}
	else
	{
		// Throw if hr is a failed result and try to get debug info
		GFX_THROW_INFO(hRESULT);
	}
}