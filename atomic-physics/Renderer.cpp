#include "Renderer.h"


Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp)
{
	m_moveLookController = std::make_unique<MoveLookController>(m_viewport);





	m_drawables.push_back(std::make_unique<Drawable>());
}

void Renderer::Update() noexcept
{

}

void Renderer::Render() noexcept
{
	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Draw each drawable
}

