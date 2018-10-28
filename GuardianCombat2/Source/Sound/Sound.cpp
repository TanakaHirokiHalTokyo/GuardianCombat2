#include "Sound.h"
#include "../main.h"
#include "../DirectXRenderer.h"

IXAudio2* SoundManager::pXAudio2 = nullptr;												// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice* SoundManager::pMasteringVoice = nullptr;						// �}�X�^�[�{�C�X
IXAudio2SourceVoice* SoundManager::apSourceVoice[SoundManager::SOUND_MAX] = {};			// �\�[�X�{�C�X
BYTE* SoundManager::apDataAudio[SoundManager::SOUND_MAX] = {};							// �I�[�f�B�I�f�[�^
DWORD SoundManager::aSizeAudio[SoundManager::SOUND_MAX] = {};							// �I�[�f�B�I�f�[�^�T�C�Y
SoundManager::SOUND_INFO SoundManager::soundInfo[SoundManager::SOUND_MAX] = {};
SoundManager::SOUNDPARAM SoundManager::aParam[SoundManager::SOUND_MAX] = {
	"resource/Sound/SelectOK.wav",0,
};

HRESULT SoundManager::Init()
{
	HRESULT hr;
	HWND hWnd = GethWnd();

	

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBoxA(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFileA(aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBoxA(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBoxA(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		apDataAudio[nCntSound] = (BYTE*)malloc(aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, apDataAudio[nCntSound], aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = pXAudio2->CreateSourceVoice(&apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBoxA(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = aSizeAudio[nCntSound];
		buffer.pAudioData = apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
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
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			apSourceVoice[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			apSourceVoice[nCntSound]->DestroyVoice();
			apSourceVoice[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(apDataAudio[nCntSound]);
			apDataAudio[nCntSound] = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	if (pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		pXAudio2->Release();
		pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
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

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = soundInfo.aSizeAudio;
	buffer.pAudioData = soundInfo.apDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = soundInfo.aParam.nCntLoop;

	// ��Ԏ擾
	soundInfo.apSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		soundInfo.apSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundInfo.apSourceVoice->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^

	soundInfo.apSourceVoice->SubmitSourceBuffer(&buffer);

	// �Đ�
	soundInfo.apSourceVoice->Start(0);

	return S_OK;
}

void Sound::StopSound(void)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	soundInfo.apSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		soundInfo.apSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		soundInfo.apSourceVoice->FlushSourceBuffers();
	}
}
