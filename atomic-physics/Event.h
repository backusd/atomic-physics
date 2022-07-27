#pragma once
#include "pch.h"

#include <functional>
#include <unordered_map>

using EventToken = std::intptr_t;

template<class... T>
class Event
{
public:
	Event() noexcept {}

	template<class... T>
	EventToken AddHandler(std::function<void(T...)> fn) noexcept
	{
		// Just take the address of the function in memory as the token, which
		// therefore guarantees its uniqueness
		EventToken token = reinterpret_cast<EventToken>(&fn);
		m_handlers[token] = fn;
		return token;
	}

	bool RemoveHandler(EventToken token) noexcept 
	{ 
		bool v = m_handlers.erase(token);
		return v;
		//return m_handlers.erase(token); 
	}

	// overload operator() to trigger the event
	template<class... T>
	void operator()(T... args) noexcept
	{
		// Trigger each event handler
		for (auto& handler : m_handlers)
			handler.second(std::forward<T>(args)...);
	}

private:
	std::unordered_map<EventToken, std::function<void(T...)>> m_handlers;
};