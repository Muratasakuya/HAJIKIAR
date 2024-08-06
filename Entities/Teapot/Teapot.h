#pragma once

// Base
#include "Engine.h"

// 構造体
#include "CBufferStructure.h"

// c++
#include <numbers>
#include <string>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*
*									Teapot Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Teapot {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Teapot() {};
	~Teapot() {};

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

	// Modelの名前
	std::string modelName_{};
	// Textureの名前
	std::string textureName_{};

};