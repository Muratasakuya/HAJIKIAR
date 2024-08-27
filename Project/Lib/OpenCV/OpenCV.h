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
#include <thread>
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
	void ColorTracking(cv::Mat& frame);

	void CropAndShowPolygonRegion(const std::array<cv::Point, 4>& points);

	void Initialize();
	void Update();
	void Draw();

	void Finalize();

	// singleton
	static OpenCV* GetInstance();

	// setter

	void SetEdgeSize(const Vector2& edgeSize);
	void SetGame3DMode(bool game3DMode);

	// getter

	std::string GetQRCodeData();
	Vector2 GetGreenCenterPos() const;
	Vector2 GetBlueCenterPos() const;
	bool IsBlueHajikiFound() const;
	bool IsGreenHajikiFound() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Vector2 edgeSize_;
	bool game3DMode_ = false;

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
	// 座標が取れなかったときの座標 pre
	Vector2 greenPreCenter_;
	Vector2 bluePreCenter_;

	Vector2 prevGreenCenter_;
	Vector2 prevBlueCenter_;

	Vector3 trackColor_;
	Vector3 trackColor2_;

	int colorRange_;

	bool isFirstFrame_ = true;

	std::vector<cv::Point2f> blackBorderPoints_;
	bool isBlackBorderDetected_ = false;

	// HAHJIKI

	bool isBlueHajikiFound_ = false;
	bool isGreenHajikiFound_ = false;

	int exposure_;
	int kelvin_;

	/*---------------------------------------------------------*/

	// ROIの範囲を設定する変数
	// 4つの頂点の座標を保持する変数
	cv::Point point1_ = { 137, 100 };
	cv::Point point2_ = { 540, 90 };
	cv::Point point3_ = { 559, 322 };
	cv::Point point4_ = { 137, 341 };
private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	cv::Mat ConvertRGBtoHSV(const Vector3& color);
};