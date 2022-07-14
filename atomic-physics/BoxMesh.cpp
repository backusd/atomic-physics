#include "BoxMesh.h"

using DirectX::XMFLOAT4;

BoxMesh::BoxMesh() :
	Mesh()
{
	PROFILE_FUNCTION();

	m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	InitializeBuffers();
}

void BoxMesh::InitializeBuffers()
{
	PROFILE_FUNCTION();

	// Create a unit cube - will be scaled up to size of simulation during rendering
	float x = 1.0f;
	float y = 1.0f;
	float z = 1.0f;

	XMFLOAT4 normal = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	std::vector<PositionNormalVertex> vertices(8); // box vertices
	vertices[0] = { { x,  y,  z, 1.0f}, normal };
	vertices[1] = { {-x,  y,  z, 1.0f}, normal };
	vertices[2] = { { x, -y,  z, 1.0f}, normal };
	vertices[3] = { { x,  y, -z, 1.0f}, normal };
	vertices[4] = { {-x, -y,  z, 1.0f}, normal };
	vertices[5] = { {-x,  y, -z, 1.0f}, normal };
	vertices[6] = { { x, -y, -z, 1.0f}, normal };
	vertices[7] = { {-x, -y, -z, 1.0f}, normal };

	std::vector<unsigned short> indices(24);

	// draw the square with all positive x
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);

	// draw the square with all negative x
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(1);

	// draw the four lines that connect positive x with negative x
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(4);

	LoadBuffers(vertices, indices);
}