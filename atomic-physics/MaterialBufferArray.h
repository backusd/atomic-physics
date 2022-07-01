#pragma once
#include "pch.h"
#include "ConstantBufferArray.h"
#include "HLSLStructures.h"

#include <vector>

class MaterialBufferArray : public ConstantBufferArray
{
public:
	MaterialBufferArray();
	MaterialBufferArray(const MaterialBufferArray&) = delete;
	void operator=(const MaterialBufferArray&) = delete;

	void UpdateMaterialProperties() const noexcept_release_only;

private:
	void BindMaterialBuffer() const noexcept_release_only;

	std::unique_ptr<PhongMaterialProperties> m_materials;
};