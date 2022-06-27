#pragma once
#include "pch.h"
#include "Bindable.h"
#include "ConstantBufferArray.h"
#include "DepthStencilState.h"
#include "HLSLStructures.h"
#include "InputLayout.h"


#include <vector>
#include <memory>

class Drawable
{
public:
	Drawable() noexcept;
	Drawable(const Drawable&) = delete;
	void operator=(const Drawable&) = delete;
	~Drawable()
	{
	}


private:


	std::vector<std::unique_ptr<Bindable>> m_bindables;
};