#include "MyMath.h"

/*-------------------------------------------------------------*/
/// 関数

// ノルム
float MyMath::Length(float a, float b) {

	return std::sqrtf(a * a + b * b);
}

// 正規化
float MyMath::Normalize(float a, float b) {

	if (b != 0) {
		return a / b;
	} else {
		return a;
	}
}
