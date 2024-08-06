#pragma once

// OpenSSL
#include <openssl/sha.h>
#include <openssl/evp.h>

// c++
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <stdexcept>

/*////////////////////////////////////////////////////////////////////////////////
*									Hash Class
////////////////////////////////////////////////////////////////////////////////*/
class Hash {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	Hash() = default;
	~Hash() = default;

#pragma region /// データ型 ///

	enum class HAJIKI_TYPE {

		NORMAL,
		FEATHER,
		HEAVY,

		// タイプの数
		TYPENUM
	};
	struct HAJIKI_SPEC {

		// 読み込むデータの種類
		HAJIKI_TYPE type;
	};

#pragma endregion

	static std::string Create(const std::string& input);

	//setter

	void SetType(HAJIKI_SPEC type);

	// getter

	std::string GetTypeHash() const;
	std::string GetTypeName() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	HAJIKI_SPEC hajikiSpec_;

};