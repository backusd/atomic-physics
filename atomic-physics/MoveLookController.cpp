#include "MoveLookController.h"

using DirectX::XMVECTOR;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;

MoveLookController::MoveLookController(D3D11_VIEWPORT vp) noexcept :
	m_eye{ 0.0f, 0.0f, 1.0f },
	m_at{ 0.0f, 0.0f, 0.0f },
	m_up{ 0.0f, 1.0f, 0.0f }
{
	CreateProjectionMatrix(vp);
}

XMMATRIX MoveLookController::ViewMatrix() const noexcept
{
	return DirectX::XMMatrixLookAtRH(m_eye, m_at, m_up);
}

XMMATRIX MoveLookController::ProjectionMatrix() const noexcept
{
    return m_projectionMatrix;
}

void MoveLookController::CreateProjectionMatrix(D3D11_VIEWPORT vp) noexcept
{
    // Perspective Matrix
    float aspectRatio = vp.Width / vp.Height;
    float fovAngleY = DirectX::XM_PI / 4;

    // This is a simple example of a change that can be made when the app is in portrait or snapped view
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // Note that the OrientationTransform3D matrix is post-multiplied here
    // in order to correctly orient the scene to match the display orientation.
    // This post-multiplication step is required for any draw calls that are
    // made to the swap chain render target. For draw calls to other targets,
    // this transform should not be applied.

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.01f,
        1000.0f
    );

    //XMFLOAT4X4 orientation = m_deviceResources->OrientationTransform3D();
    //XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
    XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity();

    // Projection Matrix (No Transpose)
    m_projectionMatrix = perspectiveMatrix * orientationMatrix;
}