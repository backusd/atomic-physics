#pragma once
#include "pch.h"
#include "Mesh.h"
#include "HLSLStructures.h"

class BoxMesh : public Mesh
{
public:
	BoxMesh();
	BoxMesh(const BoxMesh&) = delete;
	void operator=(const BoxMesh&) = delete;


private:
	void InitializeBuffers();

};