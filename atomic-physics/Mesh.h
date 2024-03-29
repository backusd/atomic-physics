#pragma once
#include "pch.h"
#include "Bindable.h"

class Mesh : public Bindable
{
public:
	Mesh() noexcept;
	Mesh(const Mesh&) = delete;
	void operator=(const Mesh&) = delete;

	void Bind() const noexcept override;
	unsigned int IndexCount() const noexcept { return m_indexCount; }
	unsigned int VertexCount() const noexcept { return m_vertexCount; }

	template <typename T, typename A>
	void LoadBuffers(std::vector<T, A>& vertices, std::vector<unsigned short>& indices) noexcept;

protected:


	D3D11_PRIMITIVE_TOPOLOGY m_topology;
	DXGI_FORMAT m_indexFormat;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	unsigned int m_indexCount;
	unsigned int m_vertexCount;
	unsigned int m_sizeOfVertex;
};

template <typename T, typename A>
void Mesh::LoadBuffers(std::vector<T, A>& vertices, std::vector<unsigned short>& indices) noexcept
{
	m_sizeOfVertex = sizeof(T);
	m_vertexCount = static_cast<unsigned int>(vertices.size());

	// Vertex Buffer
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = static_cast<UINT>(vertices.size() * m_sizeOfVertex);
	bd.StructureByteStride = m_sizeOfVertex;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	GFX_THROW_INFO(DeviceResources::D3DDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer));

	// Index Buffer
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = static_cast<UINT>(indices.size() * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO(DeviceResources::D3DDevice()->CreateBuffer(&ibd, &isd, &m_indexBuffer));

	m_indexCount = static_cast<unsigned int>(indices.size());
}