#include "Drawable.h"

Drawable::Drawable() noexcept
{
	std::shared_ptr<ConstantBuffer> cb = std::make_shared<ConstantBuffer>();
	cb->CreateBuffer<ModelViewProjectionConstantBuffer>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);

	std::unique_ptr<ConstantBufferArray> cba = std::make_unique<ConstantBufferArray>(ConstantBufferBindingLocation::VERTEX_SHADER);
	cba->AddBuffer(cb);

}