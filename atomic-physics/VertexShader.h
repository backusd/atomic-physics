#pragma once
#include "pch.h"
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Microsoft::WRL::ComPtr<ID3DBlob> blob) noexcept;
	VertexShader(const VertexShader&) = delete;
	void operator=(const VertexShader&) = delete;

	void Bind() const noexcept_release_only override;

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};