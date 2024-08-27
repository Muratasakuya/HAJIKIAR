#pragma once

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// json
#include "JsonHandler.h"

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

	bool isFirstFrame_ = true;

	// 追跡する指標の色RGBA
	Vector3 trackColor_;
	Vector3 trackColor2_;
	// 色の藩士
	int colorRange_;

	// HAHJIKI

	// 画面サイズ
	Vector2 edgeSize_;
	bool game3DMode_ = false;

	bool isBlueHajikiFound_ = false;
	bool isGreenHajikiFound_ = false;

	/*---------------------------------------------------------*/

	// json
	std::unique_ptr<JsonHandler> jsonHandler_;

	// ROIの範囲を設定する変数
	// 4つの頂点の座標を保持する変数
	cv::Point point1_;
	cv::Point point2_;
	cv::Point point3_;
	cv::Point point4_;

	std::array<cv::Point, 4> points_;

	int exposure_;
	int kelvin_;
	bool whiteBalance_;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	cv::Mat ConvertRGBtoHSV(const Vector3& color);
};