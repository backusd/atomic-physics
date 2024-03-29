#include "SamplerState.h"

SamplerState::SamplerState()
{
	PROFILE_FUNCTION();

	ResetState();
	LoadChanges();
}

void SamplerState::ResetState() noexcept
{
	m_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_desc.MipLODBias = 0.0f;
	m_desc.MaxAnisotropy = 1;
	m_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	m_desc.BorderColor[0] = 0;
	m_desc.BorderColor[1] = 0;
	m_desc.BorderColor[2] = 0;
	m_desc.BorderColor[3] = 0;
	m_desc.MinLOD = 0;
	m_desc.MaxLOD = D3D11_FLOAT32_MAX;
}

void SamplerState::LoadChanges()
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateSamplerState(&m_desc, m_samplerState.ReleaseAndGetAddressOf())
	);
}

