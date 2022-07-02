#include "InputLayout.h"

InputLayout::InputLayout(std::wstring vertexShaderFile) :
	Bindable()
{
	GFX_THROW_INFO(
		D3DReadFileToBlob(vertexShaderFile.c_str(), &m_blob)
	);
}
InputLayout::InputLayout(std::wstring vertexShaderFile, BasicGeometry geometry) :
	Bindable()
{
	GFX_THROW_INFO(
		D3DReadFileToBlob(vertexShaderFile.c_str(), &m_blob)
	);

	switch (geometry)
	{
	case BasicGeometry::BOX:	CreateBoxInputLayout(); break;
	case BasicGeometry::SPHERE: CreateSphereInputLayout(); break;
	}
}

void InputLayout::CreateBoxInputLayout()
{
	AddDescription("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddDescription(  "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateLayout();
}

void InputLayout::CreateSphereInputLayout()
{
	AddDescription("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddDescription(  "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateLayout();
}

void InputLayout::AddDescription(
	std::string semanticName,
	unsigned int semanticIndex,
	DXGI_FORMAT format,
	unsigned int inputSlot,
	unsigned int alignedByteOffset,
	D3D11_INPUT_CLASSIFICATION inputSlotClass,
	unsigned int instanceDataStepRate) noexcept
{
	// Add the sematic name to a vector so they don't go out of scope prior to calling CreateLayout
	m_semanticNames.push_back(semanticName);

	D3D11_INPUT_ELEMENT_DESC desc;
	desc.SemanticName = ""; // semantic name will be set in CreateLayout		//m_semanticNames[m_semanticNames.size() - 1].c_str();
	desc.SemanticIndex = semanticIndex;
	desc.Format = format;
	desc.InputSlot = inputSlot;
	desc.AlignedByteOffset = alignedByteOffset;
	desc.InputSlotClass = inputSlotClass;
	desc.InstanceDataStepRate = instanceDataStepRate;

	m_descriptions.push_back(desc);
}

void InputLayout::CreateLayout()
{
	// Set the semantic names here so the c_str doesn't go out of scope
	for (unsigned int iii = 0; iii < m_semanticNames.size(); ++iii)
		m_descriptions[iii].SemanticName = m_semanticNames[iii].c_str();

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateInputLayout(
			m_descriptions.data(),
			static_cast<UINT>(m_descriptions.size()),
			m_blob->GetBufferPointer(),
			m_blob->GetBufferSize(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	// Once the layout is created, we can get rid of the semantic names
	m_semanticNames.clear();
}

void InputLayout::Bind() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->IASetInputLayout(m_inputLayout.Get())
	);
}