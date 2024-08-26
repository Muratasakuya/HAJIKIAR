#pragma once

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// Lib
#include "Vector2.h"
#include "Vector3.h"

// c++
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

/*////////////////////////////////////////////////////////////////////////////////
*									OpenCV Class
////////////////////////////////////////////////////////////////////////////////*/
class OpenCV {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	OpenCV() = default;
	~OpenCV() = default;

	void OpenCamera();
	void QRTracking(const std::vector<std::string>& qrCodeDataList);
	void ColorTracking();

	void Initialize();
	void Update();
	void Draw();

	void Finalize();

	// singleton
	static OpenCV* GetInstance();

	// getter

	std::string GetQRCodeData();
	Vector2 GetGreenCenterPos() const;
	Vector2 GetBlueCenterPos() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// カメラ
	cv::VideoCapture camera_;

	// カメラから取得するフレーム
	cv::Mat frame_;
	// BGR色空間からHSV色空間に変換したフレームの保持
	cv::Mat hsvFrame_;

	// QRコード検知器
	cv::QRCodeDetector qrDecoder_;

	// デコード結果を保持する変数
	std::string decodedText_;
	// 複数のQRコードデータを保持するベクター
	std::vector <std::string> qrCodeData_;

	// ゲームシーンに渡す座標
	Vector2 greenCenter_;
	Vector2 blueCenter_;
	Vector2 prevGreenCenter_;
	Vector2 prevBlueCenter_;

	Vector3 trackColor_;
	Vector3 trackColor2_;

	int colorRange_;

	bool isFirstFrame_ = true;

	std::vector<cv::Point2f> blackBorderPoints_;
	bool isBlackBorderDetected_ = false;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	cv::Mat ConvertRGBtoHSV(const Vector3& color);
	bool DetectAndCorrectBlackBorder(const cv::Mat& inputFrame, cv::Mat& outputFrame);

};