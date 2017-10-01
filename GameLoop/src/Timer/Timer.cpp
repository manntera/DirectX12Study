//==========================
//===【Timer.cpp】
//==========================
//    時間を管理します。
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

//■■■■■■■■■■■//
//■■■【public】■■■//
//■■■■■■■■■■■//

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：プログラムの実行時間をミリ秒単位で取得します。
//■■■ 引数1：void
//■■■   返値：DWORD：実行時間(ミリ秒)
//■■■■■■■■■■■■■■■■■■■■■■
DWORD	OGL::Timer::GetTime(void)
{
	return timeGetTime();
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：ラップタイム開始します。
//■■■ 引数1：void
//■■■   返値：void
//■■■■■■■■■■■■■■■■■■■■■■
void		OGL::Timer::StartLapTime(void)
{
	m_startLapTime = GetTime();
	m_StartStopTime = m_startLapTime;
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：ラップタイム取得します。
//■■■ 引数1：void
//■■■   返値：DWORD：ラップタイム(ミリ秒)
//■■■■■■■■■■■■■■■■■■■■■■
DWORD	OGL::Timer::GetLapTime(void)
{
	if (m_stopFlg == true)
	{
		return m_StartStopTime - m_startLapTime;
	}
	return GetTime() - m_startLapTime;
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：指定したFPSのフレーム数のラップタイムを取得します
//■■■ 引数1：float：FPS数
//■■■   返値：DWORD：ラップタイム(フレーム数)
//■■■■■■■■■■■■■■■■■■■■■■
float	OGL::Timer::GetLapFrameTime(float fps)
{
	return float(GetLapTime()) / (1000 / fps);
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：時間測定を一時停止します。
//■■■ 引数1：void
//■■■   返値：void
//■■■■■■■■■■■■■■■■■■■■■■
void		OGL::Timer::StopTime(void)
{
	if (m_stopFlg == false)
	{
		m_stopFlg = true;
		m_StartStopTime = GetTime();
	}
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：一時停止した時間測定を再開させます。
//■■■ 引数1：void
//■■■   返値：void
//■■■■■■■■■■■■■■■■■■■■■■
void		OGL::Timer::StartTime(void)
{
	if (m_stopFlg == true)
	{
		m_stopFlg = false;
		m_startLapTime = m_startLapTime+(GetTime() - m_StartStopTime);
	}
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：ラップタイムを分単位で取得します。
//■■■ 引数1：void
//■■■   返値：DWORD：ラップタイム(分)
//■■■■■■■■■■■■■■■■■■■■■■
DWORD	OGL::Timer::GetLapMin(void)
{
	return GetLapTime() / 60000;
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：ラップタイムを秒単位で取得します。
//■■■ 引数1：void
//■■■   返値：DWORD：ラップタイム(秒)
//■■■■■■■■■■■■■■■■■■■■■■
DWORD	OGL::Timer::GetLapSec(void)
{
	return (GetLapTime() / 1000)%60;
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：ラップタイムをミリ秒単位で取得します。
//■■■ 引数1：void
//■■■   返値：DWORD：ラップタイム(ミリ秒)
//■■■■■■■■■■■■■■■■■■■■■■
DWORD	OGL::Timer::GetLapMSec(void)
{
	return GetLapTime() % 1000;
}