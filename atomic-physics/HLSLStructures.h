#pragma once
#include "pch.h"

struct PositionNormalVertex
{
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 normal;
};

struct ColorConstantBuffer
{
    DirectX::XMFLOAT4 color;
};

struct ModelViewProjectionConstantBuffer
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 modelViewProjection;
    DirectX::XMFLOAT4X4 inverseTransposeModel;
};

struct ModelViewProjection
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
};