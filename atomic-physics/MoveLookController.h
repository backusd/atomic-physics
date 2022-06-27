#pragma once
#include "pch.h"
#include "DeviceResources.h"



class MoveLookController
{
public:
	MoveLookController(D3D11_VIEWPORT vp) noexcept;
	MoveLookController(const MoveLookController&) = delete;
	void operator=(const MoveLookController&) = delete;

	DirectX::XMMATRIX ViewMatrix() const noexcept;
	DirectX::XMMATRIX ProjectionMatrix() const noexcept;

private:
	void CreateProjectionMatrix(D3D11_VIEWPORT vp) noexcept;


	// Eye/at/up vectors
	DirectX::XMVECTOR m_eye;
	DirectX::XMVECTOR m_at;
	DirectX::XMVECTOR m_up;

	DirectX::XMMATRIX m_projectionMatrix;
};