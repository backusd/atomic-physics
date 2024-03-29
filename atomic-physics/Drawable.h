#pragma once
#include "pch.h"
#include "Bindable.h"
#include "ConstantBufferArray.h"
#include "DepthStencilState.h"
#include "HLSLStructures.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "VertexShader.h"

#include "SphereMesh.h"
#include "MoveLookController.h"

#include <vector>
#include <memory>

class Drawable
{
public:
	Drawable(std::shared_ptr<MoveLookController> mlc) noexcept;
	Drawable(const Drawable&) = delete;
	void operator=(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual void Update() const noexcept {}
	virtual void Draw() const noexcept = 0;

	virtual DirectX::XMMATRIX GetScaleMatrix() const noexcept { return DirectX::XMMatrixIdentity(); }
	virtual DirectX::XMMATRIX GetRotationMatrix() const noexcept { return DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll); }
	virtual DirectX::XMMATRIX GetTranslationMatrix() const noexcept { return DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z); }
	DirectX::XMMATRIX GetModelMatrix() const noexcept { return GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix(); }

	void Position(float x, float y, float z) noexcept;
	void Velocity(float x, float y, float z) noexcept;



protected:


	std::unique_ptr<Mesh> m_mesh;
	std::shared_ptr<MoveLookController> m_moveLookController;
	DirectX::XMFLOAT3 m_position, m_velocity;
	float m_roll, m_pitch, m_yaw;
};