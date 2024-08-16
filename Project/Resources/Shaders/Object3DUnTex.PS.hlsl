#include "Object3D.hlsli"

/*===========================================================
                     Object3D PS Shader
===========================================================*/

/// �}�e���A���\����
struct Material {
    
	float4 color; // �I�u�W�F�N�g�̐F
	int enableLighting; // ���C�e�B���O�̗L��
	int enableHalfLambert; // �n�|�t�����o�[�g�̗L��
	int enablePhongReflection; // �t�H�����˂̗L��
	int enableBlinnPhongReflection; // �u����-�t�H�����˂̗L��
	float shininess; // �P�x
	float3 specularColor; // ���ʔ��ːF
	float4x4 uvTransform; // UVTransform
};

/// ���s����
struct DirectionalLight {

	float4 color; // �F
	float3 direction; // ����
	float intensity; // �P�x ����
};

/// �|�C���g���C�g
struct PointLight {
	
	float4 color; // �F
	float3 pos; // ���W
	float intensity; // �P�x ����
	float radius; // ���a,�T�C�Y
	float decay; // ������
};

/// �X�|�b�g���C�g
struct SpotLight {

	float4 color; // �F
	float3 pos; // ���W
	float intensity; // �P�x ����
	float3 direction; // ����
	float distance; // ����
	float decay; // ������
	float cosAngle; // ��]�p
	float cosFalloffStart; // ������]�p
};

// �S�Ẵ��C�g
struct PunctualLight {
	
	DirectionalLight directionalLight; // ���s����
	PointLight pointLight; // �|�C���g���C�g �{����point�Ə���������point�͊����Ƌ������Ďg���Ȃ��B�������Ȃ��������킹��
	SpotLight spotLight; // �X�|�b�g���C�g
};

// �J����
struct Camera {

	float3 worldPosition;
};

struct PixelShaderOutput {
    
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0); // ���W�X�^ 0
ConstantBuffer<PunctualLight> gPunctual : register(b1); // ���W�X�^ 1
ConstantBuffer<Camera> gCamera : register(b2); // ���W�X�^ 2

PixelShaderOutput main(VertexShaderOutput input) {
    
	PixelShaderOutput output;
	
	/*==================================================================================================================*/
	
	//									���@Lighting�̗L���A�F�̐ݒ� ��
	
	/*==================================================================================================================*/
    
	// Lighting���邩�ǂ����A���Ȃ��Ȃ炻�̂܂܂̐F���o�͂���
	if (gMaterial.enableLighting == 1) {
		
		// HalfLambert�K���A��{����ł���
		if (gMaterial.enableHalfLambert == 1) {

			float NdotL = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
			
			// rgb
			output.color.rgb = gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cos * gPunctual.directionalLight.intensity;
        
			// ���l
			output.color.a = gMaterial.color.a;
		}
		
		// HalfLambert�Ȃ��A������Lambert����
		else {
            
			float cos = saturate(dot(normalize(input.normal), -gPunctual.directionalLight.direction));
        
			// rgb
			output.color.rgb =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cos * gPunctual.directionalLight.intensity;
        
			// ���l
			output.color.a = gMaterial.color.a;
		}
		
		// PhongReflection�K�� �g�������͎��R
		if (gMaterial.enablePhongReflection == 1) {
			
			// PointLight�̓��ˌ�
			float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
            
            // PointLight�ւ̋���
			float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
            // �w���ɂ��R���g���[��
			float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
            
            // SpotLight�̓��ˌ�
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
            
            // SpotLight�ւ̋���
			float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
            // �w���ɂ��R���g���[��
			float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
            
            // Camera�����Z�o
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight
            
            // ���ˌ��̔��˃x�N�g���̌v�Z
			float3 reflectDirectionalLight = reflect(normalize(gPunctual.directionalLight.direction), normalize(input.normal));
            
			float RdotEDirectionalLight = dot(reflectDirectionalLight, toEye);
			float specularPowDirectionalLight = pow(saturate(RdotEDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;
            
            // ���ʔ���
			float3 specularDirectionalLight =
            gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
            // ���ˌ��̔��˃x�N�g���̌v�Z
			float3 reflectPointLight = reflect(pointLightDirection, normalize(input.normal));
            
			float RdotEPointLight = dot(reflectPointLight, toEye);
			float specularPowPointLight = pow(saturate(RdotEPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
			float3 diffusePointLight =
            gMaterial.color.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;
            
            // ���ʔ���
			float3 specularPointLight =
            gPunctual.pointLight.color.rgb * gPunctual.pointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
             /// SpotLight
            
            // ���ˌ��̔��˃x�N�g���̌v�Z
			float3 reflectSpotLight = reflect(spotLightDirectionOnSurface, normalize(input.normal));
            
			float RdotESpotLight = dot(reflectSpotLight, toEye);
			float specularPowSpotLight = pow(saturate(RdotESpotLight), gMaterial.shininess);
            
			float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
			float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);
            
			float cosAngle = dot(spotLightDirectionOnSurface, gPunctual.spotLight.direction);
			float falloffFactor = saturate((cosAngle - gPunctual.spotLight.cosAngle) / (gPunctual.spotLight.cosFalloffStart - gPunctual.spotLight.cosAngle));
            
            // �g�U����
			float3 diffuseSpotLight =
            gMaterial.color.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * factorSpotLight * falloffFactor;
            
            // ���ʔ���
			float3 specularSpotLight =
            gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * factorSpotLight * falloffFactor * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // �g�U���� + ���ʔ��� LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // ��
			output.color.a = gMaterial.color.a;
			
		}
		
		// BlinnPhongReflection
		else if (gMaterial.enableBlinnPhongReflection == 1) {
			
			  // PointLight�̓��ˌ�
			float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
            
            // PointLight�ւ̋���
			float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
            // �w���ɂ��R���g���[��
			float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
            
            // SpotLight�̓��ˌ�
			float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
            
            // SpotLight�ւ̋���
			float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
            // �w���ɂ��R���g���[��
			float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
            
            // Camera�����Z�o
			float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            /*-------------------------------------------------------------------------------------------------*/
            /// DirectionalLight

			float3 halfVectorDirectionalLight = normalize(normalize(-gPunctual.directionalLight.direction) + toEye);
			float NDotHDirectionalLight = dot(normalize(input.normal), halfVectorDirectionalLight);
			float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);
            
			float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
			float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
			float3 diffuseDirectionalLight =
            gMaterial.color.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;
            
            // ���ʔ���
			float3 specularDirectionalLight =
            gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
            
             /*-------------------------------------------------------------------------------------------------*/
            /// PointLight
            
			float3 halfVectorPointLight = normalize(-pointLightDirection + toEye);
			float NDotHPointLight = dot(normalize(input.normal), halfVectorPointLight);
			float specularPowPointLight = pow(saturate(NDotHPointLight), gMaterial.shininess);
            
			float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
			float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
			float3 diffusePointLight =
            gMaterial.color.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;
            
            // ���ʔ���
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
            
            // �g�U����
			float3 diffuseSpotLight =
            gMaterial.color.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight;
            
            // ���ʔ���
			float3 specularSpotLight =
            gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight * specularPowSpotLight * gMaterial.specularColor;
            
            /*-------------------------------------------------------------------------------------------------*/
            
            // �g�U���� + ���ʔ��� LightDirecion + PointLight + SpotLight
			output.color.rgb =
            diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
            
            // ��
			output.color.a = gMaterial.color.a;
		}
	} else {

		output.color = gMaterial.color ;
	}
        
	return output;
}