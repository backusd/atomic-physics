#include "MoveLookController.h"

using DirectX::XMVECTOR;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using DirectX::XMFLOAT3;

using DirectX::operator*;
using DirectX::operator+;

MoveLookController::MoveLookController(D3D11_VIEWPORT vp) noexcept :
	m_eye{ 0.0f, 0.0f, 5.0f },
	m_at{ 0.0f, 0.0f, 0.0f },
	m_up{ 0.0f, 1.0f, 0.0f },
    m_mouseDown(false),
    m_mousePositionX(0.0f),
    m_mousePositionY(0.0f),
    m_mousePositionXNew(0.0f),
    m_mousePositionYNew(0.0f),
    m_movingToNewLocation(false),
    m_eyeTarget{ 0.0f, 0.0f, 0.0f },
    m_eyeInitial{ 0.0f, 0.0f, 0.0f },
    m_upTarget{ 0.0f, 0.0f, 0.0f },
    m_upInitial{ 0.0f, 0.0f, 0.0f },
    m_moveStartTime(0),
    m_movementComplete(0),
    m_movementMaxTime(0),
    m_timeAtLastMoveUpdate(0),
    m_totalRotationAngle(0.0f),
    m_rotatingLeftRight(false),
    m_rotatingUpDown(false),
    m_leftArrow(false),
    m_rightArrow(false),
    m_upArrow(false),
    m_downArrow(false),
    m_shift(false),
    m_ctrl(false),
    m_elapsedTime(0)
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

void MoveLookController::OnLPress(const Mouse::Event& e) noexcept
{
    // When the pointer is pressed begin tracking the pointer movement.
    m_mouseDown = true;
    m_mousePositionX = m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionY = m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::OnLRelease(const Mouse::Event& e) noexcept
{
    // Stop tracking pointer movement when the pointer is released.
    m_mouseDown = false;
    m_mousePositionX = m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionY = m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::OnLDoubleClick(const Mouse::Event& e) noexcept
{
    // Set automated move flags and initial data - 0.5 seconds for the move
    InitializeAutomatedMove(0.5);

    // Set eye target location to half the distance to the center
    m_eyeTarget.x = m_eyeInitial.x / 2.0f;
    m_eyeTarget.y = m_eyeInitial.y / 2.0f;
    m_eyeTarget.z = m_eyeInitial.z / 2.0f;

    m_upTarget = m_upInitial;
}

void MoveLookController::OnRPress(const Mouse::Event& /* e */) noexcept
{

}

void MoveLookController::OnRRelease(const Mouse::Event& /* e */) noexcept
{

}

void MoveLookController::OnMPress(const Mouse::Event& /* e */) noexcept
{

}

void MoveLookController::OnMRelease(const Mouse::Event& /* e */) noexcept
{

}

void MoveLookController::OnWheelUp(const Mouse::Event& e) noexcept
{
    // Only update if not already moving (this avoids a flood of WM_MOUSEWHEEL messages)
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.1 seconds for the move
        InitializeAutomatedMove(0.1);

        // Set eye target location to be 10% closer than the current location for every wheel up event
        float factor = 1.1f;
        m_eyeTarget.x = m_eyeInitial.x * factor;
        m_eyeTarget.y = m_eyeInitial.y * factor;
        m_eyeTarget.z = m_eyeInitial.z * factor;

        m_upTarget = m_upInitial;
    }
}

void MoveLookController::OnWheelDown(const Mouse::Event& e) noexcept
{
    // Only update if not already moving (this avoids a flood of WM_MOUSEWHEEL messages)
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.1 seconds for the move
        InitializeAutomatedMove(0.1);

        // Set eye target location to be 10% closer than the current location for every wheel up event
        float factor = 0.9f;
        m_eyeTarget.x = m_eyeInitial.x * factor;
        m_eyeTarget.y = m_eyeInitial.y * factor;
        m_eyeTarget.z = m_eyeInitial.z * factor;

        m_upTarget = m_upInitial;
    }
}

void MoveLookController::OnMouseMove(const Mouse::Event& e) noexcept
{
    m_mousePositionXNew = static_cast<float>(e.GetPosX());
    m_mousePositionYNew = static_cast<float>(e.GetPosY());
}

