#include "DepthStencilState.h"


DepthStencilState::DepthStencilState(unsigned int stencilReferenceNumber) noexcept :
	Bindable(),
	m_stencilReferenceNumber(stencilReferenceNumber)
{
	PROFILE_FUNCTION();

	ResetState();
	LoadChanges();
}

void DepthStencilState::ResetState() noexcept
{
	PROFILE_FUNCTION();

	m_desc.DepthEnable		= true;
	m_desc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	m_desc.DepthFunc		= D3D11_COMPARISON_LESS;

	m_desc.StencilEnable	= true;
	m_desc.StencilReadMask	= 0xFF;
	m_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	m_desc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	m_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_desc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	m_desc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	m_desc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	m_desc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	m_desc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	m_desc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
}

void DepthStencilState::LoadChanges()
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateDepthStencilState(&m_desc, m_depthStencilState.ReleaseAndGetAddressOf())
	);
}

void DepthStencilState::Bind() const noexcept_release_only
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->OMSetDepthStencilState(m_depthStencilState.Get(), m_stencilReferenceNumber)
	);
}