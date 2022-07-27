#include "Event.h"

// Event Handler ===============================================================

/*
EventHandler::EventHandler(std::function<void()> fn) noexcept :
	m_fn(fn)
{
}

// overload operator() to trigger the event
void EventHandler::operator()() noexcept
{
	m_fn();
}
*/


// Event =======================================================================

//EventToken m_nextToken = 0;

/*
Event::Event() noexcept
{
}

EventToken Event::AddHandler(std::function<void()> fn) noexcept
{
	// Increment token is it is unique and different than previous token
	++m_nextToken;

	m_handlers[m_nextToken] = EventHandler(fn);
}


bool Event::RemoveHandler(EventToken token) noexcept
{
	return m_handlers.erase(token);
}

void Event::operator()() noexcept
{
	// Trigger each event handler
	for (auto& handler : m_handlers)
		handler.second();
}
*/

// ParticleAddedEvent =============================================================

/*
ParticleAddedEvent::ParticleAddedEvent() noexcept : Event()
{
}
*/
