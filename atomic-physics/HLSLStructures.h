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

struct EyePositionBuffer
{
    DirectX::XMFLOAT4 eyePosition;
};

struct ModelViewProjectionPreMultiplied
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

constexpr int MAX_INSTANCES = 300;

struct ModelViewProjectionPreMultipliedArray
{
    ModelViewProjectionPreMultiplied instanceData[MAX_INSTANCES];
};

struct _PhongMaterial
{
    _PhongMaterial()
        : Emissive(0.0f, 0.0f, 0.0f, 1.0f)
        , Ambient(0.1f, 0.1f, 0.1f, 1.0f)
        , Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
        , Specular(1.0f, 1.0f, 1.0f, 1.0f)
        , SpecularPower(128.0f)
        , UseTexture(false)
    {}

    DirectX::XMFLOAT4   Emissive;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4   Ambient;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4   Diffuse;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4   Specular;
    //----------------------------------- (16 byte boundary)
    float               SpecularPower;
    // Add some padding complete the 16 byte boundary.
    BOOL                UseTexture;
    // Add some padding to complete the 16 byte boundary.
    float               Padding[2] = { 0.0f, 0.0f }; // Initialize just to get rid of unintialization warnings
    //----------------------------------- (16 byte boundary)
}; // Total:                                80 bytes (5 * 16)

constexpr int NUM_PHONG_MATERIALS = 10;

struct PhongMaterialProperties
{
    _PhongMaterial   Materials[NUM_PHONG_MATERIALS];
};


// Not sure why, but the 3 bytes of padding is necessary here.
// I am getting a problem in HLSL where trying to create a cbuffer with an array
// of uint adds 3 bytes of HIDDEN padding to each uint element
struct PhongMaterialIndexBuffer
{
    unsigned int materialIndex;
    unsigned int padding[3];    // DO NOT REMOVE
};

struct PhongMaterialIndexArray
{
    PhongMaterialIndexBuffer materialIndex[MAX_INSTANCES];
};


constexpr int MAX_LIGHTS = 8;

enum LightType
{
    DirectionalLight = 0,
    PointLight = 1,
    SpotLight = 2
};

struct Light
{
    Light()
        : Position(0.0f, 0.0f, 0.0f, 1.0f)
        , Direction(0.0f, 0.0f, 1.0f, 0.0f)
        , Color(1.0f, 1.0f, 1.0f, 1.0f)
        , SpotAngle(DirectX::XM_PIDIV2)
        , ConstantAttenuation(1.0f)
        , LinearAttenuation(0.0f)
        , QuadraticAttenuation(0.0f)
        , LightType(DirectionalLight)
        , Enabled(0)
    {}

    DirectX::XMFLOAT4    Position;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4    Direction;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4    Color;
    //----------------------------------- (16 byte boundary)
    float       SpotAngle;
    float       ConstantAttenuation;
    float       LinearAttenuation;
    float       QuadraticAttenuation;
    //----------------------------------- (16 byte boundary)
    int         LightType;
    int         Enabled;
    // Add some padding to make this struct size a multiple of 16 bytes.
    int         Padding[2] = { 0, 0 }; // Initialize just to get rid of unintialization warnings
    //----------------------------------- (16 byte boundary)
};  // Total:                              80 bytes ( 5 * 16 )

struct LightProperties
{
    LightProperties()
        : //EyePosition(0.0f, 0.0f, 0.0f, 1.0f),
        GlobalAmbient(0.2f, 0.2f, 0.8f, 1.0f)
    {}

    //DirectX::XMFLOAT4   EyePosition;
    //----------------------------------- (16 byte boundary)
    //DirectX::XMFLOAT4   GlobalAmbient;
    //----------------------------------- (16 byte boundary)
    //Light               Lights[MAX_LIGHTS]; // 80 * 8 bytes
    // Total:                                  672 bytes (42 * 16)


    DirectX::XMFLOAT4   GlobalAmbient;
    //----------------------------------- (16 byte boundary)
    Light               Lights[MAX_LIGHTS]; // 80 * 8 bytes
    // Total:                                  672 bytes (42 * 16)
};  