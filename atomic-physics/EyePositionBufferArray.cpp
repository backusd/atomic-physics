#include "EyePositionBufferArray.h"

using DirectX::XMFLOAT4;
using DirectX::XMMATRIX;

EyePositionBufferArray::EyePositionBufferArray(std::shared_ptr<MoveLookController> mlc) :
	ConstantBufferArray(ConstantBufferBindingLocation::PIXEL_SHADER),
	m_moveLookController(mlc)
{
	BindFunc = std::bind(&EyePositionBufferArray::BindEyePositionBuffer, this);
	
	XMFLOAT4 position;
	DirectX::XMStoreFloat4(&position, m_moveLookController->Position());

	std::shared_ptr<ConstantBuffer> eyePositionBuffer = std::make_shared<ConstantBuffer>();
	eyePositionBuffer->CreateBuffer<LightProperties>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0, static_cast<void*>(&position));

	AddBuffer(eyePositionBuffer);
}

void EyePositionBufferArray::BindEyePositionBuffer() const noexcept_release_only
{
	// Call PSSetConstantBuffers to bind the buffer to slot 1
	GFX_THROW_INFO_ONLY(
		DeviceResources::D3DDeviceContext()->PSSetConstantBuffers(1u, static_cast<unsigned int>(m_rawBufferPointers.size()), m_rawBufferPointers.data())
	);
}

void EyePositionBufferArray::Update() const
{
	ID3D11DeviceContext4* context = DeviceResources::D3DDeviceContext();
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Update PS constant buffer with eye position data (ALWAYS at slot 1)
	Microsoft::WRL::ComPtr<ID3D11Buffer> psBuffer;
	GFX_THROW_INFO_ONLY(
		context->PSGetConstantBuffers(1, 1, psBuffer.ReleaseAndGetAddressOf())
	);

	GFX_THROW_INFO(
		context->Map(psBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)
	);

	EyePositionBuffer* mappedBuffer = (EyePositionBuffer*)ms.pData;
	DirectX::XMStoreFloat4(&(mappedBuffer->eyePosition), m_moveLookController->Position());

	GFX_THROW_INFO_ONLY(
		context->Unmap(psBuffer.Get(), 0)
	);
}