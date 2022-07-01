#pragma once
#include "pch.h"
#include "Drawable.h"
#include "PhysicsConstants.h"

#include <memory>

class Sphere : public Drawable
{
public:
	Sphere(std::shared_ptr<MoveLookController> mlc) noexcept;
	Sphere(const Sphere&) = delete;
	void operator=(const Sphere&) = delete;

	void Update() const noexcept;
	void Draw() const noexcept_release_only;

	DirectX::XMMATRIX GetScaleMatrix() const noexcept override { return DirectX::XMMatrixScaling(m_radius, m_radius, m_radius); }

	void SetAtomType(int elementNumber) noexcept;

private:
	void UpdateModelViewProjectionBuffer() const;

	float m_radius;

	std::unique_ptr<ConstantBufferArray> psMaterialBufferArray;
};