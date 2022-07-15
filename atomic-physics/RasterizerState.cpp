#include "RasterizerState.h"


RasterizerState::RasterizerState() noexcept :
	Bindable()
{
	PROFILE_FUNCTION();

	ResetState();
	LoadChanges();
}

void RasterizerState::ResetState() noexcept
{
	m_desc.FillMode = D3D11_FILL_SOLID;
	m_desc.CullMode = D3D11_CULL_NONE;
	m_desc.FrontCounterClockwise = true;
	m_desc.DepthBias = 0;
	m_desc.SlopeScaledDepthBias = 0.0f;
	m_desc.DepthBiasClamp = 0.0f;
	m_desc.DepthClipEnable = true;
	m_desc.ScissorEnable = false;
	m_desc.MultisampleEnable = false;
	m_desc.AntialiasedLineEnable = false;
}

void RasterizerState::LoadChanges() noexcept
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateRasterizerState(&m_desc, m_rasterizerState.ReleaseAndGetAddressOf())
	);
}

void RasterizerState::Bind() const noexcept_release_only
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->RSSetState(m_rasterizerState.Get())
	);
}