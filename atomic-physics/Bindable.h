#pragma once
#include "pch.h"
#include "DeviceResources.h"

class Bindable
{
public:
	Bindable() {}
	Bindable(const Bindable&) = delete;
	void operator=(const Bindable&) = delete;

	virtual void Bind() const noexcept_release_only = 0;
};