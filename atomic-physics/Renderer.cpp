#include "Renderer.h"


Renderer::Renderer(D3D11_VIEWPORT vp) noexcept :
	m_viewport(vp)
{
	m_moveLookController = std::make_shared<MoveLookController>(m_viewport);





	m_drawables.push_back(std::make_unique<Sphere>(m_moveLookController));
}

void Renderer::Update() noexcept
{
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Update();
}

void Renderer::Render() noexcept
{
	// Set the viewport (should be specific to this renderer)
	DeviceResources::SetViewport(m_viewport);

	// Draw each drawable
	for (const std::unique_ptr<Drawable>& drawable : m_drawables)
		drawable->Draw();
}

