#include "PhongReflection.hlsli"

/*===========================================================
                PhongReflection VS Shader
===========================================================*/

struct Material {
    
	float4 color;
	int enableLighting;
	int enablePhongReflection;
	int enableBlinnPhongReflection;
	float4x4 uvTransform;
	float3 specularColor;
	float shininess;
};

struct DirectionalLight {

	float4 color;
	float3 direction;
	float intensity;
};

struct PointLight {
	
	float4 color;
	float3 pos;
	float intensity;
	float radius;
	float decay;
};

struct SpotLight {

	float4 color;
	float3 pos;
	float intensity;
	float3 direction;
	float distance;
	float decay;
	float cosAngle;
	float cosFalloffStart;
};

struct Camera {

	float3 worldPosition;
};

struct PixelShaderOutput {
    
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
    
	float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
	PixelShaderOutput output;
    
	if (gMaterial.enableLighting == 1) {
		if (gMaterial.enablePhongReflection == 1) {
            
            /*=================================================================================================*/
            ///
            ///                                    PhongReflection
            ///
            /*=================================================================================================*/
            
            // texture‚Ìƒ¿’l‚ª0.5fˆÈ‰º‚ÌŽž‚ÉPixel‚ðŠü‹p
			if (textureColor.a <= 0.5f) {
            
				discard;
			}
            // texture‚Ìƒ¿’l‚ª0.0f‚ÌŽž‚ÉPixel‚ðŠü‹p
			if (textureColor.a == 0.0f) {
            
				discard;
			}
            
            // PointLight‚Ì“üŽËŒõ
			float3 pointLightDirection = normalize(input.worldPosition - gPointLight.pos);
            
            // PointLight‚Ö‚Ì‹——£
			float distancePointLight = length(gPointLight.pos - input.worldPosition);
            // Žw”‚É‚æ‚éƒRƒ“ƒgƒ[ƒ‹
			float factorPointLight = pow(saturate(-distancePointLight / gPointLight.radius + 1.0f), gPointLight.decay);
            
            // SpotLight‚Ì“üŽËŒõ
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.pos);
            
            // SpotLight‚Ö‚Ì‹——£
			float distanceSpotLight = length(gSpotLight.pos - input.worldPosition);
            // Žw”‚É‚æ‚éƒRƒ“ƒgƒ[ƒ‹
			float factorSpotLight = pow(saturate(-distanceSpotLight / gSpotLight.distance + 1.0f), gSpotLight.decay);
            
            // Camera•ûŒüŽZo
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight
            
            // “üŽËŒõ‚Ì”½ŽËƒxƒNƒgƒ‹‚ÌŒvŽZ
			float3 reflectDirectionalLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
            
			float RdotEDirectionalLight = dot(reflectDirectionalLight, toEye);
			float specularPowDirectionalLight = pow(saturate(RdotEDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // ŠgŽU”½ŽË
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cosDirectionalLight * gDirectionalLight.intensity;
            
            // ‹¾–Ê”½ŽË
			float3 specularDirectionalLight =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
            // “üŽËŒõ‚Ì”½ŽËƒxƒNƒgƒ‹‚ÌŒvŽZ
			float3 reflectPointLight = reflect(pointLightDirection, normalize(input.normal));
            
			float RdotEPointLight = dot(reflectPointLight, toEye);
			float specularPowPointLight = pow(saturate(RdotEPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // ŠgŽU”½ŽË
			float3 diffusePointLight =
            gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cosPointLight * gPointLight.intensity * factorPointLight;
            
            // ‹¾–Ê”½ŽË
			float3 specularPointLight =
            gPointLight.color.rgb * gPointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
             /// SpotLight
            
            // “üŽËŒõ‚Ì”½ŽËƒxƒNƒgƒ‹‚ÌŒvŽZ
			float3 reflectSpotLight = reflect(spotLightDirectionOnSurface, normalize(input.normal));
            
			float RdotESpotLight = dot(reflectSpotLight, toEye);
			float specularPowSpotLight = pow(saturate(RdotESpotLight), gMaterial.shininess);
            
			float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
			float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);
            
			float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
			float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
            
            // ŠgŽU”½ŽË
			float3 diffuseSpotLight =
            gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cosSpotLight * gSpotLight.intensity * factorSpotLight * falloffFactor;
            
            // ‹¾–Ê”½ŽË
			float3 specularSpotLight =
            gSpotLight.color.rgb * gSpotLight.intensity * factorSpotLight * falloffFactor * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // ŠgŽU”½ŽË + ‹¾–Ê”½ŽË LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // ƒ¿
			output.color.a = gMaterial.color.a * textureColor.a;
            
		} else if (gMaterial.enableBlinnPhongReflection == 1) {
             /*=================================================================================================*/
            ///
            ///                                 BlinnPhongReflection
            ///
            /*==================================================================================================*/
            
            // texture‚Ìƒ¿’l‚ª0.5fˆÈ‰º‚ÌŽž‚ÉPixel‚ðŠü‹p
			if (textureColor.a <= 0.5f) {
            
				discard;
			}
            // texture‚Ìƒ¿’l‚ª0.0f‚ÌŽž‚ÉPixel‚ðŠü‹p
			if (textureColor.a == 0.0f) {
            
				discard;
			}

            // PointLight‚Ì“üŽËŒõ
			float3 pointLightDirection = normalize(input.worldPosition - gPointLight.pos);
            
            // PointLight‚Ö‚Ì‹——£
			float distancePointLight = length(gPointLight.pos - input.worldPosition);
            // Žw”‚É‚æ‚éƒRƒ“ƒgƒ[ƒ‹
			float factorPointLight = pow(saturate(-distancePointLight / gPointLight.radius + 1.0f), gPointLight.decay);
            
            // SpotLight‚Ì“üŽËŒõ
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.pos);
            
            // SpotLight‚Ö‚Ì‹——£
			float distanceSpotLight = length(gSpotLight.pos - input.worldPosition);
            // Žw”‚É‚æ‚éƒRƒ“ƒgƒ[ƒ‹
			float factorSpotLight = pow(saturate(-distanceSpotLight / gSpotLight.distance + 1.0f), gSpotLight.decay);
            
            // Camera•ûŒüŽZo
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight

			float3 halfVectorDirectionalLight = normalize(normalize(-gDirectionalLight.direction) + toEye);
			float NDotHDirectionalLight = dot(normalize(input.normal), halfVectorDirectionalLight);
			float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // ŠgŽU”½ŽË
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cosDirectionalLight * gDirectionalLight.intensity;
            
            // ‹¾–Ê”½ŽË
			float3 specularDirectionalLight =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
			float3 halfVectorPointLight = normalize(-pointLightDirection + toEye);
			float NDotHPointLight = dot(normalize(input.normal), halfVectorPointLight);
			float specularPowPointLight = pow(saturate(NDotHPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // ŠgŽU”½ŽË
			float3 diffusePointLight =
            gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cosPointLight * gPointLight.intensity * factorPointLight;
            
            // ‹¾–Ê”½ŽË
			float3 specularPointLight =
            gPointLight.color.rgb * gPointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
             /// SpotLight
            
			float3 halfVectorSpotLight = normalize(-spotLightDirectionOnSurface + toEye);
			float NDotHSpotLight = dot(normalize(input.normal), halfVectorSpotLight);
			float specularPowSpotLight = pow(saturate(NDotHSpotLight), gMaterial.shininess);
            
			float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
			float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);
            
			float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
			float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
            
            // ŠgŽU”½ŽË
			float3 diffuseSpotLight =
            gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cosSpotLight * gSpotLight.intensity * falloffFactor * factorSpotLight;
            
            // ‹¾–Ê”½ŽË
			float3 specularSpotLight =
            gSpotLight.color.rgb * gSpotLight.intensity * falloffFactor * factorSpotLight * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // ŠgŽU”½ŽË + ‹¾–Ê”½ŽË LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // ƒ¿
			output.color.a = gMaterial.color.a * textureColor.a;
		}
	}
    /*=================================================================================================*/
    ///
    ///                                    Lighting–³‚µ
    ///
   /*=================================================================================================*/
	else {
        
        // texture‚Ìƒ¿’l‚ª0.5fˆÈ‰º‚ÌŽž‚ÉPixel‚ðŠü‹p
		if (textureColor.a <= 0.5f) {
            
			discard;
		}
        // texture‚Ìƒ¿’l‚ª0.0f‚ÌŽž‚ÉPixel‚ðŠü‹p
		if (textureColor.a == 0.0f) {
            
			discard;
		}
        
		output.color = gMaterial.color * textureColor;
	}
    
     // output‚Ìƒ¿’l‚ª0.0f‚ÌŽž‚ÉPixel‚ðŠü‹p
	if (output.color.a == 0.0f) {
            
		discard;
	}
    
	return output;
}