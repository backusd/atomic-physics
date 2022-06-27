#include "Renderer.h"


Renderer::Renderer()
{

}

void Renderer::Update() noexcept
{

}

void Renderer::Render() noexcept
{
	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	context->ClearDepthStencilView(DeviceResources::DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView* const targets[1] = { DeviceResources::BackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, DeviceResources::DepthStencilView());

	const FLOAT clearColor[4] = { 55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f };
	context->ClearRenderTargetView(DeviceResources::BackBufferRenderTargetView(), clearColor);
}