#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <xaudio2.h>


class SoundManager
{
public:
	enum SOUND_LABEL
	{
		SELECTOK_SE,
		SOUND_MAX,
	};
	struct SOUNDPARAM
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	};
	struct SOUND_INFO
	{
		DWORD aSizeAudio;
		BYTE* apDataAudio;
		SoundManager::SOUNDPARAM aParam;
		IXAudio2SourceVoice* apSourceVoice;
	};

	static HRESULT Init();
	static void Uninit();
	static SOUND_INFO GetSound(SOUND_LABEL label);
private:
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2* pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice* pMasteringVoice;			// マスターボイス
	static IXAudio2SourceVoice* apSourceVoice[SOUND_MAX];		// ソースボイス
	static BYTE* apDataAudio[SOUND_MAX];						// オーディオデータ
	static DWORD aSizeAudio[SOUND_MAX];						// オーディオデータサイズ
	static SOUNDPARAM aParam[SOUND_MAX];

	static SOUND_INFO soundInfo[SOUND_MAX];
};	

class Sound
{
public:
	Sound(SoundManager::SOUND_LABEL label);
	HRESULT PlaySound();
	void StopSound(void);
private:
	SoundManager::SOUND_INFO soundInfo;
};