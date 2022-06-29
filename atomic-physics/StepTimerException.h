#pragma once
#include "pch.h"
#include "BaseException.h"


#define TIMER_EXCEPT( description ) StepTimerException( __LINE__,__FILE__, description)

class StepTimerException : public BaseException
{
public:
	StepTimerException(int line, const char* file, std::string description) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	std::string GetErrorInfo() const noexcept;
private:
	std::string m_info;
};