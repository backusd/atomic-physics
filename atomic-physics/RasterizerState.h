#pragma once
#include "pch.h"
#include "Bindable.h"

class RasterizerState : public Bindable
{
public:
	RasterizerState() noexcept;
	RasterizerState(const RasterizerState&) = delete;
	void operator=(const RasterizerState&) = delete;

	void FillMode(D3D11_FILL_MODE mode) { m_desc.FillMode = mode; LoadChanges(); }
	void CullMode(D3D11_CULL_MODE mode) { m_desc.CullMode = mode; LoadChanges(); }
	void FrontCounterClockwise(bool frontCounterClockwise) { m_desc.FrontCounterClockwise = frontCounterClockwise; LoadChanges(); }
	void DepthBias(int bias) { m_desc.DepthBias = bias; LoadChanges(); }
	void SlopeScaledDepthBias(float bias) { m_desc.SlopeScaledDepthBias = bias; LoadChanges(); }
	void DepthBiasClamp(float bias) { m_desc.DepthBiasClamp = bias; LoadChanges(); }
	void DepthClipEnable(bool enable) { m_desc.DepthClipEnable = enable; LoadChanges(); }
	void ScissorEnable(bool enable) { m_desc.ScissorEnable = enable; LoadChanges(); }
	void MultisampleEnable(bool enable) { m_desc.MultisampleEnable = enable; LoadChanges(); }
	void AntialiasedLineEnable(bool enable) { m_desc.AntialiasedLineEnable = enable; LoadChanges(); }

	void Bind() const noexcept_release_only override;

	void ResetState() noexcept;

private:
	void LoadChanges() noexcept;

	D3D11_RASTERIZER_DESC m_desc;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
};