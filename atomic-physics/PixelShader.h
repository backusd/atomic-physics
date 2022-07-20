#pragma once
#include "pch.h"
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(std::wstring pixelShaderFile) noexcept;
	PixelShader(const PixelShader&) = delete;
	void operator=(const PixelShader&) = delete;

	void Bind() const noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};