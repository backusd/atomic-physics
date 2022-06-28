
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix modelViewProjection;
    matrix inverseTransposeModel;
};

struct VertexShaderInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 positionWS : POS_WS;
    float3 normalWS : NORM_WS;
};


// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;

    output.position = mul(modelViewProjection, input.position); // Screen position
    output.positionWS = mul(model, input.position); // World space position
    output.normalWS = mul((float3x3) inverseTransposeModel, input.normal.xyz); // compute the world space normal

    return output;
}