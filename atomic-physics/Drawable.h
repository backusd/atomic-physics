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

	virtual void Update() const {}
	virtual void Draw() const noexcept_release_only = 0;

protected:


	std::vector<std::unique_ptr<Bindable>> m_bindables;
	std::unique_ptr<Mesh> m_mesh;
	std::shared_ptr<MoveLookController> m_moveLookController;
};