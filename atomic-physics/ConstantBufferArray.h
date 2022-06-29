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
	ConstantBufferArray(ConstantBufferBindingLocation bindToStage, BasicGeometry geometry);
	ConstantBufferArray(const ConstantBufferArray&) = delete;
	void operator=(const ConstantBufferArray&) = delete;

	void AddBuffer(std::shared_ptr<ConstantBuffer> buffer) noexcept;
	void ClearBuffers() noexcept;

	void Bind() const noexcept_release_only override;

	void UpdateSubresource(int index, void* data) noexcept_release_only;

	ID3D11Buffer* GetRawBufferPointer(int index) const noexcept { return m_buffers[index]->GetRawBufferPointer(); }


protected:
	void CreateSphereConstantBufferArray() noexcept;

	std::function<void()> BindFunc;

	void BindCS() const noexcept_release_only;
	void BindVS() const noexcept_release_only;
	void BindHS() const noexcept_release_only;
	void BindDS() const noexcept_release_only;
	void BindGS() const noexcept_release_only;
	void BindPS() const noexcept_release_only;

	std::vector<ID3D11Buffer*> m_rawBufferPointers;
	std::vector<std::shared_ptr<ConstantBuffer>> m_buffers;
};