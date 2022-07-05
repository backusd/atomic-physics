#include "Keyboard.h"

Keyboard::Keyboard() noexcept :
	m_autorepeatEnabled(false)
{
}

bool Keyboard::KeyIsPressedImpl(unsigned char keycode) const noexcept
{
	return m_keystates[keycode];
}

Keyboard::Event Keyboard::ReadKeyImpl() noexcept
{
	if (m_keybuffer.size() > 0u)
	{
		Keyboard::Event e = m_keybuffer.front();
		m_keybuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmptyImpl() const noexcept
{
	return m_keybuffer.empty();
}

char Keyboard::ReadCharImpl() noexcept
{
	if (m_charbuffer.size() > 0u)
	{
		unsigned char charcode = m_charbuffer.front();
		m_charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmptyImpl() const noexcept
{
	return m_charbuffer.empty();
}

void Keyboard::FlushKeyImpl() noexcept
{
	m_keybuffer = std::queue<Event>();
}

void Keyboard::FlushCharImpl() noexcept
{
	m_charbuffer = std::queue<char>();
}

void Keyboard::FlushImpl() noexcept
{
	FlushKeyImpl();
	FlushCharImpl();
}

void Keyboard::EnableAutorepeatImpl() noexcept
{
	m_autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeatImpl() noexcept
{
	m_autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabledImpl() const noexcept
{
	return m_autorepeatEnabled;
}

void Keyboard::OnKeyPressedImpl(unsigned char keycode) noexcept
{
	m_keystates[keycode] = true;
	m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_keybuffer);
}

void Keyboard::OnKeyReleasedImpl(unsigned char keycode) noexcept
{
	m_keystates[keycode] = false;
	m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(m_keybuffer);
}

void Keyboard::OnCharImpl(char character) noexcept
{
	m_charbuffer.push(character);
	TrimBuffer(m_charbuffer);
}

void Keyboard::ClearStateImpl() noexcept
{
	m_keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_bufferSize)
	{
		buffer.pop();
	}
}