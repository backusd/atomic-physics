#pragma once
#include "pch.h"
#include "DeviceResources.h"


class Renderer
{
public:
	Renderer();

	void Update() noexcept;
	void Render() noexcept;

private:
};