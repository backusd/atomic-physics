#pragma once
#include "pch.h"
#include "DeviceResources.h"

class Bindable
{
public:
	Bindable() {}
	Bindable(const Bindable&) = delete;
	void operator=(const Bindable&) = delete;
	virtual ~Bindable() {} // MUST be declared virtual so derived classes are properly destructed

	virtual void Bind() const noexcept_release_only = 0;
};