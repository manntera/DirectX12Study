#pragma once
#include<Windows.h>

namespace OGL
{
	class	Input
	{
	public:
		void UpdateInput(void);
		bool KeyPress(int vKey);
		bool KeyTrigger(int vKey);
		bool KeyRelease(int vKey);
	private:
		char m_inputHistory[255];
	public:
		static	Input*	GetInstance(void)
		{
			static Input	instance;
			return &instance;
		}
		virtual ~Input() {};

	private:
		Input();
		Input(const Input &hoge) {};
		Input &operator=(const Input &) { return *this; }



	};	//EndOf__class_Input
}