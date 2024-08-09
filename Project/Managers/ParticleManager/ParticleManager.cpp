#include "ParticleManager.h"

#include "DXCommon.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "MainCamera3D.h"

/*////////////////////////////////////////////////////////////////////////////////
*								衝突判定
////////////////////////////////////////////////////////////////////////////////*/
bool ParticleManager::IsCollision(const AABB& aabb, const Vector3& point) {

	// ポイントがAABBの範囲内にあるかをチェック
	if ((point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z)) {

		return true;
	}
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								頂点データの生成
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::CreateVertexData() {

	HRESULT hr;

	// 頂点データの作成 自作
	//左上
	modelData_.vertices.push_back(
		{ .pos = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	// 右上
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	// 左下
	modelData_.vertices.push_back(
		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
	// 左下
	modelData_.vertices.push_back(
		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
	// 右上
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	// 右下
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });

	// 頂点データサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * modelData_.vertices.size());

	// 頂点バッファ生成
	vertexResource_ = vertexReourceInstance_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);

	// 頂点バッファビューの生成
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeVB;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点データのマッピング
	hr = vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	assert(SUCCEEDED(hr));
}

/*////////////////////////////////////////////////////////////////////////////////
*									   初期化
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::Initialize(DXCommon* dxCommon, SrvManager* srvManager,TextureManager* textureManager) {

	assert(dxCommon);
	assert(srvManager);
	assert(textureManager);

	// インスタンス代入
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	textureManager_ = textureManager;

	// ランダムエンジン初期化
	// 処理保留


	// 頂点データの初期化
	CreateVertexData();

	// 加速フィールドの初期化
	accelerationField_.acceleration = { 15.0f,0.0f,0.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

}

/*////////////////////////////////////////////////////////////////////////////////
*								パーティクルグループの生成
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::CreateParticleGroup(const std::string name, const std::string filePath) {

	// 乱数の生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	HRESULT hr;

	// 登録済みの名前かチェックしてassert
	assert(particleGroups_.find(name) == particleGroups_.end() && "Particle group with this name already exists");

	// 新たな(空)のパーティクルグループを作成し、コンテナに登録
	ParticleGroup newGroup;
	for (uint32_t i = 0; i < 3; i++) {

		ParticleData particle;
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 1.0f, 1.0f, 1.0f };
		particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
		particle.color = { distColor(randomEngine), distColor(randomEngine), distColor(randomEngine), 1.0f };
		particle.lifeTime = distTime(randomEngine);
		particle.currentTime = 0.0f;
		Vector3 randomTranslate = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
		particle.transform.translate = randomTranslate;

		newGroup.particles.push_back(particle);
	}
	particleGroups_[name] = newGroup;
	auto& group = particleGroups_[name];

	// 新たなパーティクルグループの
	//1.マテリアルデータにテクスチャのファイルパスを指定
	group.filePath = filePath;
	//2.テクスチャを読み込む
	textureManager_->LoadTexture(group.filePath);
	//3.マテリアルデータにテクスチャのSRVインデックスを記録
	//4.インスタンシング用のリソースの生成
	group.numInstance = instanceMaxCount_;
	group.instancingResource = vertexReourceInstance_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(ParticleForGPU) * group.numInstance);
	hr = group.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group.particleData));
	assert(SUCCEEDED(hr));
	//5.インスタンシング用にSRVを確保してインデックスを記録
	group.instancingSrvIndex = srvManager_->Allocate();
	//6.SRV生成
	srvManager_->CreateSRVForStructureBuffer(
		group.instancingSrvIndex, group.instancingResource.Get(), group.numInstance, sizeof(ParticleForGPU));
}

