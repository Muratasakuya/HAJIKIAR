
#define NOMINMAX

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

void OpenCV::SetEdgeSize(const Vector2& edgeSize) {

	edgeSize_ = edgeSize;
}

void OpenCV::SetGame3DMode(bool game3DMode) {

	game3DMode_ = game3DMode;
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

Vector2 OpenCV::GetGreenLargestCenterPos() const {

	return largestGreenCenter_;
}

Vector2 OpenCV::GetSmallestGreenCenterPos() const{

	return smallestGreenCenter_;
}

// isBlueHajikiFound_ getter
bool OpenCV::IsBlueHajikiFound() const {

	return isBlueHajikiFound_;
}

// isGreenHajikiFound_ getter
bool OpenCV::IsGreenHajikiFound() const {

	return isGreenHajikiFound_;
}

Vector2 OpenCV::GetTrackColorPos() const {

	return trackColorCenter1_;
}
bool OpenCV::FoundTrackColor() const {

	return foundTrackColor1_;
}

Vector2 OpenCV::GetOtherColorPos() const {

	return trackColorCenter2_;
}
bool OpenCV::FoundOtherColor() const {

	return foundTrackColor2_;
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
*									彩度調整
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::IncreaseSaturation(cv::Mat& frame, float saturationScale) {

	// BGR色空間からHSV色空間に変換
	cv::Mat hsvFrame;
	cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

	// HSVチャンネルを分離
	std::vector<cv::Mat> hsvChannels;
	cv::split(hsvFrame, hsvChannels);

	// 彩度チャンネルを調整
	hsvChannels[1] *= saturationScale; // Sチャンネルを指定倍率で調整

	// 彩度が255を超えないようにクリップ
	cv::threshold(hsvChannels[1], hsvChannels[1], 255, 255, cv::THRESH_TRUNC);

	// チャンネルを結合して再度HSV画像を作成
	cv::merge(hsvChannels, hsvFrame);

	// HSV色空間からBGR色空間に戻す
	cv::cvtColor(hsvFrame, frame, cv::COLOR_HSV2BGR);
}

Vector2 OpenCV::NormalizeCoordinates(const cv::Point& center){

	Vector2 normalizedCenter;

	if (game3DMode_) {
		// ウィンドウの中心座標を求める
		float halfWidth = 640.0f / 2.0f;
		float halfHeight = 360.0f / 2.0f;

		// 座標をウィンドウの中心を基準に変換
		float centeredX = static_cast<float>(center.x) - halfWidth;
		float centeredY = static_cast<float>(center.y) - halfHeight;

		// 中心を基準にした座標を -1 から 1 に正規化
		normalizedCenter.x = centeredX / halfWidth;
		normalizedCenter.y = centeredY / halfHeight;

		// 正規化された座標を使用してエッジサイズを計算
		normalizedCenter.x *= edgeSize_.x;
		normalizedCenter.y *= edgeSize_.y;
	} else {
		// 座標の正規化
		normalizedCenter.x = static_cast<float>(center.x) / 640.0f;
		normalizedCenter.y = static_cast<float>(center.y) / 360.0f;

		normalizedCenter.x *= edgeSize_.x;
		normalizedCenter.y *= edgeSize_.y;
	}

	return normalizedCenter;
}

/*////////////////////////////////////////////////////////////////////////////////
*									カメラ起動
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::OpenCamera() {

	// カメラ起動
	camera_.open(0, cv::CAP_DSHOW);
	framerate_ = 120;

	// カメラが開けなければエラー
	if (!camera_.isOpened()) {

		throw std::runtime_error("Error: Camera could not be opened.");
	}

	// 初期化が終わったら、必要に応じて解像度を上げる
	camera_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	camera_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera_.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	camera_.set(cv::CAP_PROP_FPS, framerate_);

	trackColor1_ = { 0.0f, 0.0f, 1.0f };
	trackColor2_ = { 0.3529f, 1.0f, 0.0f};

	colorRange_ = 22;
}

/*////////////////////////////////////////////////////////////////////////////////
*							領域を切り取るメソッド
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::CropAndShowPolygonRegion(const std::array<cv::Point, 4>& points) {
	// カメラが開かれていなければ早期リターン
	if (!camera_.isOpened()) {
		return;
	}

	// フレームのコピーを作成
	cv::Mat frameCopy = processedFrame_.clone();

	// 画像全体をマスクするための黒いマスクを作成
	cv::Mat mask = cv::Mat::zeros(processedFrame_.size(), CV_8UC1);

	// ポリゴンの頂点を設定
	std::vector<std::vector<cv::Point>> pts{ std::vector<cv::Point>(points.begin(), points.end()) };

	// マスクに白いポリゴンを描画
	cv::fillPoly(mask, pts, cv::Scalar(255));

	// マスクを使用して元の画像から領域を切り取る
	cv::Mat croppedFrame;
	processedFrame_.copyTo(croppedFrame, mask);

	// 変換先の4つの頂点を指定
	std::vector<cv::Point2f> dstPoints;
	dstPoints.push_back(cv::Point2f(0, 0));               // 左上
	dstPoints.push_back(cv::Point2f(640, 0));             // 右上
	dstPoints.push_back(cv::Point2f(640, 360));           // 右下
	dstPoints.push_back(cv::Point2f(0, 360));             // 左下

	// 変換元の頂点（入力頂点）を設定
	std::vector<cv::Point2f> srcPoints;
	for (const auto& point : points) {
		srcPoints.push_back(cv::Point2f(static_cast<float>(point.x), static_cast<float>(point.y)));
	}

	// 射影変換行列を計算
	cv::Mat transformMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);

	// 射影変換を実行して、指定されたサイズに変換
	cv::Mat transformedFrame;
	cv::warpPerspective(croppedFrame, transformedFrame, transformMatrix, cv::Size(640, 360));

	// 色のトラッキング
	ColorTracking(transformedFrame);

	// 変換されたフレームの表示
	cv::imshow("TransformedRegion", transformedFrame);
}

/*////////////////////////////////////////////////////////////////////////////////
*							色のついたものの複数トラッキング
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::ColorTracking(cv::Mat& frame) {

	/*======================================================*/
	// ImGui

#ifdef _DEBUG

	ImGui::Begin("OpenCV");

	// 2つの色の設定
	ImGui::ColorEdit3("trackColor1", &trackColor1_.x);
	ImGui::ColorEdit3("trackColor2", &trackColor2_.x);
	ImGui::DragInt("colorRange", &colorRange_, 1, 0, 100);

	ImGui::End();
#endif

	/*======================================================*/

#pragma region /// method ///

	// フレームをHSV色空間に変換
	cv::Mat hsvFrame;
	cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

	// RGBからHSV色空間に変換
	cv::Mat hsvColor1 = ConvertRGBtoHSV(trackColor1_);
	cv::Mat hsvColor2 = ConvertRGBtoHSV(trackColor2_);

	// HSV値を取得
	cv::Vec3b hsvValues1 = hsvColor1.at<cv::Vec3b>(0, 0);
	cv::Vec3b hsvValues2 = hsvColor2.at<cv::Vec3b>(0, 0);

	// 指定した2つの色の範囲を定義
	cv::Mat colorMask1, colorMask2;
	cv::inRange(hsvFrame, cv::Scalar(hsvValues1[0] - colorRange_, 100, 100),
		cv::Scalar(hsvValues1[0] + colorRange_, 255, 255), colorMask1);
	cv::inRange(hsvFrame, cv::Scalar(hsvValues2[0] - colorRange_, 100, 100),
		cv::Scalar(hsvValues2[0] + colorRange_, 255, 255), colorMask2);

	// モルフォロジー変換（開閉処理）でノイズ除去
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(colorMask1, colorMask1, cv::MORPH_CLOSE, kernel);
	cv::morphologyEx(colorMask1, colorMask1, cv::MORPH_OPEN, kernel);
	cv::morphologyEx(colorMask2, colorMask2, cv::MORPH_CLOSE, kernel);
	cv::morphologyEx(colorMask2, colorMask2, cv::MORPH_OPEN, kernel);

	// 2つの色の物体の輪郭を検出
	std::vector<std::vector<cv::Point>> color_contours1, color_contours2;
	cv::findContours(colorMask1, color_contours1, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::findContours(colorMask2, color_contours2, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// 中心座標の計算
	Vector2 currentColorCenter1{};
	Vector2 currentColorCenter2{};

	// 1つ目の色の物体の中心座標を計算
	foundTrackColor1_ = false; // フラグをリセット
	for (size_t i = 0; i < color_contours1.size(); ++i) {
		cv::Moments m = cv::moments(color_contours1[i]);
		double area = cv::contourArea(color_contours1[i]);

		if (area > 100 && m.m00 != 0) {
			cv::Point center(
				static_cast<int>(m.m10 / m.m00),
				static_cast<int>(m.m01 / m.m00));

			currentColorCenter1 = NormalizeCoordinates(center);
			foundTrackColor1_ = true;
			break; // 最初に見つけた中心だけを取得
		}
	}

	// 2つ目の色の物体の中心座標を計算
	foundTrackColor2_ = false; // フラグをリセット
	for (size_t i = 0; i < color_contours2.size(); ++i) {
		cv::Moments m = cv::moments(color_contours2[i]);
		double area = cv::contourArea(color_contours2[i]);

		if (area > 100 && m.m00 != 0) {
			cv::Point center(
				static_cast<int>(m.m10 / m.m00),
				static_cast<int>(m.m01 / m.m00));

			currentColorCenter2 = NormalizeCoordinates(center);
			foundTrackColor2_ = true;
			break; // 最初に見つけた中心だけを取得
		}
	}

	// フレーム間の追跡の平滑化
	if (isFirstFrame_) {
		// 最初のフレームでは現在の座標を使用
		trackColorCenter1_ = currentColorCenter1;
		trackColorCenter2_ = currentColorCenter2;
		isFirstFrame_ = false;
	} else {
		// 前回のフレームの座標を使用するための条件を追加
		if (foundTrackColor1_) {
			// 座標が取得できた場合は現在の座標を使用
			trackColorCenter1_ = currentColorCenter1;
		} else {
			// 座標が取得できなかった場合は前回の座標を使用
			trackColorCenter1_ = preTrackColorCenter1_;
		}

		if (foundTrackColor2_) {
			// 座標が取得できた場合は現在の座標を使用
			trackColorCenter2_ = currentColorCenter2;
		} else {
			// 座標が取得できなかった場合は前回の座標を使用
			trackColorCenter2_ = preTrackColorCenter2_;
		}

		// 平滑化の係数
		float alpha = 0.5f;
		trackColorCenter1_.x = alpha * trackColorCenter1_.x + (1.0f - alpha) * preTrackColorCenter1_.x;
		trackColorCenter1_.y = alpha * trackColorCenter1_.y + (1.0f - alpha) * preTrackColorCenter1_.y;

		trackColorCenter2_.x = alpha * trackColorCenter2_.x + (1.0f - alpha) * preTrackColorCenter2_.x;
		trackColorCenter2_.y = alpha * trackColorCenter2_.y + (1.0f - alpha) * preTrackColorCenter2_.y;
	}

	// 前回の座標を更新
	preTrackColorCenter1_ = trackColorCenter1_;
	preTrackColorCenter2_ = trackColorCenter2_;
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Initialize() {

	jsonHandler_ = std::make_unique<JsonHandler>("./Resources/GlobalVariables/");

	// JSONからポイント情報とカメラ設定を読み込む
	if (!jsonHandler_->LoadSettings("settings", points_, exposure_, kelvin_, whiteBalance_, saturationScale_)) {

		// デフォルト値を設定
		points_ = { cv::Point(0, 0), cv::Point(100, 0), cv::Point(100, 100), cv::Point(0, 100) };
		exposure_ = -6;
		kelvin_ = 4500;
		whiteBalance_ = false;
		saturationScale_ = 1.0f;
	}

	// 読み込んだ設定を使って初期化
	point1_ = points_[0];
	point2_ = points_[1];
	point3_ = points_[2];
	point4_ = points_[3];

	camera_.set(cv::CAP_PROP_AUTO_WB, whiteBalance_);
	camera_.set(cv::CAP_PROP_EXPOSURE, exposure_);
	camera_.set(cv::CAP_PROP_WB_TEMPERATURE, kelvin_);
}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void OpenCV::Update() {

#ifdef _DEBUG

	// ImGuiを使用してROIの範囲を設定
	ImGui::Begin("Polygon ROI Control");

	bool updated = false;

	updated |= ImGui::SliderInt2("Point 1", &point1_.x, -640, frame_.cols);
	updated |= ImGui::SliderInt2("Point 2", &point2_.x, -640, frame_.cols);
	updated |= ImGui::SliderInt2("Point 3", &point3_.x, -640, frame_.cols);
	updated |= ImGui::SliderInt2("Point 4", &point4_.x, -640, frame_.cols);
	ImGui::InputInt("exposure ", &exposure_);
	ImGui::InputInt("kelvin ", &kelvin_);
	ImGui::Checkbox("whiteBalance", &whiteBalance_);
	ImGui::DragFloat("saturation", &saturationScale_, 0.01f, 0.0f, 3.0f);

	ImGui::Separator();
	ImGui::Text("Json");
	if (ImGui::Button("Save")) {

		points_ = { point1_, point2_, point3_, point4_ };
		jsonHandler_->SaveSettings("settings", points_, exposure_, kelvin_, whiteBalance_, saturationScale_);
		std::string message = "Settings.json saved.";
		MessageBoxA(nullptr, message.c_str(), "Save Confirmation", 0);
	}

ImGui::End();

#endif // _DEBUG

	// ImGuiで変更した値の適応
	camera_.set(cv::CAP_PROP_AUTO_WB, whiteBalance_);
	camera_.set(cv::CAP_PROP_EXPOSURE, exposure_);
	camera_.set(cv::CAP_PROP_WB_TEMPERATURE, kelvin_);

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

	// 彩度を上げる
	IncreaseSaturation(frame_, saturationScale_);

	// 全体のトーンカーブエディタのレンダリング
	RenderToneCurveEditor(toneCurvePoints_, "Tone Curve Editor - Overall");
	// RGB個別のトーンカーブエディタのレンダリング
	RenderToneCurveEditor(toneCurvePointsR_, "Tone Curve Editor - Red");
	RenderToneCurveEditor(toneCurvePointsG_, "Tone Curve Editor - Green");
	RenderToneCurveEditor(toneCurvePointsB_, "Tone Curve Editor - Blue");
	// 全体のトーンカーブを適用
	ApplyToneCurve(frame_, processedFrame_, toneCurvePoints_);
	// RGBごとのトーンカーブを適用
	ApplyToneCurveRGB(processedFrame_, processedFrame_, toneCurvePointsR_, toneCurvePointsG_, toneCurvePointsB_);

	// 指定した4つの頂点で領域を切り取って表示する
	std::array<cv::Point, 4> points = { point1_, point2_, point3_, point4_ };
	CropAndShowPolygonRegion(points);

	//// 全体のトーンカーブエディタのレンダリング
	//RenderToneCurveEditor(toneCurvePoints_, "Tone Curve Editor - Overall");
	//// RGB個別のトーンカーブエディタのレンダリング
	//RenderToneCurveEditor(toneCurvePointsR_, "Tone Curve Editor - Red");
	//RenderToneCurveEditor(toneCurvePointsG_, "Tone Curve Editor - Green");
	//RenderToneCurveEditor(toneCurvePointsB_, "Tone Curve Editor - Blue");
	//// 全体のトーンカーブを適用
	//ApplyToneCurve(frame_, processedFrame_, toneCurvePoints_);
	//// RGBごとのトーンカーブを適用
	//ApplyToneCurveRGB(processedFrame_, processedFrame_, toneCurvePointsR_, toneCurvePointsG_, toneCurvePointsB_);

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
void OpenCV::RenderToneCurveEditor(std::vector<cv::Point>& points, const char* label) {


#ifdef _DEBUG

	ImGui::Begin(label);

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

	ImGui::InvisibleButton("canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft);
	bool is_hovered = ImGui::IsItemHovered();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

	for (size_t i = 0; i < points.size(); ++i) {
		ImVec2 p = ImVec2(canvas_pos.x + (points[i].x / 255.0f) * canvas_size.x, canvas_pos.y + (1.0f - points[i].y / 255.0f) * canvas_size.y);
		draw_list->AddCircleFilled(p, 3.0f, IM_COL32(255, 0, 0, 255));

		if (ImGui::IsMouseHoveringRect(ImVec2(p.x - 5, p.y - 5), ImVec2(p.x + 5, p.y + 5)) && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			ImVec2 mouse_pos = ImGui::GetMousePos();
			points[i].x = static_cast<int>((mouse_pos.x - canvas_pos.x) / canvas_size.x * 255);
			points[i].y = static_cast<int>(255 - (mouse_pos.y - canvas_pos.y) / canvas_size.y * 255);
		}
	}

	for (size_t i = 0; i < points.size() - 1; ++i) {
		ImVec2 p1 = ImVec2(canvas_pos.x + (points[i].x / 255.0f) * canvas_size.x, canvas_pos.y + (1.0f - points[i].y / 255.0f) * canvas_size.y);
		ImVec2 p2 = ImVec2(canvas_pos.x + (points[i + 1].x / 255.0f) * canvas_size.x, canvas_pos.y + (1.0f - points[i + 1].y / 255.0f) * canvas_size.y);
		draw_list->AddLine(p1, p2, IM_COL32(255, 255, 255, 255));
	}

	ImGui::End();

#endif // _DEBUG

}
void OpenCV::ApplyToneCurve(const cv::Mat& inputImage, cv::Mat& outputImage, const std::vector<cv::Point>& toneCurvePoints) {

	cv::Mat lookupTable(1, 256, CV_8U);
	uchar* p = lookupTable.ptr();
	for (int i = 0; i < 256; ++i) {
		float x = static_cast<float>(i);
		float y = static_cast<float>(i);

		for (size_t j = 0; j < toneCurvePoints.size() - 1; ++j) {
			cv::Point p0 = toneCurvePoints[j];
			cv::Point p1 = toneCurvePoints[j + 1];

			if (x >= p0.x && x <= p1.x) {
				float t = (x - p0.x) / (p1.x - p0.x);
				y = p0.y * (1 - t) + p1.y * t;
				break;
			}
		}

		p[i] = cv::saturate_cast<uchar>(y);
	}

	cv::LUT(inputImage, lookupTable, outputImage);
}
void OpenCV::ApplyToneCurveRGB(const cv::Mat& inputImage, cv::Mat& outputImage, const std::vector<cv::Point>& toneCurvePointsR,
	const std::vector<cv::Point>& toneCurvePointsG, const std::vector<cv::Point>& toneCurvePointsB) {

	std::array<cv::Mat, 3> lookupTables;
	lookupTables[0] = cv::Mat(1, 256, CV_8U);  // Blue
	lookupTables[1] = cv::Mat(1, 256, CV_8U);  // Green
	lookupTables[2] = cv::Mat(1, 256, CV_8U);  // Red

	// 各チャンネルのルックアップテーブルを作成
	for (int ch = 0; ch < 3; ++ch) {
		uchar* p = lookupTables[ch].ptr();
		const std::vector<cv::Point>& toneCurvePoints = (ch == 0) ? toneCurvePointsB : (ch == 1) ? toneCurvePointsG : toneCurvePointsR;

		for (int i = 0; i < 256; ++i) {
			float x = static_cast<float>(i);
			float y = static_cast<float>(i);

			for (size_t j = 0; j < toneCurvePoints.size() - 1; ++j) {
				cv::Point p0 = toneCurvePoints[j];
				cv::Point p1 = toneCurvePoints[j + 1];

				if (x >= p0.x && x <= p1.x) {
					float t = (x - p0.x) / (p1.x - p0.x);
					y = p0.y * (1 - t) + p1.y * t;
					break;
				}
			}

			p[i] = cv::saturate_cast<uchar>(y);
		}
	}

	// 各チャンネルにトーンカーブを適用
	cv::Mat channels[3];
	cv::split(inputImage, channels);
	for (int ch = 0; ch < 3; ++ch) {
		cv::LUT(channels[ch], lookupTables[ch], channels[ch]);
	}
	cv::merge(channels, 3, outputImage);
}