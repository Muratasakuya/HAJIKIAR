#pragma once

// c++
#include <cstdint>

// パイプラインの種類
enum PipelineType {

	Primitive,      // 単色 テクスチャを使用しない
	Sprite2D,         // 2Dテクスチャ
	Normal,        // テクスチャ使用 Lightingの有無あり
	pParticle,      // パーティクル用
};

// ブレンドモード
enum BlendMode {

	kBlendModeNone,     // ブレンド無し
	kBlendModeNormal,   // 通常αブレンド
	kBlendModeAdd,      // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply,  // 乗算
	kBlendModeScreen,   // スクリーン
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 4;

// ブレンドモードの数
static const uint32_t blendModeNum = 6;