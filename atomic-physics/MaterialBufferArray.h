#pragma once
#include "pch.h"
#include "ConstantBufferArray.h"
#include "HLSLStructures.h"

#include <vector>

class MaterialBufferArray : public ConstantBufferArray
{
public:
	MaterialBufferArray() noexcept;
	MaterialBufferArray(const MaterialBufferArray&) = delete;
	void operator=(const MaterialBufferArray&) = delete;

	void UpdateMaterialProperties() const noexcept;

private:
	void BindMaterialBuffer() const noexcept;

	std::unique_ptr<PhongMaterialProperties> m_materials;
};