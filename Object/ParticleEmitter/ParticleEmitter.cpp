#include "ParticleEmitter.h"

#include "ImGuiManager.h"



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
ParticleEmitter::ParticleEmitter() {

	// インスタンス代入
	particleManager_ = ParticleManager::GetInstance();

	// エミッタ情報の初期化
	emitter_.count = 3;
	// 0.5秒置きに発生
	emitter_.frequency = 0.5f;
	// 発生頻度用の時刻
	emitter_.frequencyTime = 0.0f;
	// 座標
	emitter_.transform.translate = { 0.0f,0.0f,0.0f };

	// デフォルト
	std::string useName = "circle";
	name_.push_back(useName);

	isAddParticle_ = false;
}



/*////////////////////////////////////////////////////////////////////////////////
*								  エミット
////////////////////////////////////////////////////////////////////////////////*/
void ParticleEmitter::Emit() {

	for (size_t index = 0; index < name_.size(); index++) {

		std::string name = name_[index];

		particleManager_->Emit(name, emitter_.transform.translate, emitter_.count);
	}
}



/*////////////////////////////////////////////////////////////////////////////////
*								  更新処理
////////////////////////////////////////////////////////////////////////////////*/
void ParticleEmitter::Update() {

	// 時刻を進める
	emitter_.frequencyTime += kDeltaTime;

	// 発生頻度より大きいなら発生
	if (emitter_.frequency <= emitter_.frequencyTime) {

		// 発生
		Emit();

		// 余計に過ぎた時間を加味して頻度計算
		emitter_.frequencyTime -= emitter_.frequency;
	}

	if (!isAddParticle_) {
		ImGui::Begin("ParticleEmitter");

		if (ImGui::Button("AddParticle")) {

			particleManager_->CreateParticleGroup("uvChecker", "./Resources/Images/uvChecker.png");
			std::string addName = "uvChecker";
			name_.push_back(addName);

			isAddParticle_ = true;
		}

		ImGui::End();
	}

	// パーティクル更新
	particleManager_->Update();

	for (size_t index = 0; index < name_.size(); index++) {

		std::string name = name_[index];

		Engine::DrawParticle(name, name, kBlendModeAdd);
	}
}