#pragma once

// c++
#include <cstdint>

// パイプラインの種類
enum PipelineType {

	Primitive,      // 単色 テクスチャを使用しない
	Object2D,       // 2Dテクスチャ
	pObject3D,      // テクスチャ使用 Lightingの有無あり
	Object3DUnTex,  // マテリアルのみ Lightingの有無あり
	pParticle,      // パーティクル用
};

// ブレンドモード
enum BlendMode {

	kBlendModeNone,     // ブレンド無し
	kBlendModeNormal,   // 通常αブレンド
	kBlendModeAdd,      // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply, // 乗算
	kBlendModeScreen,   // スクリーン
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 5;

// ブレンドモードの数
static const uint32_t blendModeNum = 6;