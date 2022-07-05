#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Mouse.h"

class MoveLookController
{
public:
	MoveLookController(D3D11_VIEWPORT vp) noexcept;
	MoveLookController(const MoveLookController&) = delete;
	void operator=(const MoveLookController&) = delete;

	void Update(D3D11_VIEWPORT viewport) noexcept;

	DirectX::XMMATRIX ViewMatrix() const noexcept;
	DirectX::XMMATRIX ProjectionMatrix() const noexcept;

	DirectX::XMVECTOR Position() const noexcept { return m_eye; }

	void OnLPress(Mouse::Event e) noexcept;
	void OnLRelease(Mouse::Event e) noexcept;
	void OnLDoubleClick(Mouse::Event e) noexcept;
	void OnRPress(Mouse::Event e) noexcept;
	void OnRRelease(Mouse::Event e) noexcept;
	void OnMPress(Mouse::Event e) noexcept;
	void OnMRelease(Mouse::Event e) noexcept;
	void OnWheelUp(Mouse::Event e) noexcept;
	void OnWheelDown(Mouse::Event e) noexcept;
	void OnMouseMove(Mouse::Event e) noexcept;

private:
	void CreateProjectionMatrix(D3D11_VIEWPORT vp) noexcept;

	void RotateLeftRight(float theta);
	void RotateUpDown(float theta);

	// Eye/at/up vectors
	DirectX::XMVECTOR m_eye;
	DirectX::XMVECTOR m_at;
	DirectX::XMVECTOR m_up;

	DirectX::XMMATRIX m_projectionMatrix;

	// Pointer Variables
	bool  m_mouseDown;
	float m_mousePositionX;
	float m_mousePositionY;
	float m_mousePositionXNew;
	float m_mousePositionYNew;
};