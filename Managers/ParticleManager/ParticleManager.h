#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <random>
#include <list>
#include <unordered_map>
#include <numbers>
#include <string>

class DXCommon;
class SrvManager;

static const uint32_t instanceMaxCount_ = 128;
// Δt
static const float kDeltaTime = 1.0f / 60.0f;

/*////////////////////////////////////////////////////////////////////////////////
*
*							ParticleManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class ParticleManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	struct ParticleGroup {

		std::string filePath;
		std::list<ParticleData> particles;
		uint32_t instancingSrvIndex;
		uint32_t numInstance;
		ComPtr<ID3D12Resource> instancingResource;
		ParticleForGPU* particleData = nullptr;
	};

	void CreateVertexData();

	void CreateParticleGroup(const std::string name, const std::string filePath);
	void Emit(const std::string name, const Vector3& pos, uint32_t count);

	void SetBufferData(ID3D12GraphicsCommandList* commandList, const std::string& name);
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, const std::string& name, const std::string& textureName);
	void DrawCall(ID3D12GraphicsCommandList* commandList, const std::string& name);

	void Initialize(DXCommon* dxCommon, SrvManager* srvManager);
	void Update();

	// singleton
	static ParticleManager* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	VertexResource vertexReourceInstance_;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;
	AccelerationField accelerationField_;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertexResource_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	// 頂点バッファデータ
	VertexData* vertexData_ = nullptr;
	// モデル頂点
	ModelData modelData_;

	ParticleManager() = default;
	~ParticleManager() = default;
	// コピー禁止
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

	bool IsCollision(const AABB& aabb, const Vector3& point);
};