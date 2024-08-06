#include "Hash.h"



// hajikiSpec_ = type setter
void Hash::SetType(Hash::HAJIKI_SPEC type) {

	hajikiSpec_ = type;
}



/*////////////////////////////////////////////////////////////////////////////////

*								ハッシュ値を生成する

////////////////////////////////////////////////////////////////////////////////*/
std::string Hash::Create(const std::string& input) {

	EVP_MD_CTX* context = EVP_MD_CTX_new();
	if (context == nullptr) {
		throw std::runtime_error("Failed to create EVP_MD_CTX");
	}

	if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to initialize digest");
	}

	if (EVP_DigestUpdate(context, input.c_str(), input.size()) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to update digest");
	}

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int lengthOfHash = 0;

	if (EVP_DigestFinal_ex(context, hash, &lengthOfHash) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to finalize digest");
	}

	EVP_MD_CTX_free(context);

	std::stringstream ss;
	for (unsigned int i = 0; i < lengthOfHash; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}

	return ss.str();
}



/*////////////////////////////////////////////////////////////////////////////////

*						HAJIKI_TYPEのハッシュ値を取得する

////////////////////////////////////////////////////////////////////////////////*/
std::string Hash::GetTypeHash() const {

	// 各タイプのハッシュ値を生成
	static const std::unordered_map<HAJIKI_TYPE, std::string> typeToString = {

		{HAJIKI_TYPE::NORMAL, "HAJIKI_TYPE::NORMAL"},
		{HAJIKI_TYPE::FEATHER, "HAJIKI_TYPE::FEATHER"},
		{HAJIKI_TYPE::HEAVY, "HAJIKI_TYPE::HEAVY"}
	};

	auto it = typeToString.find(hajikiSpec_.type);
	if (it != typeToString.end()) {

		return Create(it->second);
	}

	// 未知タイプの場合のデフォルトハッシュ値を返す
	return Create("UNKNOWN");
}



/*////////////////////////////////////////////////////////////////////////////////

*						HAJIKI_TYPEの名前を取得する

////////////////////////////////////////////////////////////////////////////////*/
std::string Hash::GetTypeName() const {

	static const std::unordered_map<HAJIKI_TYPE, std::string> typeToString = {

		{HAJIKI_TYPE::NORMAL, "NORMAL"},
		{HAJIKI_TYPE::FEATHER, "FEATHER"},
		{HAJIKI_TYPE::HEAVY, "HEAVY"}
	};

	auto it = typeToString.find(hajikiSpec_.type);
	if (it != typeToString.end()) {

		return it->second;
	}

	return "UNKNOWN";
}