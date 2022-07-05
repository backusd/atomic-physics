#pragma once
#include "pch.h"

#include <queue>
#include <bitset>

class Keyboard
{
public:
	// START Event ==================================================================================
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
	// END Event ==================================================================================


public:
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// key event stuff
	static bool KeyIsPressed(unsigned char keycode) noexcept { return Get().KeyIsPressedImpl(keycode); }
	static Event ReadKey() noexcept { return Get().ReadKeyImpl(); }
	static bool KeyIsEmpty() noexcept { return Get().KeyIsEmptyImpl(); }
	static void FlushKey() noexcept { return Get().FlushKeyImpl(); }
	// char event stuff
	static char ReadChar() noexcept { return Get().ReadCharImpl(); }
	static bool CharIsEmpty() noexcept { return Get().CharIsEmptyImpl(); }
	static void FlushChar() noexcept { return Get().FlushCharImpl(); }
	static void Flush() noexcept { return Get().FlushImpl(); }
	// autorepeat control
	static void EnableAutorepeat() noexcept { return Get().EnableAutorepeatImpl(); }
	static void DisableAutorepeat() noexcept { return Get().DisableAutorepeatImpl(); }
	static bool AutorepeatIsEnabled() noexcept { return Get().AutorepeatIsEnabledImpl(); }

	static void OnKeyPressed(unsigned char keycode) noexcept { return Get().OnKeyPressedImpl(keycode); }
	static void OnKeyReleased(unsigned char keycode) noexcept { return Get().OnKeyReleasedImpl(keycode); }
	static void OnChar(char character) noexcept { return Get().OnCharImpl(character); }
	static void ClearState() noexcept { return Get().ClearStateImpl(); }

private:
	Keyboard() noexcept;

	static Keyboard& Get() noexcept
	{
		static Keyboard keyboard;
		return keyboard;
	}

	// key event stuff
	bool KeyIsPressedImpl(unsigned char keycode) const noexcept;
	Event ReadKeyImpl() noexcept;
	bool KeyIsEmptyImpl() const noexcept;
	void FlushKeyImpl() noexcept;
	// char event stuff
	char ReadCharImpl() noexcept;
	bool CharIsEmptyImpl() const noexcept;
	void FlushCharImpl() noexcept;
	void FlushImpl() noexcept;
	// autorepeat control
	void EnableAutorepeatImpl() noexcept;
	void DisableAutorepeatImpl() noexcept;
	bool AutorepeatIsEnabledImpl() const noexcept;

	void OnKeyPressedImpl(unsigned char keycode) noexcept;
	void OnKeyReleasedImpl(unsigned char keycode) noexcept;
	void OnCharImpl(char character) noexcept;
	void ClearStateImpl() noexcept;


	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr unsigned int m_nKeys = 256u;
	static constexpr unsigned int m_bufferSize = 16u;
	bool m_autorepeatEnabled;
	std::bitset<m_nKeys> m_keystates;
	std::queue<Event> m_keybuffer;
	std::queue<char> m_charbuffer;
};