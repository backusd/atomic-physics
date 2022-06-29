#include "StepTimerException.h"

StepTimerException::StepTimerException(int line, const char* file, std::string description) noexcept :
	BaseException(line, file)
{
	m_info = description;
}


const char* StepTimerException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* StepTimerException::GetType() const noexcept
{
	return "Step Timer Exception";
}

std::string StepTimerException::GetErrorInfo() const noexcept
{
	return m_info;
}