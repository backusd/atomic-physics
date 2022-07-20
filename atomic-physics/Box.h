#pragma once
#include "pch.h"
#include "Drawable.h"
#include "BoxMesh.h"

#include <memory>

class Box : public Drawable
{
public:
	Box(std::shared_ptr<MoveLookController> mlc) noexcept;
	Box(const Box&) = delete;
	void operator=(const Box&) = delete;

	void Draw() const noexcept override;

	DirectX::XMMATRIX GetScaleMatrix() const noexcept override { return DirectX::XMMatrixScaling(m_xLength / 2, m_yLength / 2, m_zLength / 2); }

	DirectX::XMFLOAT3 GetBoxSize() const noexcept { return { m_xLength, m_yLength, m_zLength }; }
	void SetBoxSize(float x, float y, float z) noexcept;
	void SetBoxSize(DirectX::XMFLOAT3 dimensions) noexcept;

private:
	void UpdateModelViewProjectionBuffer() const;

	float m_xLength;
	float m_yLength;
	float m_zLength;

	static std::unique_ptr<InputLayout>			m_inputLayout;
	static std::unique_ptr<VertexShader>		m_vertexShader;
	static std::unique_ptr<PixelShader>			m_pixelShader;
	static std::unique_ptr<BoxMesh>				m_mesh;
	static std::unique_ptr<ConstantBufferArray> m_vsBuffers;
	static std::unique_ptr<RasterizerState>		m_rasterizerState;
	static std::unique_ptr<DepthStencilState>	m_depthStencilState;
};