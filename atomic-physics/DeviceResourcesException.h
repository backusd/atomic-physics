#pragma once
#include "pch.h"
#include "BaseException.h"
#include "DxgiInfoManager.h"
#include "MacroHelper.h"

#include <exception>
#include <sstream>
#include <vector>

#define HR HRESULT hr;

#if defined(_DEBUG)
#define INFOMAN DxgiInfoManager& infoManager = DeviceResources::GetInfoManager();
#endif

#define GFX_EXCEPT_NOINFO(hr) DeviceResourcesException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) { HR if( FAILED( hr = (hrcall) ) ) throw DeviceResourcesException( __LINE__,__FILE__,hr ); }

#if defined(_DEBUG)

	#define GFX_EXCEPT(hr) DeviceResourcesException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
	#define GFX_THROW_INFO(hrcall) TERMINATE_ON_THROW(HR INFOMAN infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr) )
	#define GFX_DEVICE_REMOVED_EXCEPT(hr) { INFOMAN DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() ) }
	#define GFX_THROW_INFO_ONLY(call) TERMINATE_ON_THROW(INFOMAN infoManager.Set(); call; {auto v = infoManager.GetMessages(); if(!v.empty()) {throw InfoException( __LINE__,__FILE__,v);}})

#else
	#define GFX_EXCEPT(hr) DeviceResourcesException( __LINE__,__FILE__,(hr) )
	//#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
	#define GFX_THROW_INFO(hrcall) { HR if( FAILED( hr = (hrcall) ) ) std::terminate(); }
	#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,(hr) )
	#define GFX_THROW_INFO_ONLY(call) call;
#endif




// ================================================================================

class DeviceResourcesException : public BaseException
{
public:
	DeviceResourcesException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorInfo() const noexcept;

	static std::string TranslateErrorCode(HRESULT hr) noexcept;

private:
	HRESULT hr;
	std::string m_info;
};

// ================================================================================

class InfoException : public BaseException
{
public:
	InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	std::string GetErrorInfo() const noexcept;
private:
	std::string m_info;
};

// ================================================================================

class DeviceRemovedException : public DeviceResourcesException
{
public:
	DeviceRemovedException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
	const char* GetType() const noexcept override;
private:
	std::string m_reason;
};