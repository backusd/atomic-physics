#include "ConstantBufferArray.h"

ConstantBufferArray::ConstantBufferArray(ConstantBufferBindingLocation bindToStage) noexcept :
	Bindable()
{
	PROFILE_FUNCTION();

	switch (bindToStage)
	{
	case ConstantBufferBindingLocation::COMPUTE_SHADER:	 BindFunc = [this]() { this->BindCS(); }; break;
	case ConstantBufferBindingLocation::VERTEX_SHADER:	 BindFunc = [this]() { this->BindVS(); }; break;
	case ConstantBufferBindingLocation::HULL_SHADER:	 BindFunc = [this]() { this->BindHS(); }; break;
	case ConstantBufferBindingLocation::DOMAIN_SHADER:	 BindFunc = [this]() { this->BindDS(); }; break;
	case ConstantBufferBindingLocation::GEOMETRY_SHADER: BindFunc = [this]() { this->BindGS(); }; break;
	case ConstantBufferBindingLocation::PIXEL_SHADER:	 BindFunc = [this]() { this->BindPS(); }; break;
	}
}

ConstantBufferArray::ConstantBufferArray(ConstantBufferBindingLocation bindToStage, BasicGeometry geometry) noexcept :
	Bindable()
{
	PROFILE_FUNCTION();

	switch (bindToStage)
	{
	case ConstantBufferBindingLocation::COMPUTE_SHADER:	 BindFunc = [this]() { this->BindCS(); }; break;
	case ConstantBufferBindingLocation::VERTEX_SHADER:	 BindFunc = [this]() { this->BindVS(); }; break;
	case ConstantBufferBindingLocation::HULL_SHADER:	 BindFunc = [this]() { this->BindHS(); }; break;
	case ConstantBufferBindingLocation::DOMAIN_SHADER:	 BindFunc = [this]() { this->BindDS(); }; break;
	case ConstantBufferBindingLocation::GEOMETRY_SHADER: BindFunc = [this]() { this->BindGS(); }; break;
	case ConstantBufferBindingLocation::PIXEL_SHADER:	 BindFunc = [this]() { this->BindPS(); }; break;
	}

	switch (geometry)
	{
	case BasicGeometry::BOX:	CreateBoxConstantBufferArray(); break;
	case BasicGeometry::SPHERE: CreateSphereConstantBufferArray(); break;
	}
}

void ConstantBufferArray::CreateBoxConstantBufferArray() noexcept
{
	PROFILE_FUNCTION();

	// Model/view/projection buffer
	std::shared_ptr<ConstantBuffer> mvpBuffer = std::make_shared<ConstantBuffer>();
	mvpBuffer->CreateBuffer<ModelViewProjection>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);
	AddBuffer(mvpBuffer);

	// Color buffer (Set color to black)
	ColorConstantBuffer ccb = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	std::shared_ptr<ConstantBuffer> colorBuffer = std::make_shared<ConstantBuffer>();
	colorBuffer->CreateBuffer<ColorConstantBuffer>(D3D11_USAGE_DEFAULT, 0, 0, 0, static_cast<void*>(&ccb));
	AddBuffer(colorBuffer);
}

void ConstantBufferArray::CreateSphereConstantBufferArray() noexcept
{
	PROFILE_FUNCTION();

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
	PROFILE_FUNCTION();

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