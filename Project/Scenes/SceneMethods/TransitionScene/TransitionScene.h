#pragma once

// 2D
#include "GameObject2D.h"

// c++
#include <memory>
#include <string>

enum class TransitionType {

	Fade,
	Other /// まだない
};

/*////////////////////////////////////////////////////////////////////////////////
*								TitleScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TransitionScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	TransitionScene();
	~TransitionScene() = default;

	void Fade();

	void Draw();

	// setter

	void SetType(const TransitionType& type);

	// getter

	bool FadeOutFinished() const;
	bool FadeInFinished() const;
	float GetWaitTime() const;

public:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	TransitionType type_;

	/*----------------------------------------------------------------------*/
	// フェード

	std::unique_ptr<GameObject2D> fade_;

	float fadeWaitTime_;
	float fadeAlpha_;
	bool fadeOutFinished_;
	bool fadeInFinished_;

};