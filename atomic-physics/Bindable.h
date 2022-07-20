#pragma once
#include "pch.h"
#include "DeviceResources.h"

class Bindable
{
public:
	Bindable() noexcept {}
	Bindable(const Bindable&) = delete;
	void operator=(const Bindable&) = delete;
	virtual ~Bindable() = default; // MUST be declared virtual so derived classes are properly destructed

	virtual void Bind() const noexcept = 0;
};