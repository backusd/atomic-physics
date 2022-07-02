#pragma once
#include "pch.h"
#include "BasicGeometry.h"
#include "Bindable.h"
#include "InputLayoutException.h"

#include <vector>
#include <string>

class InputLayout : public Bindable
{
public:
	InputLayout(std::wstring vertexShaderFile);
	InputLayout(std::wstring vertexShaderFile, BasicGeometry geometry);
	InputLayout(const InputLayout&) = delete;
	void operator=(const InputLayout&) = delete;

	void AddDescription(std::string semanticName,
						unsigned int semanticIndex,
						DXGI_FORMAT format,
						unsigned int inputSlot,
						unsigned int alignedByteOffset,
						D3D11_INPUT_CLASSIFICATION inputSlotClass,
						unsigned int instanceDataStepRate) noexcept;

	void CreateLayout();

	void Bind() const noexcept_release_only override;

	Microsoft::WRL::ComPtr<ID3DBlob> GetVertexShaderFileBlob() noexcept { return m_blob; }

private:
	void CreateBoxInputLayout();
	void CreateSphereInputLayout();

	std::vector<std::string> m_semanticNames;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_descriptions;

	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};