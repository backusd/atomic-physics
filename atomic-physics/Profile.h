#pragma once
#include "TestConfig.h"

#ifdef PROFILE
#include <chrono>
#include <format>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#endif

#ifdef PROFILE
#define PROFILE_BEGIN_SESSION(name, filepath) Instrumentor::Get().BeginSession(name, filepath)
#define PROFILE_END_SESSION() Instrumentor::Get().EndSession()

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define TIMER_NAME CAT(timer, __LINE__)

#define PROFILE_SCOPE(name) InstrumentationTimer TIMER_NAME(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)



#else
#define PROFILE_BEGIN_SESSION(name, filepath)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

#ifdef PROFILE
struct ProfileResult
{
	std::string name;
	long long start, end;
	uint32_t threadID;
};

struct InstrumentationSession
{
	std::string name;
};

class Instrumentor
{
public:
	Instrumentor() : m_currentSession(nullptr), m_profileCount(0)
	{}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json")
	{
		m_outputStream.open(filepath);
		WriteHeader();
		m_currentSession = new InstrumentationSession{ name };
	}

	void EndSession()
	{
		WriteFooter();
		m_outputStream.close();
		delete m_currentSession;
		m_currentSession = nullptr;
		m_profileCount = 0;
	}

	void WriteProfile(const ProfileResult& result)
	{
		if (m_profileCount++ > 0)
			m_outputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_outputStream << "{";
		m_outputStream << "\"cat\":\"function\",";
		m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
		m_outputStream << "\"name\":\"" << name << "\",";
		m_outputStream << "\"ph\":\"X\",";
		m_outputStream << "\"pid\":0,";
		m_outputStream << "\"tid\":" << result.threadID << ",";
		m_outputStream << "\"ts\":" << result.start;
		m_outputStream << "}";
	
		m_outputStream.flush();
	}

	void WriteHeader()
	{
		m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_outputStream.flush();
	}

	void WriteFooter()
	{
		m_outputStream << "]}";
		m_outputStream.flush();
	}

	static Instrumentor& Get()
	{
		static Instrumentor* instance = new Instrumentor();
		return *instance;
	}

private:
	InstrumentationSession* m_currentSession;
	std::ofstream m_outputStream;
	int m_profileCount;
};


class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name) :
		m_name(name), m_stopped(false)
	{
		m_startTimePoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer()
	{
		if (!m_stopped)
			Stop();
	}

	void Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
		
		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_name, start, end, threadID });
		
		//long long duration = end - start;
		//double    ms = duration * 0.001;
		//OutputDebugString(std::format("{}us ({:.3f}ms)\n", duration, ms).c_str());

		m_stopped = true;
	}


private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	const char* m_name;
	bool m_stopped;
};


#endif // PROFILE
