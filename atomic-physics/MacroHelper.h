#pragma once
#include "pch.h"

#include <format>
#include <exception>
#include <string>

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define ERROR_POPUP(text, caption) MessageBox(nullptr, text, caption, MB_OK | MB_ICONEXCLAMATION)
#define TERMINATE_ON_THROW(fn)	\
	try {																								\
		fn;																								\
	}																									\
	catch (const std::exception& e)																		\
	{																									\
		std::string text = "Function: " STRINGIFY(fn) " threw an exception\n";							\
		text +=            "    FILE: " STRINGIFY(__FILE__) "\n";										\
		text +=            "    LINE: " STRINGIFY(__LINE__) "\n";										\
		text +=			   "    Details:\n";															\
		text += e.what();																				\
		ERROR_POPUP(text.c_str(), "" STRINGIFY(fn) " ERROR");															\
		std::terminate();																				\
	}																									\
	catch (...)																							\
	{																									\
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);	\
		std::terminate();																				\
	}																									

