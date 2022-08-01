
#define MAX_INSTANCES 300

struct ModelViewProjection
{
    matrix model;
    matrix modelViewProjection;
    matrix inverseTransposeModel;
};

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    ModelViewProjection instanceData[MAX_INSTANCES];
};

struct VertexShaderInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    uint instanceID : SV_InstanceID;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 positionWS : POS_WS;
    float3 normalWS : NORM_WS;
    uint instanceID : Instance_ID;
};


// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    
    ModelViewProjection data = instanceData[input.instanceID];

    output.position = mul(data.modelViewProjection, input.position); // Screen position
    output.positionWS = mul(data.model, input.position); // World space position
    output.normalWS = mul((float3x3) data.inverseTransposeModel, input.normal.xyz); // compute the world space normal

    output.instanceID = input.instanceID;
    
    return output;
}