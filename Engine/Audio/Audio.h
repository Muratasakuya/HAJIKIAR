#pragma once

// DirectX
#include <xaudio2.h>
#include "ComPtr.h"

// c++
#include <fstream>
#include <cassert>

// 音声データ
struct SoundData {

	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	uint32_t bufferSize;
};

/*////////////////////////////////////////////////////////////////////////////////
*
*								Audio Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Audio {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// チャンク
	struct ChunkHeader {

		char id[4];
		int32_t size;
	};

	// RIFFチャンク
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	// FMTチャンク
	struct FormatChunk {

		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	SoundData SoundLoadWave(const char* filename);
	void SoundPlayWave(const SoundData& soundData);
	void SoundUnload(SoundData* soundData);

	void Initialize();

	void Reset(SoundData* soundData);
	void Finalize();

	// singleton
	static Audio* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

	Audio() = default;
	~Audio() = default;
	// コピー禁止
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&) = delete;
};