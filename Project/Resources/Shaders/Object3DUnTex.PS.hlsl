#include "Object3D.hlsli"

/*===========================================================
                     Object3D PS Shader
===========================================================*/

/// マテリアル構造体
struct Material {
    
	float4 color; // オブジェクトの色
	int enableLighting; // ライティングの有無
	int enableHalfLambert; // ハ－フランバートの有無
	int enablePhongReflection; // フォン反射の有無
	int enableBlinnPhongReflection; // ブリン-フォン反射の有無
	float shininess; // 輝度
	float3 specularColor; // 鏡面反射色
	float4x4 uvTransform; // UVTransform
};

/// 平行光源
struct DirectionalLight {

	float4 color; // 色
	float3 direction; // 向き
	float intensity; // 輝度 強さ
};

/// ポイントライト
struct PointLight {
	
	float4 color; // 色
	float3 pos; // 座標
	float intensity; // 輝度 強さ
	float radius; // 半径,サイズ
	float decay; // 減衰率
};

/// スポットライト
struct SpotLight {

	float4 color; // 色
	float3 pos; // 座標
	float intensity; // 輝度 強さ
	float3 direction; // 向き
	float distance; // 距離
	float decay; // 減衰率
	float cosAngle; // 回転角
	float cosFalloffStart; // 初期回転角
};

// 全てのライト
struct PunctualLight {
	
	DirectionalLight directionalLight; // 平行光源
	PointLight pointLight; // ポイントライト 本当はpointと書きたいがpointは既存と競合して使えない。しかたなく他も合わせる
	SpotLight spotLight; // スポットライト
};

// カメラ
struct Camera {

	float3 worldPosition;
};

struct PixelShaderOutput {
    
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0); // レジスタ 0
ConstantBuffer<PunctualLight> gPunctual : register(b1); // レジスタ 1
ConstantBuffer<Camera> gCamera : register(b2); // レジスタ 2

PixelShaderOutput main(VertexShaderOutput input) {
    
	PixelShaderOutput output;
	
	/*==================================================================================================================*/
	
	//									↓　Lightingの有無、色の設定 ↓
	
	/*==================================================================================================================*/
    
	// Lightingするかどうか、しないならそのままの色を出力する
	if (gMaterial.enableLighting == 1) {
		
		// HalfLambert適応、基本これでいい
		if (gMaterial.enableHalfLambert == 1) {

			float NdotL = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
			
			// rgb
			output.color.rgb = gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cos * gPunctual.directionalLight.intensity;
        
			// α値
			output.color.a = gMaterial.color.a;
		}
		
		// HalfLambertなし、ただのLambert反射
		else {
            
			float cos = saturate(dot(normalize(input.normal), -gPunctual.directionalLight.direction));
        
			// rgb
			output.color.rgb =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cos * gPunctual.directionalLight.intensity;
        
			// α値
			output.color.a = gMaterial.color.a;
		}
		
		// PhongReflection適応 使い分けは自由
		if (gMaterial.enablePhongReflection == 1) {
			
			// PointLightの入射光
			float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
            
            // PointLightへの距離
			float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
            // 指数によるコントロール
			float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
            
            // SpotLightの入射光
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
            
            // SpotLightへの距離
			float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
            // 指数によるコントロール
			float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
            
            // Camera方向算出
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight
            
            // 入射光の反射ベクトルの計算
			float3 reflectDirectionalLight = reflect(normalize(gPunctual.directionalLight.direction), normalize(input.normal));
            
			float RdotEDirectionalLight = dot(reflectDirectionalLight, toEye);
			float specularPowDirectionalLight = pow(saturate(RdotEDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // 拡散反射
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;
            
            // 鏡面反射
			float3 specularDirectionalLight =
            gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
            // 入射光の反射ベクトルの計算
			float3 reflectPointLight = reflect(pointLightDirection, normalize(input.normal));
            
			float RdotEPointLight = dot(reflectPointLight, toEye);
			float specularPowPointLight = pow(saturate(RdotEPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // 拡散反射
			float3 diffusePointLight =
            gMaterial.color.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;
            
            // 鏡面反射
			float3 specularPointLight =
            gPunctual.pointLight.color.rgb * gPunctual.pointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
             /// SpotLight
            
            // 入射光の反射ベクトルの計算
			float3 reflectSpotLight = reflect(spotLightDirectionOnSurface, normalize(input.normal));
            
			float RdotESpotLight = dot(reflectSpotLight, toEye);
			float specularPowSpotLight = pow(saturate(RdotESpotLight), gMaterial.shininess);
            
			float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
			float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);
            
			float cosAngle = dot(spotLightDirectionOnSurface, gPunctual.spotLight.direction);
			float falloffFactor = saturate((cosAngle - gPunctual.spotLight.cosAngle) / (gPunctual.spotLight.cosFalloffStart - gPunctual.spotLight.cosAngle));
            
            // 拡散反射
			float3 diffuseSpotLight =
            gMaterial.color.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * factorSpotLight * falloffFactor;
            
            // 鏡面反射
			float3 specularSpotLight =
            gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * factorSpotLight * falloffFactor * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // 拡散反射 + 鏡面反射 LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // α
			output.color.a = gMaterial.color.a;
			
		}
		
		// BlinnPhongReflection
		else if (gMaterial.enableBlinnPhongReflection == 1) {
			
			  // PointLightの入射光
			float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
            
            // PointLightへの距離
			float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
            // 指数によるコントロール
			float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
            
            // SpotLightの入射光
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
            
            // SpotLightへの距離
			float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
            // 指数によるコントロール
			float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
            
            // Camera方向算出
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight

			float3 halfVectorDirectionalLight = normalize(normalize(-gPunctual.directionalLight.direction) + toEye);
			float NDotHDirectionalLight = dot(normalize(input.normal), halfVectorDirectionalLight);
			float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // 拡散反射
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;
            
            // 鏡面反射
			float3 specularDirectionalLight =
            gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
			float3 halfVectorPointLight = normalize(-pointLightDirection + toEye);
			float NDotHPointLight = dot(normalize(input.normal), halfVectorPointLight);
			float specularPowPointLight = pow(saturate(NDotHPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // 拡散反射
			float3 diffusePointLight =
            gMaterial.color.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;
            
            // 鏡面反射
			float3 specularPointLight =
            gPunctual.pointLight.color.rgb * gPunctual.pointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
             /// SpotLight
            
			float3 halfVectorSpotLight = normalize(-spotLightDirectionOnSurface + toEye);
			float NDotHSpotLight = dot(normalize(input.normal), halfVectorSpotLight);
			float specularPowSpotLight = pow(saturate(NDotHSpotLight), gMaterial.shininess);
            
			float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
			float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);
            
			float cosAngle = dot(spotLightDirectionOnSurface, gPunctual.spotLight.direction);
			float falloffFactor = saturate((cosAngle - gPunctual.spotLight.cosAngle) / (gPunctual.spotLight.cosFalloffStart - gPunctual.spotLight.cosAngle));
            
            // 拡散反射
			float3 diffuseSpotLight =
            gMaterial.color.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight;
            
            // 鏡面反射
			float3 specularSpotLight =
            gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // 拡散反射 + 鏡面反射 LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // α
			output.color.a = gMaterial.color.a;
		}
	} else {

		output.color = gMaterial.color ;
	}
        
	return output;
}