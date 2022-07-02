#include "Mouse.h"

Mouse::Mouse() noexcept :
	m_x(0),
	m_y(0),
	m_leftIsPressed(false),
	m_rightIsPressed(false),
	m_middleIsPressed(false),
	m_isInWindow(false),
	m_wheelDeltaCarry(0)
{
}

std::pair<int, int> Mouse::GetPosImpl() const noexcept
{
	return { m_x,m_y };
}

int Mouse::GetPosXImpl() const noexcept
{
	return m_x;
}

int Mouse::GetPosYImpl() const noexcept
{
	return m_y;
}

bool Mouse::IsInWindowImpl() const noexcept
{
	return m_isInWindow;
}

bool Mouse::LeftIsPressedImpl() const noexcept
{
	return m_leftIsPressed;
}

bool Mouse::RightIsPressedImpl() const noexcept
{
	return m_rightIsPressed;
}

bool Mouse::MiddleIsPressedImpl() const noexcept
{
	return m_middleIsPressed;
}

Mouse::Event Mouse::ReadImpl() noexcept
{
	if (m_buffer.size() > 0u)
	{
		Mouse::Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::FlushImpl() noexcept
{
	m_buffer = std::queue<Event>();
}

void Mouse::OnMouseMoveImpl(int newx, int newy) noexcept
{
	m_x = newx;
	m_y = newy;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBufferImpl();
}

void Mouse::OnMouseLeaveImpl() noexcept
{
	m_isInWindow = false;
	m_buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBufferImpl();
}

void Mouse::OnMouseEnterImpl() noexcept
{
	m_isInWindow = true;
	m_buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBufferImpl();
}

void Mouse::OnLeftPressedImpl(int /* x */, int /* y */) noexcept
{
	m_leftIsPressed = true;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBufferImpl();
}

void Mouse::OnLeftReleasedImpl(int /* x */, int /* y */) noexcept
{
	m_leftIsPressed = false;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBufferImpl();
}

void Mouse::OnLeftDoubleClickImpl(int /* x */, int /* y */) noexcept
{
	m_buffer.push(Mouse::Event(Mouse::Event::Type::LDoubleClick, *this));
	TrimBufferImpl();
}

void Mouse::OnRightPressedImpl(int /* x */, int /* y */) noexcept
{
	m_rightIsPressed = true;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBufferImpl();
}

void Mouse::OnRightReleasedImpl(int /* x */, int /* y */) noexcept
{
	m_rightIsPressed = false;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBufferImpl();
}

void Mouse::OnMiddlePressedImpl(int /* x */, int /* y */) noexcept
{
	m_middleIsPressed = true;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBufferImpl();
}

void Mouse::OnMiddleReleasedImpl(int /* x */, int /* y */) noexcept
{
	m_middleIsPressed = false;

	m_buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBufferImpl();
}

void Mouse::OnWheelUpImpl(int /* x */, int /* y */) noexcept
{
	m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBufferImpl();
}

void Mouse::OnWheelDownImpl(int /* x */, int /* y */) noexcept
{
	m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBufferImpl();
}

void Mouse::TrimBufferImpl() noexcept
{
	while (m_buffer.size() > m_bufferSize)
	{
		m_buffer.pop();
	}
}

void Mouse::OnWheelDeltaImpl(int x, int y, int delta) noexcept
{
	m_wheelDeltaCarry += delta;
	// generate events for every 120 
	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUpImpl(x, y);
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDownImpl(x, y);
	}
}