#pragma once
#include "pch.h"
#include "Mesh.h"
#include "HLSLStructures.h"

class SphereMesh : public Mesh
{
public:
	SphereMesh();
	SphereMesh(const SphereMesh&) = delete;
	void operator=(const SphereMesh&) = delete;


private:
	void InitializeBuffers();

};