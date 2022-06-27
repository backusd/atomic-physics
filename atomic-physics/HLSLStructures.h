#pragma once
#include "pch.h"

struct ModelViewProjectionConstantBuffer
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 modelViewProjection;
    DirectX::XMFLOAT4X4 inverseTransposeModel;
};