/*////////////////////////////////////////////////////////////////////////////////
*								パーティクル発生装置
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::Emit(const std::string name, const Vector3& pos, uint32_t count) {

	// 乱数の生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	// 既存のパーティクルグループを検索
	auto it = particleGroups_.find(name);

	if (it != particleGroups_.end()) {

		ParticleGroup& group = it->second;

		for (uint32_t i = 0; i < count; i++) {

			ParticleData particle;
			particle.transform.scale = { 1.0f, 1.0f, 1.0f };
			particle.transform.rotate = { 1.0f, 1.0f, 1.0f };
			particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
			particle.color = { distColor(randomEngine), distColor(randomEngine), distColor(randomEngine), 1.0f };
			particle.lifeTime = distTime(randomEngine);
			particle.currentTime = 0.0f;
			Vector3 randomTranslate = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
			particle.transform.translate = pos + randomTranslate;

			group.particles.push_back(particle);
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									更新処理
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::Update() {

	MainCamera3D* camera3D = MainCamera3D::GetInstance();

	// ビルボード行列の計算
	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Matrix4x4::Multiply(backToFrontMatrix, camera3D->GetCameraMatrix());
	// 平行移動成分はなし
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// 全てのパーティクルグループについて処理する
	for (auto& [name, group] : particleGroups_) {
		// グループ内のパーティクルを処理するためのリスト
		auto& particles = group.particles;

		// パーティクルごとに処理を行う
		for (auto it = particles.begin(); it != particles.end();) {

			// 1.寿命に達していたらグループから外す
			if (it->lifeTime <= it->currentTime) {

				it = particles.erase(it);
				continue;
			}

			// 2.場の影響を計算
			// 衝突していたら加速させる
			if (IsCollision(accelerationField_.area, it->transform.translate)) {

				it->velocity +=
				{accelerationField_.acceleration.x* kDeltaTime,
					accelerationField_.acceleration.y* kDeltaTime,
					accelerationField_.acceleration.z* kDeltaTime};
			}

			// 3.移動処理
			// 速度を適用
			it->transform.translate += {

				it->velocity.x* kDeltaTime,
					it->velocity.y* kDeltaTime,
					it->velocity.z* kDeltaTime
			};

			// 4.経過時間を加算
			it->currentTime += kDeltaTime;

			// 5.ワールド行列を計算
			Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(it->transform.scale);
			Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(it->transform.translate);
			Matrix4x4 worldMatrix =
				Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(billboardMatrix, translateMatrix));

			// 6.WVP合成
			Matrix4x4 wvpMatrix =
				Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera3D->GetViewMatrix(), camera3D->GetProjectionMatrix()));

			float alpha = 1.0f - (it->currentTime / it->lifeTime);

			// 7.インスタンシング用データ1個分の書き込み
			int index = static_cast<uint32_t>(std::distance(particles.begin(), it));
			group.particleData[index].World = worldMatrix;
			group.particleData[index].WVP = wvpMatrix;
			group.particleData[index].color = it->color;
			group.particleData[index].color.w = alpha;

			++it;
		}

		// インスタンシング用のデータの数を更新
		group.numInstance = static_cast<uint32_t>(particles.size());
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								頂点バッファのセット
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::SetBufferData(ID3D12GraphicsCommandList* commandList, const std::string& name) {

	// 頂点データ転送
	std::memcpy(vertexData_,
		modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// particle用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, particleGroups_[name].instancingResource.Get()->GetGPUVirtualAddress());
}

/*////////////////////////////////////////////////////////////////////////////////
*									SRVのセット
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::SetGraphicsRootDescriptorTable(
	ID3D12GraphicsCommandList* commandList, const std::string& name, const std::string& textureName) {

	textureManager_->SetGraphicsRootDescriptorTable(commandList, 1, textureName);
	commandList->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(particleGroups_[name].instancingSrvIndex));
}

/*////////////////////////////////////////////////////////////////////////////////
*									パーティクル描画
////////////////////////////////////////////////////////////////////////////////*/
void ParticleManager::DrawCall(ID3D12GraphicsCommandList* commandList, const std::string& name) {

	commandList->DrawInstanced(UINT(modelData_.vertices.size()), particleGroups_[name].numInstance, 0, 0);
}