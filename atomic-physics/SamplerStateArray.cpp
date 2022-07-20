#include "SamplerStateArray.h"

SamplerStateArray::SamplerStateArray(SamplerStateBindingLocation bindToStage) noexcept:
	Bindable()
{
	PROFILE_FUNCTION();

	switch (bindToStage)
	{
	case SamplerStateBindingLocation::COMPUTE_SHADER:	BindFunc = [this]() { this->BindCS(); }; break;
	case SamplerStateBindingLocation::VERTEX_SHADER:	BindFunc = [this]() { this->BindVS(); }; break;
	case SamplerStateBindingLocation::HULL_SHADER:		BindFunc = [this]() { this->BindHS(); }; break;
	case SamplerStateBindingLocation::DOMAIN_SHADER:	BindFunc = [this]() { this->BindDS(); }; break;
	case SamplerStateBindingLocation::GEOMETRY_SHADER:	BindFunc = [this]() { this->BindGS(); }; break;
	case SamplerStateBindingLocation::PIXEL_SHADER:		BindFunc = [this]() { this->BindPS(); }; break;
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

void SamplerStateArray::Bind() const noexcept
{
	PROFILE_FUNCTION();

	BindFunc();
}

void SamplerStateArray::BindCS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->CSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindVS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->VSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindHS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->HSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindDS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindGS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->GSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}

void SamplerStateArray::BindPS() const noexcept
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetSamplers(0u, static_cast<unsigned int>(m_rawSamplerStatePointers.size()), m_rawSamplerStatePointers.data())
	);
}