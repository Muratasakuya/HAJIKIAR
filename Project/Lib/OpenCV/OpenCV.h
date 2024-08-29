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
#include <deque>
#include <limits>
#include <string>
#include <thread>
#include <iostream>
#include <algorithm>
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

	void Finalize();

	// singleton
	static OpenCV* GetInstance();

	// setter

	void SetEdgeSize(const Vector2& edgeSize);
	void SetGame3DMode(bool game3DMode);

	// getter

	std::string GetQRCodeData();
	Vector2 GetGreenLargestCenterPos() const;
	Vector2 GetSmallestGreenCenterPos() const;
	bool IsBlueHajikiFound() const;
	bool IsGreenHajikiFound() const;

	Vector2 GetTrackColorPos() const;
	bool FoundTrackColor() const;

	Vector2 GetOtherColorPos() const;
	bool FoundOtherColor() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// カメラ
	cv::VideoCapture camera_;

	int framerate_;

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
	Vector2 largestGreenCenter_;
	Vector2 smallestGreenCenter_;
	Vector2 blueCenter_;
	Vector2 prevGreenCenter_;
	Vector2 prevBlueCenter_;

	bool isFirstFrame_ = true;

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
	std::vector<cv::Point> toneCurvePoints_ = { {0, 0}, {128, 128}, {255, 255} };
	std::vector<cv::Point> toneCurvePointsR_ = { {0, 0}, {128, 128}, {255, 255} };
	std::vector<cv::Point> toneCurvePointsG_ = { {0, 0}, {128, 128}, {255, 255} };
	std::vector<cv::Point> toneCurvePointsB_ = { {0, 0}, {128, 128}, {255, 255} };
	cv::Mat processedFrame_;

	int exposure_;
	int kelvin_;
	bool whiteBalance_;
	float saturationScale_ = 1.0f;

	/*-------------------------------------------------------------------------------*/
	// New

	// 追跡する指標の色RGBA
	Vector3 trackColor1_;
	Vector3 trackColor2_;

	Vector2 trackColorCenter1_;
	Vector2 preTrackColorCenter1_;
	Vector2 trackColorCenter2_;
	Vector2 preTrackColorCenter2_;

	bool foundTrackColor1_;
	bool foundTrackColor2_;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	cv::Mat ConvertRGBtoHSV(const Vector3& color);
	void RenderToneCurveEditor(std::vector<cv::Point>& points, const char* label);
	void ApplyToneCurve(const cv::Mat& inputImage, cv::Mat& outputImage, const std::vector<cv::Point>& toneCurvePoints);
	void ApplyToneCurveRGB(const cv::Mat& inputImage, cv::Mat& outputImage, const std::vector<cv::Point>& toneCurvePointsR,
		const std::vector<cv::Point>& toneCurvePointsG, const std::vector<cv::Point>& toneCurvePointsB);
	void IncreaseSaturation(cv::Mat& frame, float saturationScale);
	Vector2 NormalizeCoordinates(const cv::Point& center);
};