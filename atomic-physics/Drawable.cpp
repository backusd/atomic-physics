#include "Drawable.h"

Drawable::Drawable(std::shared_ptr<MoveLookController> mlc) noexcept :
	m_moveLookController(mlc),
	m_mesh(nullptr),
	m_position{},
	m_velocity{},
	m_roll(0.0f),
	m_pitch(0.0f),
	m_yaw(0.0f)
{
}

void Drawable::Position(float x, float y, float z) noexcept
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Drawable::Velocity(float x, float y, float z) noexcept
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}