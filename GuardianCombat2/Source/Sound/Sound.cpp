#include "Sound.h"
#include "../main.h"
#include "../DirectXRenderer.h"

IXAudio2* SoundManager::pXAudio2 = nullptr;												// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice* SoundManager::pMasteringVoice = nullptr;						// マスターボイス
IXAudio2SourceVoice* SoundManager::apSourceVoice[SoundManager::SOUND_MAX] = {};			// ソースボイス
BYTE* SoundManager::apDataAudio[SoundManager::SOUND_MAX] = {};							// オーディオデータ
DWORD SoundManager::aSizeAudio[SoundManager::SOUND_MAX] = {};							// オーディオデータサイズ
SoundManager::SOUND_INFO SoundManager::soundInfo[SoundManager::SOUND_MAX] = {};
SoundManager::SOUNDPARAM SoundManager::aParam[SoundManager::SOUND_MAX] = {
	"resource/Sound/SelectOK.wav",0,
};

HRESULT SoundManager::Init()
{
	HRESULT hr;
	HWND hWnd = GethWnd();

	

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2オブジェクトの開放
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFileA(aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBoxA(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBoxA(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		apDataAudio[nCntSound] = (BYTE*)malloc(aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, apDataAudio[nCntSound], aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = pXAudio2->CreateSourceVoice(&apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = aSizeAudio[nCntSound];
		buffer.pAudioData = apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = aParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		
		soundInfo[nCntSound].aSizeAudio = aSizeAudio[nCntSound];
		soundInfo[nCntSound].apDataAudio = apDataAudio[nCntSound];
		soundInfo[nCntSound].aParam = aParam[nCntSound];
		soundInfo[nCntSound].apSourceVoice = apSourceVoice[nCntSound];
	}

	
	return S_OK;
}

void SoundManager::Uninit()
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (apSourceVoice[nCntSound])
		{
			// 一時停止
			apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			apSourceVoice[nCntSound]->DestroyVoice();
			apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(apDataAudio[nCntSound]);
			apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	if (pXAudio2)
	{
		// XAudio2オブジェクトの開放
		pXAudio2->Release();
		pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

SoundManager::SOUND_INFO SoundManager::GetSound(SOUND_LABEL label)
{
	return soundInfo[label];
}

HRESULT SoundManager::CheckChunk(HANDLE hFile, DWORD format, DWORD * pChunkSize, DWORD * pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT SoundManager::ReadChunkData(HANDLE hFile, void * pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

Sound::Sound(SoundManager::SOUND_LABEL label)
{
	soundInfo = SoundManager::GetSound(label);
}

HRESULT Sound::PlaySound()
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundInfo.aSizeAudio;
	buffer.pAudioData = soundInfo.apDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = soundInfo.aParam.nCntLoop;

	// 状態取得
	soundInfo.apSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		soundInfo.apSourceVoice->Stop(0);

		// オーディオバッファの削除
		soundInfo.apSourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録

	soundInfo.apSourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	soundInfo.apSourceVoice->Start(0);

	return S_OK;
}

void Sound::StopSound(void)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	soundInfo.apSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		soundInfo.apSourceVoice->Stop(0);

		// オーディオバッファの削除
		soundInfo.apSourceVoice->FlushSourceBuffers();
	}
}
