#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Drawable.h"
#include "EyePositionBufferArray.h"
#include "Lighting.h"
#include "MoveLookController.h"
#include "PhysicsConstants.h"
#include "Sphere.h"
#include "SimulationManager.h"

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
	std::unique_ptr<Lighting> m_lighting;
	std::unique_ptr<EyePositionBufferArray> m_eyePositionBufferArray;

};