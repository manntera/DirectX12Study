//==========================
//===�yTimer.cpp�z
//==========================
//    ���Ԃ��Ǘ����܂��B
//==========================
#include "Timer.h"

OGL::Timer::Timer()
{
	m_startLapTime = 0;
	m_StartStopTime = 0;
	m_stopFlg = false;
}

OGL::Timer::~Timer()
{
}

//����������������������//
//�������ypublic�z������//
//����������������������//

//��������������������������������������������
//�������@�@�\�F�v���O�����̎��s���Ԃ��~���b�P�ʂŎ擾���܂��B
//������ ����1�Fvoid
//������   �Ԓl�FDWORD�F���s����(�~���b)
//��������������������������������������������
DWORD	OGL::Timer::GetTime(void)
{
	return timeGetTime();
}

//��������������������������������������������
//�������@�@�\�F���b�v�^�C���J�n���܂��B
//������ ����1�Fvoid
//������   �Ԓl�Fvoid
//��������������������������������������������
void		OGL::Timer::StartLapTime(void)
{
	m_startLapTime = GetTime();
	m_StartStopTime = m_startLapTime;
}

//��������������������������������������������
//�������@�@�\�F���b�v�^�C���擾���܂��B
//������ ����1�Fvoid
//������   �Ԓl�FDWORD�F���b�v�^�C��(�~���b)
//��������������������������������������������
DWORD	OGL::Timer::GetLapTime(void)
{
	if (m_stopFlg == true)
	{
		return m_StartStopTime - m_startLapTime;
	}
	return GetTime() - m_startLapTime;
}

//��������������������������������������������
//�������@�@�\�F�w�肵��FPS�̃t���[�����̃��b�v�^�C�����擾���܂�
//������ ����1�Ffloat�FFPS��
//������   �Ԓl�FDWORD�F���b�v�^�C��(�t���[����)
//��������������������������������������������
float	OGL::Timer::GetLapFrameTime(float fps)
{
	return float(GetLapTime()) / (1000 / fps);
}

//��������������������������������������������
//�������@�@�\�F���ԑ�����ꎞ��~���܂��B
//������ ����1�Fvoid
//������   �Ԓl�Fvoid
//��������������������������������������������
void		OGL::Timer::StopTime(void)
{
	if (m_stopFlg == false)
	{
		m_stopFlg = true;
		m_StartStopTime = GetTime();
	}
}

//��������������������������������������������
//�������@�@�\�F�ꎞ��~�������ԑ�����ĊJ�����܂��B
//������ ����1�Fvoid
//������   �Ԓl�Fvoid
//��������������������������������������������
void		OGL::Timer::StartTime(void)
{
	if (m_stopFlg == true)
	{
		m_stopFlg = false;
		m_startLapTime = m_startLapTime+(GetTime() - m_StartStopTime);
	}
}

//��������������������������������������������
//�������@�@�\�F���b�v�^�C���𕪒P�ʂŎ擾���܂��B
//������ ����1�Fvoid
//������   �Ԓl�FDWORD�F���b�v�^�C��(��)
//��������������������������������������������
DWORD	OGL::Timer::GetLapMin(void)
{
	return GetLapTime() / 60000;
}

//��������������������������������������������
//�������@�@�\�F���b�v�^�C����b�P�ʂŎ擾���܂��B
//������ ����1�Fvoid
//������   �Ԓl�FDWORD�F���b�v�^�C��(�b)
//��������������������������������������������
DWORD	OGL::Timer::GetLapSec(void)
{
	return (GetLapTime() / 1000)%60;
}

//��������������������������������������������
//�������@�@�\�F���b�v�^�C�����~���b�P�ʂŎ擾���܂��B
//������ ����1�Fvoid
//������   �Ԓl�FDWORD�F���b�v�^�C��(�~���b)
//��������������������������������������������
DWORD	OGL::Timer::GetLapMSec(void)
{
	return GetLapTime() % 1000;
}