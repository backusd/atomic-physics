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
	Instrumentor() : 
		m_currentSession(nullptr), 
		m_dataCount(0), 
		m_remainingFrames(0),
		m_capturingFrames(false)
	{}

	bool SessionIsActive() { return m_currentSession != nullptr; }

	std::string SessionName()
	{
		if (m_currentSession != nullptr)
			return m_currentSession->name;

		return "";
	}

	void CaptureFrames(unsigned int frameCount, const std::string& name, const std::string& filepath)
	{
		m_capturingFrames = true;
		m_remainingFrames = frameCount;

		// Don't begin session, wait until the start of the next frame
		//BeginSession(name, filepath);
		m_name = name;
		m_filepath = filepath;
	}

	void NotifyNextFrame()
	{
		if (SessionIsActive())
		{
			// If we are capturing frames, see if we need to end the session
			if (m_capturingFrames)
			{
				--m_remainingFrames;

				if (m_remainingFrames == 0)
				{
					m_capturingFrames = false;
					EndSession();
				}
			}
		}
		else if (m_capturingFrames)
		{
			// If there is no active session, but we need to start capturing frames, begin a new session
			BeginSession();
		}
	}

	void BeginSession()
	{
		m_dataCount = 0;
		m_filepath = m_filepath;
		m_currentSession = new InstrumentationSession{ m_name };
	}
	void BeginSession(const std::string& name, std::string filepath = "results.json")
	{
		m_dataCount = 0;
		m_filepath = filepath;
		m_currentSession = new InstrumentationSession{ name };
	}

	void EndSession()
	{
		std::ofstream outFile(m_filepath);

		// Header
		outFile << "{\"otherData\": {},\"traceEvents\":[";

		// Data
		for (unsigned int iii = 0; iii < m_dataCount; ++iii)
		{
			if (iii != 0)
				outFile << ",";

			outFile << "{";
			outFile << "\"cat\":\"function\",";
			outFile << "\"dur\":" << (m_data[iii].end - m_data[iii].start) << ",";
			outFile << "\"name\":\"" << m_data[iii].name << "\",";
			outFile << "\"ph\":\"X\",";
			outFile << "\"pid\":0,";
			outFile << "\"tid\":" << m_data[iii].threadID << ",";
			outFile << "\"ts\":" << m_data[iii].start;
			outFile << "}";
		}

		// Footer
		outFile << "]}";

		outFile.close();

		delete m_currentSession;
		m_currentSession = nullptr;
	}

	void WriteProfile(const std::string& name, long long start, long long end, uint32_t threadID)
	{
		if (m_dataCount < 999999)
		{
			m_data[m_dataCount].name = name;
			m_data[m_dataCount].start = start;
			m_data[m_dataCount].end = end;
			m_data[m_dataCount].threadID = threadID;

			++m_dataCount;
		}
	}

	static Instrumentor& Get()
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
};


class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name) :
		m_name(name),
		m_stopped(false)
	{
		// Don't do anything if session is not active
		if (Instrumentor::Get().SessionIsActive())
		{
			// Perform string processing before starting the timer
			//		Remove __cdecl
			size_t position = m_name.find("__cdecl ");
			if (position != std::string::npos)
				m_name.erase(position, 8);
			//		Replace (void) -> ()
			position = m_name.find("(void)");
			if (position != std::string::npos)
				m_name.erase(position + 1, 4);   // just erase "void" in "(void)"
			//		Replace " -> '
			std::replace(m_name.begin(), m_name.end(), '"', '\'');

			m_startTimePoint = std::chrono::high_resolution_clock::now();
		}
	}

	~InstrumentationTimer()
	{
		if (Instrumentor::Get().SessionIsActive() && !m_stopped)
			Stop();
	}

	void Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

		Instrumentor::Get().WriteProfile(m_name, start, end, threadID);

		//std::chrono::time_point<std::chrono::high_resolution_clock> testEndPoint = std::chrono::high_resolution_clock::now();
		//long long testEnd = std::chrono::time_point_cast<std::chrono::microseconds>(testEndPoint).time_since_epoch().count();

		//long long duration = testEnd - start;
		//double    ms = duration * 0.001;
		//OutputDebugString(std::format("{}us ({:.3f}ms)\n", duration, ms).c_str());

		m_stopped = true;
	}


private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	std::string m_name;
	bool m_stopped;
};


#endif // PROFILE
