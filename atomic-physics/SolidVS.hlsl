// #include "VSInclude.hlsli" // Declares VS constant buffer at slot 0

/////////////
// GLOBALS //
/////////////
//cbuffer ModelViewProjectionConstantBuffer : register(b0)
//{
//    matrix model;
//    matrix modelViewProjection;
//    matrix inverseTransposeModel;
//};

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    output.position = mul(worldMatrix, input.position);
    output.position = mul(viewMatrix, output.position);
    output.position = mul(projectionMatrix, output.position);

    // Store the input color for the pixel shader to use.
    output.color = input.color;

    return output;
}