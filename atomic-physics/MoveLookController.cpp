#include "MoveLookController.h"

using DirectX::XMVECTOR;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;

using DirectX::operator*;
using DirectX::operator+;

MoveLookController::MoveLookController(D3D11_VIEWPORT vp) noexcept :
	m_eye{ 0.0f, 0.0f, 5.0f },
	m_at{ 0.0f, 0.0f, 0.0f },
	m_up{ 0.0f, 1.0f, 0.0f },
    m_mouseDown(false)
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

void MoveLookController::OnLPress(Mouse::Event e) noexcept
{
    // When the pointer is pressed begin tracking the pointer movement.
    m_mouseDown = true;
    m_mousePositionX = m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionY = m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::OnLRelease(Mouse::Event e) noexcept
{
    // Stop tracking pointer movement when the pointer is released.
    m_mouseDown = false;
    m_mousePositionX = m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionY = m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::OnLDoubleClick(Mouse::Event e) noexcept
{

}

void MoveLookController::OnRPress(Mouse::Event /* e */) noexcept
{

}

void MoveLookController::OnRRelease(Mouse::Event /* e */) noexcept
{

}

void MoveLookController::OnMPress(Mouse::Event /* e */) noexcept
{

}

void MoveLookController::OnMRelease(Mouse::Event /* e */) noexcept
{

}

void MoveLookController::OnWheelUp(Mouse::Event e) noexcept
{

}

void MoveLookController::OnWheelDown(Mouse::Event e) noexcept
{

}

void MoveLookController::OnMouseMove(Mouse::Event e) noexcept
{
    m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::Update(D3D11_VIEWPORT viewport) noexcept
{
    if (m_mouseDown)
    {
        // Cancel out any existing automated movement
        // m_movingToNewLocation = false;

        // Compute the eye distance to center
        float radius = 0.0f;
        DirectX::XMStoreFloat(&radius, DirectX::XMVector3Length(m_eye));

        // If the pointer were to move from the middle of the screen to the far right,
        // that should produce one full rotation. Therefore, set a rotationFactor = 2
        float rotationFactor = 2.0f;
        float radiansPerPixelX = (DirectX::XM_2PI / viewport.Width) * rotationFactor;
        float radiansPerPixelY = (DirectX::XM_2PI / viewport.Height) * rotationFactor;

        float thetaX = radiansPerPixelX * (m_mousePositionX - m_mousePositionXNew);
        float thetaY = radiansPerPixelY * (m_mousePositionYNew - m_mousePositionY);

        // Rotate
        RotateLeftRight(thetaX);
        RotateUpDown(thetaY);

        // reset the mouse position variables
        m_mousePositionX = m_mousePositionXNew;
        m_mousePositionY = m_mousePositionYNew;
    }
}

void MoveLookController::RotateLeftRight(float theta)
{
    // Use Rodrigue's Rotation Formula
        //     See here: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        //     v_rot : vector after rotation
        //     v     : vector before rotation
        //     theta : angle of rotation
        //     k     : unit vector representing axis of rotation
        //     v_rot = v*cos(theta) + (k x v)*sin(theta) + k*(k dot v)*(1-cos(theta))

    XMVECTOR v = m_eye;
    XMVECTOR k = m_up;
    m_eye = v * cos(theta) + DirectX::XMVector3Cross(k, v) * sin(theta) + k * DirectX::XMVector3Dot(k, v) * (1 - cos(theta));

    // Do NOT change the up-vector
}

void MoveLookController::RotateUpDown(float theta)
{
    // Use Rodrigue's Rotation Formula
    //     See here: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    //     v_rot : vector after rotation
    //     v     : vector before rotation
    //     theta : angle of rotation
    //     k     : unit vector representing axis of rotation
    //     v_rot = v*cos(theta) + (k x v)*sin(theta) + k*(k dot v)*(1-cos(theta))


    // The axis of rotation vector for up/down rotation will be the cross product 
    // between the eye-vector and the up-vector (must make it a unit vector)
    XMVECTOR v = m_eye;
    XMVECTOR k = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_eye, m_up));
    m_eye = v * cos(theta) + DirectX::XMVector3Cross(k, v) * sin(theta) + k * DirectX::XMVector3Dot(k, v) * (1 - cos(theta));

    // Now update the new up-vector should be the cross product between the k-vector and the new eye-vector
    m_up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(k, m_eye));
}