#pragma once

// Base
#include "Engine.h"

// 構造体
#include "CBufferStructure.h"

// c++
#include <string>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*
*									Sphere Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Sphere {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Sphere() {};
	~Sphere() {};

	void Initialize();
	void Update();
	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// SRT
	Transform transform_{};

	// Material
	Material material_{};

	// Light
	PunctualLight light_{};

	// Textureの名前
	std::string textureName_{};

};