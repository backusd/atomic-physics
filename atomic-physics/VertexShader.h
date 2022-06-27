#pragma once
#include "pch.h"
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Microsoft::WRL::ComPtr<ID3DBlob> blob);

	void Bind() const noexcept_release_only override;

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};