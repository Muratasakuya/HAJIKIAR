
/*===========================================================
                     Primitive VS Shader
===========================================================*/

struct TransformationMatrix
{

    float4x4 WVP;
};

struct VertexShaderOutput
{
    
    float4 position : SV_POSITION;
};

struct VertexShaderInput
{
    
    float4 position : POSITION0;
};

ConstantBuffer<TransformationMatrix> gTransformationMatirx : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    
    VertexShaderOutput output;
    
    output.position = mul(input.position, gTransformationMatirx.WVP);
    
    return output;
}