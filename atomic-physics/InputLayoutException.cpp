#include "InputLayoutException.h"

InputLayoutException::InputLayoutException(int line, const char* file, std::string description) noexcept :
	BaseException(line, file)
{
	m_info = description;
}


const char* InputLayoutException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* InputLayoutException::GetType() const noexcept
{
	return "Input Layout Exception";
}

std::string InputLayoutException::GetErrorInfo() const noexcept
{
	return m_info;
}