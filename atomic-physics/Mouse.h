#pragma once
#include "pch.h"

#include <queue>

class Mouse
{
public:
	// START Event ==================================================================================
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			LDoubleClick,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type m_type;
		bool m_leftIsPressed;
		bool m_rightIsPressed;
		int m_x;
		int m_y;
	public:
		Event() noexcept
			:
			m_type(Type::Invalid),
			m_leftIsPressed(false),
			m_rightIsPressed(false),
			m_x(0),
			m_y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			m_type(type),
			m_leftIsPressed(parent.m_leftIsPressed),
			m_rightIsPressed(parent.m_rightIsPressed),
			m_x(parent.m_x),
			m_y(parent.m_y)
		{}
		bool IsValid() const noexcept
		{
			return m_type != Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return m_type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return{ m_x,m_y };
		}
		int GetPosX() const noexcept
		{
			return m_x;
		}
		int GetPosY() const noexcept
		{
			return m_y;
		}
		bool LeftIsPressed() const noexcept
		{
			return m_leftIsPressed;
		}
		bool RightIsPressed() const noexcept
		{
			return m_rightIsPressed;
		}
	};
	// END Event ==================================================================================


public:
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	static std::pair<int, int> GetPos() noexcept { return Get().GetPosImpl(); }
	static int GetPosX() noexcept { return Get().GetPosXImpl(); }
	static int GetPosY() noexcept { return Get().GetPosYImpl(); }
	static bool IsInWindow() noexcept { return Get().IsInWindowImpl(); }
	static bool LeftIsPressed() noexcept { return Get().LeftIsPressedImpl(); }
	static bool RightIsPressed() noexcept { return Get().RightIsPressedImpl(); }
	static bool MiddleIsPressed() noexcept { return Get().MiddleIsPressedImpl(); }
	static Mouse::Event Read() noexcept { return Get().ReadImpl(); }
	static bool IsEmpty() noexcept { return Get().IsEmptyImpl(); }
	static void Flush() noexcept { return Get().FlushImpl(); }

	static void OnMouseMove(int x, int y) noexcept { return Get().OnMouseMoveImpl(x, y); }
	static void OnMouseLeave() noexcept { return Get().OnMouseLeaveImpl(); }
	static void OnMouseEnter() noexcept { return Get().OnMouseEnterImpl(); }
	static void OnLeftPressed(int x, int y) noexcept { return Get().OnLeftPressedImpl(x, y); }
	static void OnLeftReleased(int x, int y) noexcept { return Get().OnLeftReleasedImpl(x, y); }
	static void OnLeftDoubleClick(int x, int y) noexcept { return Get().OnLeftDoubleClickImpl(x, y); }
	static void OnRightPressed(int x, int y) noexcept { return Get().OnRightPressedImpl(x, y); }
	static void OnRightReleased(int x, int y) noexcept { return Get().OnRightReleasedImpl(x, y); }
	static void OnMiddlePressed(int x, int y) noexcept { return Get().OnMiddlePressedImpl(x, y); }
	static void OnMiddleReleased(int x, int y) noexcept { return Get().OnMiddleReleasedImpl(x, y); }
	static void OnWheelUp(int x, int y) noexcept { return Get().OnWheelUpImpl(x, y); }
	static void OnWheelDown(int x, int y) noexcept { return Get().OnWheelDownImpl(x, y); }
	static void TrimBuffer() noexcept { return Get().TrimBufferImpl(); }
	static void OnWheelDelta(int x, int y, int delta) noexcept { return Get().OnWheelDeltaImpl(x, y, delta); }


private:
	Mouse() noexcept;

	static Mouse& Get() noexcept
	{
		static Mouse mouse;
		return mouse;
	}

	std::pair<int, int> GetPosImpl() const noexcept;
	int GetPosXImpl() const noexcept;
	int GetPosYImpl() const noexcept;
	bool IsInWindowImpl() const noexcept;
	bool LeftIsPressedImpl() const noexcept;
	bool RightIsPressedImpl() const noexcept;
	bool MiddleIsPressedImpl() const noexcept;
	Mouse::Event ReadImpl() noexcept;
	bool IsEmptyImpl() const noexcept
	{
		return m_buffer.empty();
	}
	void FlushImpl() noexcept;

	void OnMouseMoveImpl(int x, int y) noexcept;
	void OnMouseLeaveImpl() noexcept;
	void OnMouseEnterImpl() noexcept;
	void OnLeftPressedImpl(int x, int y) noexcept;
	void OnLeftReleasedImpl(int x, int y) noexcept;
	void OnLeftDoubleClickImpl(int x, int y) noexcept;
	void OnRightPressedImpl(int x, int y) noexcept;
	void OnRightReleasedImpl(int x, int y) noexcept;
	void OnMiddlePressedImpl(int x, int y) noexcept;
	void OnMiddleReleasedImpl(int x, int y) noexcept;
	void OnWheelUpImpl(int x, int y) noexcept;
	void OnWheelDownImpl(int x, int y) noexcept;
	void TrimBufferImpl() noexcept;
	void OnWheelDeltaImpl(int x, int y, int delta) noexcept;


	static constexpr unsigned int m_bufferSize = 16u;
	int m_x = 0;
	int m_y = 0;
	bool m_leftIsPressed = false;
	bool m_rightIsPressed = false;
	bool m_middleIsPressed = false;
	bool m_isInWindow = false;
	int m_wheelDeltaCarry = 0;
	std::queue<Event> m_buffer;
};