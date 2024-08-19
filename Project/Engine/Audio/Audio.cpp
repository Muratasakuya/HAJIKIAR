#include "Audio.h"

#pragma comment(lib,"xaudio2.lib")

/*////////////////////////////////////////////////////////////////////////////////
*								音声データの読み込み
////////////////////////////////////////////////////////////////////////////////*/
void Audio::LoadWave(const std::string& filename) {

	// 識別子を除いた名前を取得
	std::string identifier = GetFileNameWithoutExtension(filename);
	SoundData soundData = LoadWaveInternal(filename);

	// サウンドデータ追加
	soundData_.emplace(identifier, soundData);
}
SoundData Audio::LoadWaveInternal(const std::string& filename) {

	std::ifstream file(filename, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
	assert(strncmp(riff.chunk.id, "RIFF", 4) == 0);
	assert(strncmp(riff.type, "WAVE", 4) == 0);

	FormatChunk format{};
	bool formatFound = false;
	bool dataFound = false;
	ChunkHeader chunkHeader;

	while (!dataFound && file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader))) {
		if (strncmp(chunkHeader.id, "fmt ", 4) == 0) {

			file.read(reinterpret_cast<char*>(&format.fmt), chunkHeader.size);
			formatFound = true;
		} else if (strncmp(chunkHeader.id, "data", 4) == 0) {

			BYTE* pBuffer = new BYTE[chunkHeader.size];
			file.read(reinterpret_cast<char*>(pBuffer), chunkHeader.size);

			SoundData soundData{};
			soundData.wfex = format.fmt;
			soundData.pBuffer = pBuffer;
			soundData.bufferSize = chunkHeader.size;

			dataFound = true;
			file.close();
			return soundData;
		} else {

			// 読み込んだチャンクをスキップ
			file.seekg(chunkHeader.size, std::ios_base::cur);
		}
	}

	// ファイルを閉じる
	file.close();
	// 必要なチャンクが見つからなかった場合はエラー
	assert(formatFound && dataFound);

	// エラー時にはデフォルトのSoundDataを返す とりあえずreturnしとく
	return SoundData{};
}

/*////////////////////////////////////////////////////////////////////////////////
*						ファイル名から拡張子を除いた名前を取得
////////////////////////////////////////////////////////////////////////////////*/
std::string Audio::GetFileNameWithoutExtension(const std::string& filename) {

	std::filesystem::path path(filename);

	return path.stem().string();
}

/*////////////////////////////////////////////////////////////////////////////////
*									音声データ解放
////////////////////////////////////////////////////////////////////////////////*/
void Audio::Unload() {

	// バッファメモリを解放
	for (auto& [name, soundData] : soundData_) {
		delete[] soundData.pBuffer;
	}
	soundData_.clear();
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンドの再生
////////////////////////////////////////////////////////////////////////////////*/
void Audio::PlayWave(const std::string& name, bool loop) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = soundData_.find(identifier);
	if (it == soundData_.end()) return;

	// 再生中の音声があるか確認 あれば早期リターン
	auto voiceIt = activeVoices_.find(identifier);
	if (voiceIt != activeVoices_.end()) {
		return;
	}

	const SoundData& soundData = it->second;

	// 新しい SourceVoice を作成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	HRESULT hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	} else {
		buf.LoopCount = 0;
	}

	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));

	hr = pSourceVoice->Start();
	assert(SUCCEEDED(hr));

	// 再生中の SourceVoice を登録
	activeVoices_[identifier] = pSourceVoice;
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンドの停止
////////////////////////////////////////////////////////////////////////////////*/
void Audio::StopWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		IXAudio2SourceVoice* pSourceVoice = it->second;
		pSourceVoice->Stop();
		pSourceVoice->DestroyVoice();
		activeVoices_.erase(it);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド一時停止
////////////////////////////////////////////////////////////////////////////////*/
void Audio::PauseWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->Stop(0);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								サウンド一時停止からの再生
////////////////////////////////////////////////////////////////////////////////*/
void Audio::ResumeWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->Start(0);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド音量の設定
////////////////////////////////////////////////////////////////////////////////*/
void Audio::SetVolume(const std::string& name, float volume) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		it->second->SetVolume(volume);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									サウンド再生中かどうか
////////////////////////////////////////////////////////////////////////////////*/
bool Audio::IsPlayWave(const std::string& name) {

	std::string identifier = GetFileNameWithoutExtension(name);

	auto it = activeVoices_.find(identifier);
	if (it != activeVoices_.end()) {

		XAUDIO2_VOICE_STATE state;
		it->second->GetState(&state);

		return (state.BuffersQueued > 0);
	}

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*										初期化
////////////////////////////////////////////////////////////////////////////////*/
void Audio::Initialize() {

	HRESULT hr;

	// XAudioエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

/*////////////////////////////////////////////////////////////////////////////////
*										  解放
////////////////////////////////////////////////////////////////////////////////*/
void Audio::Finalize() {

	Unload();
	for (auto& [name, pSourceVoice] : activeVoices_) {
		if (pSourceVoice) {

			pSourceVoice->Stop();
			pSourceVoice->DestroyVoice();
		}
	}
	activeVoices_.clear();
	if (masterVoice_) {

		masterVoice_->DestroyVoice();
		masterVoice_ = nullptr;
	}
	xAudio2_.Reset();
}