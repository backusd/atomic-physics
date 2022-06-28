#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Drawable.h"
#include "MoveLookController.h"
#include "Sphere.h"

#include <memory>
#include <vector>

class Renderer
{
public:
	Renderer(D3D11_VIEWPORT vp) noexcept;

	void Update() noexcept;
	void Render() noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }


private:


	D3D11_VIEWPORT m_viewport;
	std::shared_ptr<MoveLookController> m_moveLookController;
	std::vector<std::unique_ptr<Drawable>> m_drawables;


	/* 
	MeshBase --> SphereMesh
	ShaderBase --> SolidShaders + PhongShaders (Light + Material Properties)
	ConstantBuffer
	RasterState
	Textures
	DepthStencilState




	...AND...
	MoveLookController (one per Renderer...configurable as well)
	*/
};