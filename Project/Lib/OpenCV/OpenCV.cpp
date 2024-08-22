#include "OpenCV.h"

#include "NewMoon.h"
#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								singleton
////////////////////////////////////////////////////////////////////////////////*/
OpenCV* OpenCV::GetInstance() {

	static OpenCV instance;

	return &instance;
}

// 複数のQRコードデータを取得
std::string OpenCV::GetQRCodeData() {

	if (!qrCodeData_.empty()) {

		// 最新のデータを取得
		std::string data = qrCodeData_.back();

		// 取得したデータを削除
		qrCodeData_.pop_back();

		return data;
	}
	return "";
}

// greenCenter_ getter
Vector2 OpenCV::GetGreenCenterPos() const {

	return greenCenter_;
}

// blueCenter_ getter
Vector2 OpenCV::GetBlueCenterPos() const {

	return blueCenter_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								RGBからHSV色空間に変換
////////////////////////////////////////////////////////////////////////////////*/
cv::Mat OpenCV::ConvertRGBtoHSV(const Vector3& color) {

	// RGB値をMatに変換（1x1の画像として扱う）
	cv::Mat rgbMat(1, 1, CV_8UC3, cv::Scalar(color.z, color.y, color.x));
	cv::Mat hsvMat;

	// RGBからHSVに変換
	cv::cvtColor(rgbMat, hsvMat, cv::COLOR_BGR2HSV);

	// HSV値を取り出す
	return hsvMat;
}

/*////////////////////////////////////////////////////////////////////////////////
*								  カメラ起動
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::OpenCamera() {

	// カメラ起動
	camera_.open(0);

	// カメラが開けなければエラー
	if (!camera_.isOpened()) {

		throw std::runtime_error("Error: Camera could not be opened.");
	}

	// カメラウィンドウのサイズ
	camera_.set(cv::CAP_PROP_FRAME_WIDTH, 640);  // 横幅
	camera_.set(cv::CAP_PROP_FRAME_HEIGHT, 360); // 縦幅
	camera_.set(cv::CAP_PROP_FPS, 60);           // フレームレート (FPS)

	trackColor_ = { 0.0f, 1.0f, 0.0f };
	trackColor2_ = { 0.0f, 0.0f, 1.0f };

	colorRange_ = 10;
}

/*////////////////////////////////////////////////////////////////////////////////
*							QRコードの複数トラッキング
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::QRTracking(const std::vector<std::string>& qrCodeDataList) {

	// カメラが開かれていなければ早期リターン
	if (!camera_.isOpened()) {

		return;
	}

	// カメラから画像データを読んでframeに送る
	camera_ >> frame_;

	// 何も取得できなければ早期リターン
	if (frame_.empty()) {

		return;
	}

	// 取得したフレームの左右反転
	cv::flip(frame_, frame_, 1);

	// ガウシアンブラー
	cv::GaussianBlur(frame_, frame_, cv::Size(5, 5), 0);

	// QRコードの検出とデコード
	std::vector<cv::Point> points;
	std::vector<cv::String> decodedTexts;
	qrDecoder_.detectAndDecodeMulti(frame_, decodedTexts, points);

	// 検出された各QRコードについて処理
	for (size_t i = 0; i < decodedTexts.size(); i++) {

		std::string decodedText = decodedTexts[i];
		std::vector<cv::Point> pointSet(points.begin() + i * 4, points.begin() + (i + 1) * 4);

		// デコードしたQRコードがリストにあるか
		if (std::find(qrCodeDataList.begin(), qrCodeDataList.end(), decodedText) != qrCodeDataList.end()) {

			// QRコードの中心座標取得
			cv::Point center(0, 0);
			for (const auto& point : pointSet) {

				center += point;
			}

			center.x /= static_cast<int>(pointSet.size());
			center.y /= static_cast<int>(pointSet.size());

			// 中心座標にマークを描画
			cv::circle(frame_, center, 10, cv::Scalar(0, 255, 0), 2);
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							色のついたものの複数トラッキング
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::ColorTracking() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("OpenCV");

	ImGui::ColorEdit3("trackColor", &trackColor_.x);
	ImGui::ColorEdit3("trackColor2", &trackColor2_.x);
	ImGui::DragInt("colorRange", &colorRange_, 1, 0, 100);

	ImGui::End();

	/*======================================================*/

#pragma region /// method ///

	// フレームをHSV色空間に変換
	cv::Mat hsvFrame;
	cv::cvtColor(frame_, hsvFrame, cv::COLOR_BGR2HSV);

	// RGBからHSV色空間に変換
	cv::Mat hsvColor = ConvertRGBtoHSV(trackColor_);
	cv::Mat hsvColor2 = ConvertRGBtoHSV(trackColor2_);

	// HSV値を取得
	cv::Vec3b hsvValues = hsvColor.at<cv::Vec3b>(0, 0);
	cv::Vec3b hsvValues2 = hsvColor2.at<cv::Vec3b>(0, 0);

	// 緑色の範囲を定義
	cv::Mat greenMask;
	cv::inRange(hsvFrame, cv::Scalar(hsvValues[0] - colorRange_, 100, 100), cv::Scalar(hsvValues[0] + colorRange_, 255, 255), greenMask);

	// 青色の範囲を定義
	cv::Mat blueMask;
	cv::inRange(hsvFrame, cv::Scalar(hsvValues2[0] - colorRange_, 100, 100), cv::Scalar(hsvValues2[0] + colorRange_, 255, 255), blueMask);

	// モルフォロジー変換（開閉処理）でノイズ除去
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(greenMask, greenMask, cv::MORPH_CLOSE, kernel);
	cv::morphologyEx(greenMask, greenMask, cv::MORPH_OPEN, kernel);

	cv::morphologyEx(blueMask, blueMask, cv::MORPH_CLOSE, kernel);
	cv::morphologyEx(blueMask, blueMask, cv::MORPH_OPEN, kernel);

	// マスクの処理
	cv::Mat green_result, blue_result;
	cv::bitwise_and(frame_, frame_, green_result, greenMask);
	cv::bitwise_and(frame_, frame_, blue_result, blueMask);
#pragma endregion

	// 緑色の物体の輪郭を検出
	std::vector<std::vector<cv::Point>> green_contours;
	cv::findContours(greenMask, green_contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// 青色の物体の輪郭を検出
	std::vector<std::vector<cv::Point>> blue_contours;
	cv::findContours(blueMask, blue_contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// 緑色の物体の中心座標を計算
	Vector2 currentGreenCenter{};
	for (size_t i = 0; i < green_contours.size(); ++i) {
		cv::Moments m = cv::moments(green_contours[i]);
		double area = cv::contourArea(green_contours[i]);

		// 面積が十分大きく、m.m00がゼロでない場合のみ処理
		if (area > 100 && m.m00 != 0) {
			cv::Point center(
				static_cast<int>(m.m10 / m.m00),
				static_cast<int>(m.m01 / m.m00));

			// 座標の正規化
			float normalizedX = static_cast<float>(center.x) / 640.0f;
			float normalizedY = static_cast<float>(center.y) / 360.0f;

			currentGreenCenter = { normalizedX * NewMoon::kWindowWidthf, normalizedY * NewMoon::kWindowHeightf };

			// 最初に見つけた中心だけを取得
			break;
		}
	}

	// 青色の物体の中心座標を計算
	Vector2 currentBlueCenter{};
	for (size_t i = 0; i < blue_contours.size(); ++i) {
		cv::Moments m = cv::moments(blue_contours[i]);
		double area = cv::contourArea(blue_contours[i]);

		// 面積が十分大きく、m.m00がゼロでない場合のみ処理
		if (area > 100 && m.m00 != 0) {
			cv::Point center(
				static_cast<int>(m.m10 / m.m00),
				static_cast<int>(m.m01 / m.m00));

			// 座標の正規化
			float normalizedX = static_cast<float>(center.x) / 640.0f;
			float normalizedY = static_cast<float>(center.y) / 360.0f;

			currentBlueCenter = { normalizedX * NewMoon::kWindowWidthf, normalizedY * NewMoon::kWindowHeightf };

			// 最初に見つけた中心だけを取得
			break;
		}
	}

	// フレーム間の追跡の平滑化
	if (isFirstFrame_) {
		greenCenter_ = currentGreenCenter;
		blueCenter_ = currentBlueCenter;
		isFirstFrame_ = false;
	} else {
		// 平滑化の係数
		float alpha = 0.5f;
		greenCenter_.x = static_cast<float>(alpha * currentGreenCenter.x + (1.0f - alpha) * prevGreenCenter_.x);
		greenCenter_.y = static_cast<float>(alpha * currentGreenCenter.y + (1.0f - alpha) * prevGreenCenter_.y);

		blueCenter_.x = static_cast<float>(alpha * currentBlueCenter.x + (1.0f - alpha) * prevBlueCenter_.x);
		blueCenter_.y = static_cast<float>(alpha * currentBlueCenter.y + (1.0f - alpha) * prevBlueCenter_.y);
	}

	prevGreenCenter_ = greenCenter_;
	prevBlueCenter_ = blueCenter_;
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Initialize() {}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Update() {

	// カメラが開かれていなければ早期リターン
	if (!camera_.isOpened()) {

		return;
	}

	// カメラから画像データを読んでframeに送る
	camera_ >> frame_;

	// 何も取得できなければ早期リターン
	if (frame_.empty()) {

		return;
	}

	// 取得したフレームの左右反転
	cv::flip(frame_, frame_, 1);

	// ガウシアンブラー
	cv::GaussianBlur(frame_, frame_, cv::Size(5, 5), 0);

	// 色のトラッキング
	ColorTracking();

	//// QRコードの検出とデコード
	//decodedText_ = qrDecoder_.detectAndDecode(frame_);
	//// QRコードを検出できたら
	//if (!decodedText_.empty()) {
	//	// QRCodeデータ追加
	//	qrCodeData_.push_back(decodedText_);
	//}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Draw() {

	// カメラがどこを映しているかだけで描画する必要はない
	// デバッグ用に近い

	// カメラが開かれていなければ早期リターン
	if (!camera_.isOpened()) {

		return;
	}

	// 何も取得できなければ早期リターン
	if (frame_.empty()) {

		return;
	}

	// カメラウィンドウの表示
	cv::imshow("CameraCapture", frame_);
}

/*////////////////////////////////////////////////////////////////////////////////
*								    終了処理
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Finalize() {

	if (camera_.isOpened()) {
		camera_.release();
	}
	cv::destroyAllWindows();
}