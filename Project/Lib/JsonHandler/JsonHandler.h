#pragma once

// json
#include <json.hpp>

// OpenCV
#include <opencv2/opencv.hpp>

// c++
#include <array>
#include <string>
#include <fstream>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*									JsonHandler Class
////////////////////////////////////////////////////////////////////////////////*/
class JsonHandler {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	JsonHandler(const std::string& directoryPath);
	~JsonHandler() {}

	// OpenCV
	void SaveSettings(const std::string& fileName, const std::array<cv::Point, 4>& points, int exposure, int kelvin, bool whiteBalance, float saturationScale);
	bool LoadSettings(const std::string& fileName, std::array<cv::Point, 4>& points, int& exposure, int& kelvin, bool& whiteBalance, float& saturationScale);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::string directoryPath_;

};