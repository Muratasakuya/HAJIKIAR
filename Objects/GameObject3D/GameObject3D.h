#pragma once

// Engine
#include "Engine.h"

// 構造体
#include "CBufferStructure.h"

// c++
#include <string>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*								GameObject3D Class
////////////////////////////////////////////////////////////////////////////////*/
class GameObject3D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameObject3D();
	~GameObject3D();

	void Initialize();
	void Update();
	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	/*--------------------------------------------------------------*/
	// メインオブジェクト

	// SRT
	Transform transform_;

	// マテリアル
	Material material_;

	// ライト
	PunctualLight light_;

	/*--------------------------------------------------------------*/
	// サブオブジェクト

	// 使用するテクスチャの名前
	std::string textureName_;
	// 使用するモデルの名前
	std::string modelName_;

};