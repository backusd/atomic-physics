#pragma once
#include "TestConfig.h"

#ifdef PROFILE
#include <chrono>
#include <format>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <array>
#endif

#ifdef PROFILE
	#define PROFILE_BEGIN_SESSION(name, filepath) Instrumentor::Get().BeginSession(name, filepath)
	#define PROFILE_END_SESSION() Instrumentor::Get().EndSession()
	#define PROFILE_NEXT_FRAME() Instrumentor::Get().NotifyNextFrame()

	#define CAT2(a,b) a##b
	#define CAT(a,b) CAT2(a,b)
	#define TIMER_VAR_NAME CAT(timer, __LINE__)

	#define PROFILE_SCOPE(name) InstrumentationTimer TIMER_VAR_NAME(name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PROFILE_BEGIN_SESSION(name, filepath)
	#define PROFILE_END_SESSION()
	#define PROFILE_NEXT_FRAME()
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif

#ifdef PROFILE
struct ProfileResult
{
	std::string name = "";
	long long start = 0, end = 0;
	uint32_t threadID = 0;
};

struct InstrumentationSession
{
	std::string name;
};

class Instrumentor
{
public:
	Instrumentor() noexcept : 
		m_currentSession(nullptr), 
		m_dataCount(0), 
		m_remainingFrames(0),
		m_capturingFrames(false),
		m_capturingSeconds(false),
		m_capturingEndTime(0)
	{}

	bool SessionIsActive() const noexcept { return m_currentSession != nullptr; }

	std::string SessionName() const noexcept; 

	void CaptureFrames(unsigned int frameCount, const std::string& name, const std::string& filepath) noexcept;
	void CaptureSeconds(unsigned int seconds, const std::string& name, const std::string& filepath) noexcept;

	void NotifyNextFrame() noexcept;

	void BeginSession() noexcept;
	void BeginSession(const std::string& name, std::string filepath = "results.json") noexcept;

	void EndSession() noexcept;

	void WriteProfile(const std::string& name, long long start, long long end, uint32_t threadID) noexcept;

	static Instrumentor& Get() noexcept
	{
		static Instrumentor* instance = new Instrumentor();
		return *instance;
	}

private:
	InstrumentationSession* m_currentSession;

	std::string m_filepath, m_name;

	std::array<ProfileResult, 1000000> m_data;
	unsigned int m_dataCount;

	unsigned int m_remainingFrames;
	bool m_capturingFrames;

	bool m_capturingSeconds;
	long long m_capturingEndTime;
};


class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name) noexcept;

	~InstrumentationTimer()
	{
		if (Instrumentor::Get().SessionIsActive() && !m_stopped)
			Stop();
	}

	void Stop() noexcept;


private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	std::string m_name;
	bool m_stopped;
};


#endif // PROFILE
