#pragma once
#include "pch.h"
#include "Drawable.h"



class Sphere : public Drawable
{
public:
	Sphere(std::shared_ptr<MoveLookController> mlc) noexcept;
	Sphere(const Sphere&) = delete;
	void operator=(const Sphere&) = delete;

	void Update() const noexcept;
	void Draw() const noexcept_release_only;

	DirectX::XMMATRIX GetScaleMatrix() const noexcept override { return DirectX::XMMatrixScaling(m_radius, m_radius, m_radius); }

	void Radius(float radius) noexcept { m_radius = radius; }

private:
	void UpdateModelViewProjectionBuffer() const;

	float m_radius;
};