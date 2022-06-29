#pragma once
#include "pch.h"
#include "BaseException.h"


#define INPUT_LAYOUT_EXCEPT( description ) InputLayoutException( __LINE__,__FILE__, description)

class InputLayoutException : public BaseException
{
public:
	InputLayoutException(int line, const char* file, std::string description) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	std::string GetErrorInfo() const noexcept;
private:
	std::string m_info;
};