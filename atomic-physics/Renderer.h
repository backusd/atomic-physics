#pragma once
#include "pch.h"
#include "Box.h"
#include "DeviceResources.h"
#include "Drawable.h"
#include "EyePositionBufferArray.h"
#include "Lighting.h"
#include "MaterialBufferArray.h"
#include "Mouse.h"
#include "MoveLookController.h"
#include "Sphere.h"
#include "SimulationManager.h"

#include <memory>
#include <vector>

class Renderer
{
public:
	Renderer(D3D11_VIEWPORT vp) noexcept;
	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;

	void Update();
	void Render();

	void ProcessMouseEvents() noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }

	// Mouse Events
	void OnLPress(Mouse::Event e) const noexcept;
	void OnLRelease(Mouse::Event e) const noexcept;
	void OnLDoubleClick(Mouse::Event e) const noexcept;
	void OnRPress(Mouse::Event e) const noexcept;
	void OnRRelease(Mouse::Event e) const noexcept;
	void OnMPress(Mouse::Event e) const noexcept;
	void OnMRelease(Mouse::Event e) const noexcept;
	void OnWheelUp(Mouse::Event e) const noexcept;
	void OnWheelDown(Mouse::Event e) const noexcept;
	void OnMouseMove(Mouse::Event e) const noexcept;
	void OnMouseEnter(Mouse::Event e) const noexcept;
	void OnMouseLeave(Mouse::Event e) const noexcept;
	void OnMouseInvalidEvent(Mouse::Event e) const noexcept;

private:


	D3D11_VIEWPORT m_viewport;
	std::shared_ptr<MoveLookController> m_moveLookController;
	std::vector<std::unique_ptr<Drawable>> m_drawables;
	std::unique_ptr<Box> m_box;
	
	// Pixel Shader constant buffer arrays - set ONCE per frame
	std::unique_ptr<Lighting> m_lighting;
	std::unique_ptr<EyePositionBufferArray> m_eyePositionBufferArray;
	std::unique_ptr<MaterialBufferArray> m_materialBufferArray;

};