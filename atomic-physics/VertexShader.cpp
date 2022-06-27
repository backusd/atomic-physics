#include "VertexShader.h"

VertexShader::VertexShader(Microsoft::WRL::ComPtr<ID3DBlob> blob) :
	Bindable()
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateVertexShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			m_vertexShader.ReleaseAndGetAddressOf()
		)
	);
}

void VertexShader::Bind() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0u)
	);
}