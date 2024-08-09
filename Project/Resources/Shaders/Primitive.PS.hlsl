
/*===========================================================
                     Primitive PS Shader
===========================================================*/

struct Material
{
    
    float4 color;
};

struct PixelShaderOutput
{
    
    float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main()
{
    
    PixelShaderOutput output;
    
    output.color = gMaterial.color;
    
    return output;
}