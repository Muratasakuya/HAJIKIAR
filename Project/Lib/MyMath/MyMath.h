#pragma once

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 数学関数クラス
/// </summary>
struct MyMath {

	/*-------------------------------------------------------------*/
	/// 関数

	// ノルム
	static float Length(float a, float b);

	// 正規化
	static float Normalize(float a, float b);

};