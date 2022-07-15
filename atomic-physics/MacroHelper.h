#pragma once
#include "pch.h"
#include "BaseException.h"

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)

// #define STRINGIFY2(X) #X
// #define STRINGIFY(X) STRINGIFY2(X)

#define ERROR_POPUP(text, caption) MessageBox(nullptr, text, caption, MB_OK | MB_ICONEXCLAMATION)
#define TERMINATE_ON_THROW(fn)	\
	try {																								\
		fn;																								\
	}																									\
    catch (const BaseException& e)																		\
	{																									\
		ERROR_POPUP(e.what(), e.GetType());																\
		std::terminate();																				\
	}																									\
	catch (const std::exception& e)																		\
	{																									\
		ERROR_POPUP(e.what(), "Standard Exception");													\
		std::terminate();																				\
	}																									\
	catch (...)																							\
	{																									\
		ERROR_POPUP("No details available", "Unknown Exception");										\
		std::terminate();																				\
	}