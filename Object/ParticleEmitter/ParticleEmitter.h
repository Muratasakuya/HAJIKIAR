#pragma once

// base
#include "Engine.h"

// Manager
#include "ParticleManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*							ParticleEmitter Class
*
////////////////////////////////////////////////////////////////////////////////*/
class ParticleEmitter {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	ParticleEmitter();
	~ParticleEmitter() {};

	void Emit();
	void Update();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ParticleManager* particleManager_ = nullptr;

	Emitter emitter_{};

	std::vector<std::string> name_;

	bool isAddParticle_;

};