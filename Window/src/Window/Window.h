#pragma once
#include<Windows.h>
#include<string>

namespace OGL
{
	class Window
	{
	public:
		void		SetHinstance(HINSTANCE*	hInstance);
		void		SetWindowName(char* windowName);
		void		SetWindowSize(int width, int height);
		void		CreateShowWindow(int nCmdShow, WNDPROC wndProc);
		HINSTANCE*	GetHinstance(void);
		HWND*		GetHwnd(void);
		int			GetWidth(void);
		int			GetHeight(void);

	private:
		HINSTANCE*	mp_hInstance;
		HWND		m_hWnd;
		int			m_winWidth;
		int			m_winHeight;
		std::string	m_windowName;

	public:
		static	Window*	GetInstance(void)
		{
			static Window	instance;
			return &instance;
		}
		virtual ~Window();

	private:
		Window();
		Window(const Window &hoge) {};
		Window &operator=(const Window &) { return *this; }
	};
}
