#pragma once
#include "pch.h"
#include "DeviceResources.h"

class ConstantBuffer
{
public:
	ConstantBuffer() noexcept {}
	ConstantBuffer(const ConstantBuffer&) = delete;
	void operator=(const ConstantBuffer&) = delete;

	template <typename T>
	void CreateBuffer(D3D11_USAGE usage, unsigned int cpuAccessFlags, unsigned int miscFlags, unsigned int structuredByteStride) noexcept;
	template <typename T>
	void CreateBuffer(D3D11_USAGE usage, unsigned int cpuAccessFlags, unsigned int miscFlags, unsigned int structuredByteStride, void* initialData) noexcept;

	ID3D11Buffer* GetRawBufferPointer() const noexcept { return m_buffer.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};

template <typename T>
void ConstantBuffer::CreateBuffer(D3D11_USAGE usage, unsigned int cpuAccessFlags, unsigned int miscFlags, unsigned int structuredByteStride) noexcept
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(T);
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = structuredByteStride;

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateBuffer(
			&desc,								// Use the description we just created
			nullptr,							// Don't fill it with any data
			m_buffer.ReleaseAndGetAddressOf()	// Assign result to buffer
		)
	);
}

template <typename T>
void ConstantBuffer::CreateBuffer(D3D11_USAGE usage, unsigned int cpuAccessFlags, unsigned int miscFlags, unsigned int structuredByteStride, void* initialData) noexcept
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(T);
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = structuredByteStride;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initialData;
	data.SysMemPitch = 0;		// Only relevant for 2D/3D textures
	data.SysMemSlicePitch = 0;	// Only relevant for 2D/3D textures

	GFX_THROW_INFO(
		DeviceResources::D3DDevice()->CreateBuffer(
			&desc,								// Use the description we just created
			&data,								// Fill the buffer with the passed in data
			m_buffer.ReleaseAndGetAddressOf()	// Assign result to buffer
		)
	);
}