#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Mouse.h"
#include "SimulationManager.h"

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

	void InitializeAutomatedMove(double maxMoveTime) noexcept;
	void RotateLeftRight(float theta) noexcept;
	void RotateUpDown(float theta) noexcept;

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

	// Automated move variables
	//		When zooming in/out, or rotating 90, etc., we simply need to set the target location and
	//		Update() will move the camera for us
	bool	m_movingToNewLocation;
	DirectX::XMFLOAT3 m_eyeTarget;
	DirectX::XMFLOAT3 m_eyeInitial;
	DirectX::XMFLOAT3 m_upTarget;
	DirectX::XMFLOAT3 m_upInitial;
	double m_moveStartTime;
	double m_movementComplete;
	double m_movementMaxTime;
	//double m_timeAtLastMoveUpdate;
	//float  m_totalRotationAngle;
	//bool m_rotatingLeftRight;
	//bool m_rotatingUpDown;
};