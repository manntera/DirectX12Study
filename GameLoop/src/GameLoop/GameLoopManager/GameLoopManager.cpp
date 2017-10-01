//==========================
//===�yGameLoopManager.cpp�z
//==========================
//    �Q�[�����[�v�̊Ǘ����s���܂��B
//==========================

//=====================//
//===�y�C���N���[�h�L�q�z ===//
//=====================//
#include "GameLoopManager.h"


OGL::GameLoopManager::GameLoopManager()
{
	m_loopFlg = false;
	mp_gameScene = nullptr;
	mp_nextScene = nullptr;
}

OGL::GameLoopManager::~GameLoopManager()
{}

//����������������������//
//�������ypublic�z������//
//����������������������//

//��������������������������������������������
//�������@�@�\�F�Q�[�����[�v���J�n���܂��B
//������ ����1�Ffloat�F�Q�[�����[�v�̃t���[�����[�g
//������   �Ԓl�Fvoid
//��������������������������������������������
void	OGL::GameLoopManager::BeginGameLoopManager(float frameRate, OGL::GameScene* entryScene)
{
	mp_gameScene = entryScene;
	m_frameRate = frameRate;
	m_loopFlg = true;
	std::thread	tempThread(&OGL::GameLoopManager::LoopMain, this);
	std::swap(m_loopThread, tempThread);
}
//��������������������������������������������
//�������@�@�\�F�Q�[�����[�v���I�����܂��B
//������ ����1�Fvoid
//������   �Ԓl�Fvoid
//��������������������������������������������
void	OGL::GameLoopManager::EndGameLoopManager(void)
{
	m_loopFlg = false;
	if (m_loopThread.joinable()) {
		m_loopThread.join();
	}
}
void	OGL::GameLoopManager::SetNextScene(OGL::GameScene* entryScene)
{
	mp_nextScene = entryScene;
}
//������������������������//
//�������yprivate�z ������//
//������������������������//

//��������������������������������������������
//�������@�@�\�F�Q�[�����[�v�{�̂ł��B
//������ ����1�Fvoid
//������   �Ԓl�Fvoid
//��������������������������������������������
void	OGL::GameLoopManager::LoopMain(void)
{
	if (!mp_gameScene->Init()) {
		m_loopFlg = false;
	}
	m_loopTimer.StartLapTime();
	while (m_loopFlg) {
		if (m_loopTimer.GetLapTime() < 1000 / m_frameRate) {
			continue;
		}
		m_loopTimer.StartLapTime();
		if (!mp_gameScene->Update()) {
			m_loopFlg = false;
		}
		if (!mp_gameScene->Draw()) {
			m_loopFlg = false;
		}
		if (mp_nextScene) {
			mp_gameScene->Uninit();
			delete(mp_gameScene);
			mp_gameScene = mp_nextScene;
			mp_nextScene = nullptr;
			if (!mp_gameScene->Init()) {
				m_loopFlg = false;
			}
		}
	}
	mp_gameScene->Uninit();
}

bool	OGL::GameLoopManager::IsLoopFlg(void)
{
	return m_loopFlg;
}