void MoveLookController::Update(D3D11_VIEWPORT viewport) noexcept
{
    if (m_mouseDown)
    {
        // Cancel out any existing automated movement
        m_movingToNewLocation = false;

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
    else if (m_movingToNewLocation)
    {
        // if the view matrix has officially been read by SceneRenderer, no need to perform any update here
        if (m_movementComplete)
        {
            m_movingToNewLocation = false;
            m_rotatingLeftRight = false;
            m_rotatingUpDown = false;
        }
        else
        {
            // If the move start time is less than 0, it needs to be set
            if (m_moveStartTime < 0.0)
            {
                m_moveStartTime = SimulationManager::TotalSeconds();
                m_timeAtLastMoveUpdate = m_moveStartTime;
            }

            // If rotating left/right, just compute the necessary angle and call RotateLeftRight / RotateUpDown
            if (m_rotatingLeftRight || m_rotatingUpDown)
            {
                double currentTime = SimulationManager::TotalSeconds();
                double timeDelta;
                if (m_moveStartTime + m_movementMaxTime < currentTime)
                {
                    m_movementComplete = true;
                    timeDelta = m_moveStartTime + m_movementMaxTime - m_timeAtLastMoveUpdate;
                }
                else
                    timeDelta = currentTime - m_timeAtLastMoveUpdate;

                float theta = m_totalRotationAngle * static_cast<float>(timeDelta / m_movementMaxTime);

                if (m_rotatingLeftRight)
                    RotateLeftRight(theta);
                else
                    RotateUpDown(theta);

                m_timeAtLastMoveUpdate = currentTime;
            }
            else
            {
                // Compute the ratio of elapsed time / allowed time to complete
                double timeRatio = (SimulationManager::TotalSeconds() - m_moveStartTime) / m_movementMaxTime;

                // if the current time is passed the max time, just assign final postion
                // Need to also set the updated view matrix has been read flag because SceneRenderer
                // will read the view matrix on the next Update call. Once that is done, we can set
                // movingToNewLocation to false (above)
                if (timeRatio >= 1.0)
                {
                    m_movementComplete = true;
                    m_eye = DirectX::XMLoadFloat3(&m_eyeTarget);
                    m_up = DirectX::XMLoadFloat3(&m_upTarget);
                }
                else
                {
                    // Compute the intermediate position
                    XMFLOAT3 eyeCurrent;
                    eyeCurrent.x = m_eyeInitial.x + static_cast<float>((static_cast<double>(m_eyeTarget.x) - m_eyeInitial.x) * timeRatio);
                    eyeCurrent.y = m_eyeInitial.y + static_cast<float>((static_cast<double>(m_eyeTarget.y) - m_eyeInitial.y) * timeRatio);
                    eyeCurrent.z = m_eyeInitial.z + static_cast<float>((static_cast<double>(m_eyeTarget.z) - m_eyeInitial.z) * timeRatio);

                    m_eye = DirectX::XMLoadFloat3(&eyeCurrent);

                    // Compute the intermediate position
                    XMFLOAT3 upCurrent;
                    upCurrent.x = m_upInitial.x + static_cast<float>((static_cast<double>(m_upTarget.x) - m_upInitial.x) * timeRatio);
                    upCurrent.y = m_upInitial.y + static_cast<float>((static_cast<double>(m_upTarget.y) - m_upInitial.y) * timeRatio);
                    upCurrent.z = m_upInitial.z + static_cast<float>((static_cast<double>(m_upTarget.z) - m_upInitial.z) * timeRatio);

                    m_up = DirectX::XMLoadFloat3(&upCurrent);
                }
            }
        }
    }
}

void MoveLookController::RotateLeftRight(float theta) noexcept
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

void MoveLookController::RotateUpDown(float theta) noexcept
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

void MoveLookController::InitializeAutomatedMove(double maxMoveTime) noexcept
{
    // Set m_movingToNewLocation = true so the renderer knows to update the view matrix
    m_movingToNewLocation = true;

    // Set the move completed flag to false
    m_movementComplete = false;

    // Reset the start time to -1 to signal it needs to be set in the next Update
    m_moveStartTime = -1.0;

    // Set the movement max time to 0.1 seconds, so the zoom completes in that time
    m_movementMaxTime = maxMoveTime;

    DirectX::XMStoreFloat3(&m_eyeInitial, m_eye);
    DirectX::XMStoreFloat3(&m_upInitial, m_up);
}

void MoveLookController::OnUpArrowKeyEvent(const Keyboard::Event& e) noexcept
{
    m_upArrow = e.IsPress();

    // If no longer moving or rotating, reset the time to 0
    if (!(m_upArrow || m_downArrow || m_leftArrow || m_rightArrow))
        m_elapsedTime = 0.0f;
}

void MoveLookController::OnDownArrowKeyEvent(const Keyboard::Event& e) noexcept
{
    m_downArrow = e.IsPress();

    // If no longer moving or rotating, reset the time to 0
    if (!(m_upArrow || m_downArrow || m_leftArrow || m_rightArrow))
        m_elapsedTime = 0.0f;
}

void MoveLookController::OnLeftArrowKeyEvent(const Keyboard::Event& e) noexcept
{
    m_leftArrow = e.IsPress();

    // If no longer moving or rotating, reset the time to 0
    if (!(m_upArrow || m_downArrow || m_leftArrow || m_rightArrow))
        m_elapsedTime = 0.0f;
}

void MoveLookController::OnRightArrowKeyEvent(const Keyboard::Event& e) noexcept
{
    m_rightArrow = e.IsPress();

    // If no longer moving or rotating, reset the time to 0
    if (!(m_upArrow || m_downArrow || m_leftArrow || m_rightArrow))
        m_elapsedTime = 0.0f;
}

void MoveLookController::OnCtrlKeyEvent(const Keyboard::Event& e) noexcept
{
    m_ctrl = e.IsPress();
}

void MoveLookController::OnShiftKeyEvent(const Keyboard::Event& e) noexcept
{
    m_shift = e.IsPress();
}

void MoveLookController::OnCharEvent(char c) noexcept
{
    switch (c)
    {
    // case 'p': SimulationManager::SwitchPlayPause(); break;
    case 'c': CenterOnFace(); break;
    case 'w': RotateUp90(); break;
    case 'a': RotateLeft90(); break;
    case 's': RotateDown90(); break;
    case 'd': RotateRight90(); break;
    }
}

void MoveLookController::CenterOnFace() noexcept
{
    // Set automated move flags and initial data - 0.5 seconds for the move
    InitializeAutomatedMove(0.5);

    // Determine the coordinate with the max value and 0 out the other ones
    m_eyeTarget.x = m_eyeInitial.x;
    m_eyeTarget.y = m_eyeInitial.y;
    m_eyeTarget.z = m_eyeInitial.z;

    XMFLOAT3 length3;
    DirectX::XMStoreFloat3(&length3, DirectX::XMVector3Length(m_eye));
    float length = length3.x;

    m_eyeTarget.x = (std::abs(m_eyeInitial.x) < std::abs(m_eyeInitial.y) || std::abs(m_eyeInitial.x) < std::abs(m_eyeInitial.z)) ? 0.0f : length;
    m_eyeTarget.y = (std::abs(m_eyeInitial.y) < std::abs(m_eyeInitial.x) || std::abs(m_eyeInitial.y) < std::abs(m_eyeInitial.z)) ? 0.0f : length;
    m_eyeTarget.z = (std::abs(m_eyeInitial.z) < std::abs(m_eyeInitial.x) || std::abs(m_eyeInitial.z) < std::abs(m_eyeInitial.y)) ? 0.0f : length;

    m_eyeTarget.x *= (m_eyeInitial.x < 0.0f) ? -1.0f : 1.0f;
    m_eyeTarget.y *= (m_eyeInitial.y < 0.0f) ? -1.0f : 1.0f;
    m_eyeTarget.z *= (m_eyeInitial.z < 0.0f) ? -1.0f : 1.0f;



    // Determine the coordinate with the max value and 0 out the other ones
    // Whichever coordinate for the eye target is used must not be used for the up target, so zero it out
    float xInit = (m_eyeTarget.x == 0.0f) ? m_upInitial.x : 0.0f;
    float yInit = (m_eyeTarget.y == 0.0f) ? m_upInitial.y : 0.0f;
    float zInit = (m_eyeTarget.z == 0.0f) ? m_upInitial.z : 0.0f;

    DirectX::XMStoreFloat3(&length3, DirectX::XMVector3Length(m_up));
    length = length3.x;

    m_upTarget.x = (std::abs(xInit) < std::abs(yInit) || std::abs(xInit) < std::abs(zInit)) ? 0.0f : length;
    m_upTarget.y = (std::abs(yInit) < std::abs(xInit) || std::abs(yInit) < std::abs(zInit)) ? 0.0f : length;
    m_upTarget.z = (std::abs(zInit) < std::abs(xInit) || std::abs(zInit) < std::abs(yInit)) ? 0.0f : length;

    m_upTarget.x *= (xInit < 0.0f) ? -1.0f : 1.0f;
    m_upTarget.y *= (yInit < 0.0f) ? -1.0f : 1.0f;
    m_upTarget.z *= (zInit < 0.0f) ? -1.0f : 1.0f;
}
void MoveLookController::RotateLeft90() noexcept
{
    // Only allow a single left/right movement at a time
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.5 seconds for the move
        InitializeAutomatedMove(0.5);
        m_rotatingLeftRight = true;
        m_totalRotationAngle = -1.0f * DirectX::XM_PIDIV2;
    }
}
void MoveLookController::RotateRight90() noexcept
{
    // Only allow a single left/right movement at a time
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.5 seconds for the move
        InitializeAutomatedMove(0.5);
        m_rotatingLeftRight = true;
        m_totalRotationAngle = DirectX::XM_PIDIV2;
    }
}
void MoveLookController::RotateUp90() noexcept
{
    // Only allow a single up/down movement at a time
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.5 seconds for the move
        InitializeAutomatedMove(0.5);
        m_rotatingUpDown = true;
        m_totalRotationAngle = DirectX::XM_PIDIV2;
    }
}
void MoveLookController::RotateDown90() noexcept
{
    // Only allow a single up/down movement at a time
    if (!m_movingToNewLocation)
    {
        // Set automated move flags and initial data - 0.5 seconds for the move
        InitializeAutomatedMove(0.5);
        m_rotatingUpDown = true;
        m_totalRotationAngle = -1.0f * DirectX::XM_PIDIV2;
    }
}
