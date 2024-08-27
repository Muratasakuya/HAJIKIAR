#include "JsonHandler.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
JsonHandler::JsonHandler(const std::string& directoryPath)
	: directoryPath_(directoryPath) {

	// ディレクトリが存在しなければ作成する
	std::filesystem::path dir(directoryPath_);
	if (!std::filesystem::exists(dir)) {

		std::filesystem::create_directory(dir);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								頂点情報保存
////////////////////////////////////////////////////////////////////////////////*/
void JsonHandler::SaveSettings(const std::string& fileName, const std::array<cv::Point, 4>& points, int exposure, int kelvin, bool whiteBalance) {

	nlohmann::json j;
	for (int i = 0; i < 4; ++i) {

		j["Point" + std::to_string(i + 1)] = { points[i].x, points[i].y };
	}
	j["Exposure"] = exposure;
	j["Kelvin"] = kelvin;
	j["WhiteBalance"] = whiteBalance;

	std::string filePath = directoryPath_ + fileName + ".json";
	std::ofstream file(filePath);
	if (file.is_open()) {

		// インデント幅4でJSONファイルを書き込む
		file << j.dump(4);
		file.close();
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							JSONファイルから頂点情報を読み込む
////////////////////////////////////////////////////////////////////////////////*/
bool JsonHandler::LoadSettings(const std::string& fileName, std::array<cv::Point, 4>& points, int& exposure, int& kelvin, bool& whiteBalance) {

	std::string filePath = directoryPath_ + fileName + ".json";
	std::ifstream file(filePath);
	if (file.is_open()) {

		nlohmann::json j;
		file >> j;

		for (int i = 0; i < 4; ++i) {

			points[i].x = j["Point" + std::to_string(i + 1)][0];
			points[i].y = j["Point" + std::to_string(i + 1)][1];
		}

		exposure = j["Exposure"];
		kelvin = j["Kelvin"];
		whiteBalance = j["WhiteBalance"];
		file.close();

		return true;
	}

	// ファイルが存在しない場合や開けない場合
	return false;
}
