#pragma once
#include "pch.h"
#include "BasicGeometry.h"
#include "Bindable.h"
#include "ConstantBuffer.h"
#include "HLSLStructures.h"


#include <vector>
#include <functional>

enum class ConstantBufferBindingLocation
{
	COMPUTE_SHADER = 0,
	VERTEX_SHADER = 1,
	HULL_SHADER = 2,
	DOMAIN_SHADER = 3,
	GEOMETRY_SHADER = 4,
	PIXEL_SHADER = 5
};

class ConstantBufferArray : public Bindable
{
public:
	ConstantBufferArray(ConstantBufferBindingLocation bindToStage) noexcept;
	ConstantBufferArray(ConstantBufferBindingLocation bindToStage, BasicGeometry geometry) noexcept;
	ConstantBufferArray(const ConstantBufferArray&) = delete;
	void operator=(const ConstantBufferArray&) = delete;

	void AddBuffer(std::shared_ptr<ConstantBuffer> buffer) noexcept;
	void ClearBuffers() noexcept;

	void Bind() const noexcept override;

	void UpdateSubresource(int index, void* data) noexcept;

	ID3D11Buffer* GetRawBufferPointer(int index) const noexcept { return m_buffers[index]->GetRawBufferPointer(); }


protected:
	void CreateBoxConstantBufferArray() noexcept;
	void CreateSphereConstantBufferArray() noexcept;

	std::function<void()> BindFunc;

	void BindCS() const noexcept;
	void BindVS() const noexcept;
	void BindHS() const noexcept;
	void BindDS() const noexcept;
	void BindGS() const noexcept;
	void BindPS() const noexcept;

	std::vector<ID3D11Buffer*> m_rawBufferPointers;
	std::vector<std::shared_ptr<ConstantBuffer>> m_buffers;
};