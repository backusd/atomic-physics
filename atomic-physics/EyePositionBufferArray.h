#pragma once
#include "pch.h"
#include "ConstantBufferArray.h"
#include "HLSLStructures.h"
#include "MoveLookController.h"

#include <memory>

class EyePositionBufferArray : public ConstantBufferArray
{
public:
	EyePositionBufferArray(std::shared_ptr<MoveLookController> mlc) noexcept;
	EyePositionBufferArray(const EyePositionBufferArray&) = delete;
	void operator=(const EyePositionBufferArray&) = delete;

	void Update() const;

private:
	void BindEyePositionBuffer() const noexcept_release_only;

	std::shared_ptr<MoveLookController> m_moveLookController;
};