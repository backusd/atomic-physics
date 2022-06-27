#include "SamplerStateArray.h"

SamplerStateArray::SamplerStateArray(SamplerStateBindingLocation bindToStage) noexcept:
	Bindable()
{
	switch (bindToStage)
	{
	case SamplerStateBindingLocation::COMPUTE_SHADER:	BindFunc = std::bind(&SamplerStateArray::BindCS, this); break;
	case SamplerStateBindingLocation::VERTEX_SHADER:	BindFunc = std::bind(&SamplerStateArray::BindVS, this); break;
	case SamplerStateBindingLocation::HULL_SHADER:		BindFunc = std::bind(&SamplerStateArray::BindHS, this); break;
	case SamplerStateBindingLocation::DOMAIN_SHADER:	BindFunc = std::bind(&SamplerStateArray::BindDS, this); break;
	case SamplerStateBindingLocation::GEOMETRY_SHADER:	BindFunc = std::bind(&SamplerStateArray::BindGS, this); break;
	case SamplerStateBindingLocation::PIXEL_SHADER:		BindFunc = std::bind(&SamplerStateArray::BindPS, this); break;
	}
}

void SamplerStateArray::AddSamplerState(std::shared_ptr<SamplerState> samplerState) noexcept
{
	m_samplerStates.push_back(samplerState);
	m_rawSamplerStatePointers.push_back(m_samplerStates.back()->GetRawPointer());
}
void SamplerStateArray::ClearSamplerStates() noexcept
{
	m_samplerStates.clear();
	m_rawSamplerStatePointers.clear();
}

void SamplerStateArray::Bind() const noexcept_release_only
{
	BindFunc();
}

void SamplerStateArray::BindCS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->CSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindVS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->VSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindHS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->HSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindDS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindGS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->GSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindPS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}