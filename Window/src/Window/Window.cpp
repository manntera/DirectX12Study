#include "Window.h"

using namespace OGL;

Window::Window()
{}

Window::~Window()
{}

void	Window::SetHinstance(HINSTANCE*	hInstance)
{
	mp_hInstance = hInstance;
}

void	Window::SetWindowName(char* windowName)
{
	m_windowName = windowName;
}

void	Window::SetWindowSize(int width, int height)
{
	m_winWidth = width;
	m_winHeight = height;
}

void	Window::CreateShowWindow(int nCmdShow, WNDPROC wndProc)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = *mp_hInstance;
	wc.lpszClassName = m_windowName.c_str();
	wc.lpfnWndProc = wndProc;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "ウィンドウクラスの構造体の初期化エラー", "", MB_OK);
	}
	m_hWnd = CreateWindow(wc.lpszClassName,
		m_windowName.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_winWidth,
		m_winHeight,
		NULL,
		NULL,
		*mp_hInstance,
		NULL);
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
}

HINSTANCE*	Window::GetHinstance(void)
{
	return mp_hInstance;
}

HWND*			Window::GetHwnd(void)
{
	return &m_hWnd;
}

int	Window::GetWidth(void)
{
	return m_winWidth;
}

int	Window::GetHeight(void)
{
	return m_winHeight;
}