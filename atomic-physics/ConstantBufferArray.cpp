#include "ConstantBufferArray.h"

ConstantBufferArray::ConstantBufferArray(ConstantBufferBindingLocation bindToStage) noexcept :
	Bindable()
{
	switch (bindToStage)
	{
	case ConstantBufferBindingLocation::COMPUTE_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindCS, this); break;
	case ConstantBufferBindingLocation::VERTEX_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindVS, this); break;
	case ConstantBufferBindingLocation::HULL_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindHS, this); break;
	case ConstantBufferBindingLocation::DOMAIN_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindDS, this); break;
	case ConstantBufferBindingLocation::GEOMETRY_SHADER: BindFunc = std::bind(&ConstantBufferArray::BindGS, this); break;
	case ConstantBufferBindingLocation::PIXEL_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindPS, this); break;
	}
}

ConstantBufferArray::ConstantBufferArray(ConstantBufferBindingLocation bindToStage, BasicGeometry geometry) :
	Bindable()
{
	switch (bindToStage)
	{
	case ConstantBufferBindingLocation::COMPUTE_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindCS, this); break;
	case ConstantBufferBindingLocation::VERTEX_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindVS, this); break;
	case ConstantBufferBindingLocation::HULL_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindHS, this); break;
	case ConstantBufferBindingLocation::DOMAIN_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindDS, this); break;
	case ConstantBufferBindingLocation::GEOMETRY_SHADER: BindFunc = std::bind(&ConstantBufferArray::BindGS, this); break;
	case ConstantBufferBindingLocation::PIXEL_SHADER:	 BindFunc = std::bind(&ConstantBufferArray::BindPS, this); break;
	}

	switch (geometry)
	{
	case BasicGeometry::SPHERE: CreateSphereConstantBufferArray(); break;
	}
}

void ConstantBufferArray::CreateSphereConstantBufferArray() noexcept
{
	std::shared_ptr<ConstantBuffer> mvpBuffer = std::make_shared<ConstantBuffer>();
	mvpBuffer->CreateBuffer<ModelViewProjectionPreMultiplied>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);

	AddBuffer(mvpBuffer);
}

void ConstantBufferArray::AddBuffer(std::shared_ptr<ConstantBuffer> buffer) noexcept
{
	m_buffers.push_back(buffer);
	m_rawBufferPointers.push_back(m_buffers.back()->GetRawBufferPointer());
}
void ConstantBufferArray::ClearBuffers() noexcept
{
	m_buffers.clear();
	m_rawBufferPointers.clear();
}

void ConstantBufferArray::Bind() const noexcept_release_only
{
	BindFunc();
}

void ConstantBufferArray::BindCS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->CSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::BindVS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->VSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::BindHS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->HSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::BindDS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->DSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::BindGS() const noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->GSSetConstantBuffers(0u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::BindPS() const noexcept_release_only
{
	// Default PS Buffers:
	//		slot 0 - Lighting data
	//		slot 1 - User/eye position data
	//		slot 2 - All Phong Materials
	//
	// Therefore, you MUST start at slot 3
	//
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetConstantBuffers(3u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void ConstantBufferArray::UpdateSubresource(int index, void* data) noexcept_release_only
{
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->UpdateSubresource(m_buffers[index]->GetRawBufferPointer(), 0, nullptr, data, 0, 0)
	);
}