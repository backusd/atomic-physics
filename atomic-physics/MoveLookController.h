#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Keyboard.h"
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
	void CreateProjectionMatrix(D3D11_VIEWPORT vp) noexcept;

	DirectX::XMVECTOR Position() const noexcept { return m_eye; }

	// Mouse Event Handling
	void OnLPress(const Mouse::Event& e) noexcept;
	void OnLRelease(const Mouse::Event& e) noexcept;
	void OnLDoubleClick(const Mouse::Event& e) noexcept;
	void OnRPress(const Mouse::Event& e) noexcept;
	void OnRRelease(const Mouse::Event& e) noexcept;
	void OnMPress(const Mouse::Event& e) noexcept;
	void OnMRelease(const Mouse::Event& e) noexcept;
	void OnWheelUp(const Mouse::Event& e) noexcept;
	void OnWheelDown(const Mouse::Event& e) noexcept;
	void OnMouseMove(const Mouse::Event& e) noexcept;

	// Keyboard Event Handling
	void OnUpArrowKeyEvent(const Keyboard::Event& e) noexcept;
	void OnDownArrowKeyEvent(const Keyboard::Event& e) noexcept;
	void OnLeftArrowKeyEvent(const Keyboard::Event& e) noexcept;
	void OnRightArrowKeyEvent(const Keyboard::Event& e) noexcept;
	void OnCtrlKeyEvent(const Keyboard::Event& e) noexcept;
	void OnShiftKeyEvent(const Keyboard::Event& e) noexcept;
	void OnCharEvent(char c) noexcept;

	// Other Events
	void OnResize(D3D11_VIEWPORT vp) noexcept;


private:
	void InitializeAutomatedMove(double maxMoveTime) noexcept;
	void RotateLeftRight(float theta) noexcept;
	void RotateUpDown(float theta) noexcept;

	void CenterOnFace() noexcept;
	void RotateLeft90() noexcept;
	void RotateRight90() noexcept;
	void RotateUp90() noexcept;
	void RotateDown90() noexcept;

	// Eye/at/up vectors
	DirectX::XMVECTOR m_eye;
	DirectX::XMVECTOR m_at;
	DirectX::XMVECTOR m_up;

	DirectX::XMMATRIX m_projectionMatrix;

	// Mouse Variables
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
	double m_timeAtLastMoveUpdate;
	float  m_totalRotationAngle;
	bool m_rotatingLeftRight;
	bool m_rotatingUpDown;

	// Input states for keyboard
	bool  m_leftArrow;
	bool  m_rightArrow;
	bool  m_upArrow;
	bool  m_downArrow;
	bool  m_shift;
	bool  m_ctrl;

	// Keep track of total time to be able to compute the time delta
	double m_elapsedTime;
};