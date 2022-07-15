#pragma once
#include "pch.h"
#include "ConstantBufferArray.h"
#include "HLSLStructures.h"

#include <memory>

class Lighting : public ConstantBufferArray
{
public:
	Lighting() noexcept;
	Lighting(const Lighting&) = delete;
	void operator=(const Lighting&) = delete;

	void GlobalAmbient(DirectX::XMFLOAT4 value) noexcept_release_only { PROFILE_FUNCTION(); m_properties.GlobalAmbient = value; UpdateLightingProperties(); }
	void EditLight(
		int index,
		DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 direction,
		bool enabled = true,
		int lightType = 1,
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(DirectX::Colors::White),
		float spotAngle = DirectX::XMConvertToRadians(45.0f),
		float constantAttenuation = 1.0f,
		float linearAttenuation = 0.08f,
		float quadraticAttenuation = 0.0f) noexcept_release_only;


private:
	void CreateLightProperties() noexcept;
	void BindLightingBuffer() const noexcept_release_only;
	void UpdateLightingProperties() noexcept_release_only;

	LightProperties m_properties;
};