#pragma once
#include "pch.h"
#include "Box.h"
#include "DeviceResources.h"
#include "Drawable.h"
#include "EyePositionBufferArray.h"
#include "Keyboard.h"
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
	void ProcessKeyboardEvents() noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }
	void SetViewport(D3D11_VIEWPORT viewport) noexcept;

	// Mouse Events
	void OnLPress(const Mouse::Event& e) const noexcept;
	void OnLRelease(const Mouse::Event& e) const noexcept;
	void OnLDoubleClick(const Mouse::Event& e) const noexcept;
	void OnRPress(const Mouse::Event& e) const noexcept;
	void OnRRelease(const Mouse::Event& e) const noexcept;
	void OnMPress(const Mouse::Event& e) const noexcept;
	void OnMRelease(const Mouse::Event& e) const noexcept;
	void OnWheelUp(const Mouse::Event& e) const noexcept;
	void OnWheelDown(const Mouse::Event& e) const noexcept;
	void OnMouseMove(const Mouse::Event& e) const noexcept;
	void OnMouseEnter(const Mouse::Event& e) const noexcept;
	void OnMouseLeave(const Mouse::Event& e) const noexcept;
	void OnMouseInvalidEvent(const Mouse::Event& e) const noexcept;

	// Keyboard Events
	void OnUpArrowKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnDownArrowKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnLeftArrowKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnRightArrowKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnCtrlKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnShiftKeyEvent(const Keyboard::Event& e) const noexcept;
	void OnCharEvent(char c) const noexcept;

	// Other Events
	void OnResize(D3D11_VIEWPORT vp) noexcept;

private:
	void Render_AllSpheres() const noexcept_release_only;
	void Render_Generic() const noexcept_release_only;

	void InitializeAllSphereData() noexcept;
	void UpdateAllSphereModelViewProjectionInstanceData() const;
	void UpdateAllSphereMaterialIndexInstanceData() const;

	D3D11_VIEWPORT m_viewport;
	std::shared_ptr<MoveLookController> m_moveLookController;
	std::vector<std::unique_ptr<Drawable>> m_drawables;
	std::unique_ptr<Box> m_box;
	
	// Pixel Shader constant buffer arrays - set ONCE per frame
	std::unique_ptr<Lighting> m_lighting;
	std::unique_ptr<EyePositionBufferArray> m_eyePositionBufferArray;
	std::unique_ptr<MaterialBufferArray> m_materialBufferArray;

	// Render resources - ALL SPHERES
	std::unique_ptr<InputLayout>		 m_allSphere_InputLayout;
	std::unique_ptr<VertexShader>		 m_allSphere_VertexShader;
	std::unique_ptr<PixelShader>		 m_allSphere_PixelShader;
	std::unique_ptr<SphereMesh>			 m_allSphere_Mesh;
	std::unique_ptr<RasterizerState>	 m_allSphere_RasterizerState;
	std::unique_ptr<DepthStencilState>	 m_allSphere_DepthStencilState;
	std::unique_ptr<ConstantBufferArray> m_allSphere_ModelViewProjectionInstanceBufferArray;
	std::unique_ptr<ConstantBufferArray> m_allSphere_MaterialIndexInstanceBufferArray;

};