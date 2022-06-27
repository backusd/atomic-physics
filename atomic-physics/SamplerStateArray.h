#pragma once
#include "pch.h"
#include "Bindable.h"
#include "SamplerState.h"

#include <vector>
#include <functional>

enum class SamplerStateBindingLocation
{
	COMPUTE_SHADER = 0,
	VERTEX_SHADER = 1,
	HULL_SHADER = 2,
	DOMAIN_SHADER = 3,
	GEOMETRY_SHADER = 4,
	PIXEL_SHADER = 5
};

class SamplerStateArray : public Bindable
{
public:
	SamplerStateArray(SamplerStateBindingLocation bindToStage) noexcept;
	SamplerStateArray(const SamplerStateArray&) = delete;
	void operator=(const SamplerStateArray&) = delete;

	void AddSamplerState(std::shared_ptr<SamplerState> samplerState) noexcept;
	void ClearSamplerStates() noexcept;

	void Bind() const noexcept_release_only override;

	ID3D11SamplerState* GetRawPointer(int index) const noexcept { return m_samplerStates[index]->GetRawPointer(); }


private:
	std::function<void()> BindFunc;

	void BindCS() const noexcept_release_only;
	void BindVS() const noexcept_release_only;
	void BindHS() const noexcept_release_only;
	void BindDS() const noexcept_release_only;
	void BindGS() const noexcept_release_only;
	void BindPS() const noexcept_release_only;

	std::vector<ID3D11SamplerState*> m_rawSamplerStatePointers;
	std::vector<std::shared_ptr<SamplerState>> m_samplerStates;
};