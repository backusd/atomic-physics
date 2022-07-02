#include "SphereMesh.h"

using DirectX::XMFLOAT4;

SphereMesh::SphereMesh() :
	Mesh()
{
	InitializeBuffers();
}

void SphereMesh::InitializeBuffers()
{
	unsigned short segments = 26;
	unsigned int slices = segments / 2;
	unsigned int numVertices = (slices + 1) * (segments + 1) + 1;
	unsigned int numIndices = slices * segments * 3 * 2;

	std::vector<PositionNormalVertex> sphereVertices(numVertices);

	// To make the texture look right on the top and bottom of the sphere
	// each slice will have 'segments + 1' vertices.  The top and bottom
	// vertices will all be coincident, but have different U texture cooordinates.
	unsigned int p = 0;
	XMFLOAT4 positionNormal; // We are creating a unit sphere so the position is the same as the normal
	for (unsigned int a = 0; a <= slices; a++)
	{
		float angle1 = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_PI;
		float z = static_cast<float>(cos(angle1));
		float r = static_cast<float>(sin(angle1));
		for (unsigned int b = 0; b <= segments; b++)
		{
			float angle2 = static_cast<float>(b) / static_cast<float>(segments) * DirectX::XM_2PI;

			// We are working with the unit sphere so the position and normal
			// vectors are the same
			positionNormal = XMFLOAT4(
				static_cast<float>(r * cos(angle2)),
				static_cast<float>(r * sin(angle2)),
				z, 
				1.0f
			);
			sphereVertices[p].position = positionNormal;
			sphereVertices[p].normal = positionNormal;

			p++;
		}
	}

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.

	std::vector<unsigned short> sphereIndices(numIndices);

	p = 0;
	for (unsigned short a = 0; a < slices; a++)
	{
		unsigned short p1 = a * (segments + 1);
		unsigned short p2 = (a + 1) * (segments + 1);

		// Generate two triangles for each segment around the slice.
		for (unsigned short b = 0; b < segments; b++)
		{
			if (a < (slices - 1))
			{
				// For all but the bottom slice add the triangle with one
				// vertex in the a slice and two vertices in the a + 1 slice.
				// Skip it for the bottom slice since the triangle would be
				// degenerate as all the vertices in the bottom slice are coincident.
				sphereIndices[p] = b + p1;
				sphereIndices[p + 1] = b + p2;
				sphereIndices[p + 2] = b + p2 + 1;
				p = p + 3;
			}
			if (a > 0)
			{
				// For all but the top slice add the triangle with two
				// vertices in the a slice and one vertex in the a + 1 slice.
				// Skip it for the top slice since the triangle would be
				// degenerate as all the vertices in the top slice are coincident.
				sphereIndices[p] = b + p1;
				sphereIndices[p + 1] = b + p2 + 1;
				sphereIndices[p + 2] = b + p1 + 1;
				p = p + 3;
			}
		}
	}

	LoadBuffers(sphereVertices, sphereIndices);
}