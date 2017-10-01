#include"Input.h"
#include<stdio.h>

using namespace OGL;

Input::Input()
{
	memset(m_inputHistory, 0, sizeof(char) * 255);
	
}
void Input::UpdateInput(void)
{
	for (int i = 0; i < 255;i++) {
		m_inputHistory[i]= m_inputHistory[i] << 1;
		if (GetAsyncKeyState(i) & 0x8000) {
			m_inputHistory[i] = m_inputHistory[i] | 0x01;
		}
	}
}
bool Input::KeyPress(int vKey)
{
	if ((m_inputHistory[vKey] & 0x01)) {
		return true;
	}
	return false;
}
bool Input::KeyTrigger(int vKey)
{
	if (!(m_inputHistory[vKey] & 0x02) && (m_inputHistory[vKey] & 0x01)) {
		return true;
	}
	return false;
}
bool Input::KeyRelease(int vKey)
{
	if ((m_inputHistory[vKey] & 0x02) && !(m_inputHistory[vKey] & 0x01)) {
		return true;
	}
	return false;
}