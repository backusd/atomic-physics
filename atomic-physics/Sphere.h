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
	void Draw() const noexcept_release_only override;

	unsigned int GetElementNumber() const noexcept { return m_elementNumber; }
	DirectX::XMMATRIX GetScaleMatrix() const noexcept override { return DirectX::XMMatrixScaling(m_radius, m_radius, m_radius); }

	void SetAtomType(int elementNumber) noexcept;

private:
	void UpdateModelViewProjectionBuffer() const;

	unsigned int m_elementNumber;
	float m_radius;

	std::unique_ptr<ConstantBufferArray> m_materialIndexBufferArray;


	static std::unique_ptr<InputLayout>			m_inputLayout;
	static std::unique_ptr<VertexShader>		m_vertexShader;
	static std::unique_ptr<PixelShader>			m_pixelShader;
	static std::unique_ptr<SphereMesh>			m_mesh;
	static std::unique_ptr<ConstantBufferArray> m_vsBuffers;
	static std::unique_ptr<RasterizerState>		m_rasterizerState;
	static std::unique_ptr<DepthStencilState>	m_depthStencilState;
};