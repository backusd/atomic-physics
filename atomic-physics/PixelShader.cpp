#include "PixelShader.h"

PixelShader::PixelShader(std::wstring pixelShaderFile) noexcept :
	Bindable()
{
	PROFILE_FUNCTION();

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(pixelShaderFile.c_str(), &pBlob));
	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			m_pixelShader.ReleaseAndGetAddressOf()
		)
	);

}

void PixelShader::Bind() const noexcept_release_only
{
	PROFILE_FUNCTION();

	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0u)
	);
}