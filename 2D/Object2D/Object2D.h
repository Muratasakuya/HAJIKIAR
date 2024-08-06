#pragma once

// DirectX
#include "PipelineStateStructure.h"
#include "CBufferStructure.h"

// c++
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*								Object2D Class
////////////////////////////////////////////////////////////////////////////////*/
class Object2D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Object2D();
	~Object2D();

	void Initialize();

	void UpdateImGui(const std::string& name);
	void Update();

	void Draw(const std::string textureName,BlendMode blendMode);

	// setter

	void SetPos(const Vector2& pos);
	void SetSize(const Vector2& size);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Transform2D transform2D_{};
	Vector4 color_{};

};