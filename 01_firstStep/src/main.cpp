//��--------------------------------��//
//��---�ymain.cpp�z ----------------��//
//��--------------------------------��//
//��--�G���g���|�C���g���L�q���܂��B��//
//��--------------------------------��//

//��-------------------------��//
//��---�y�C���N���[�h�L�q�z--��//
//��-------------------------��//
#include<OglGameLoop.h>
#pragma comment(lib,"OglGameLoop.lib")
#include<OglWindow.h>
#pragma comment(lib,"OglWindow.lib")

#include"GameLoop\Demo1\Demo1.h"

#include <memory>
using namespace OGL;

//��----------------��//
//��--�y�O���錾�z--��//
//��----------------��//
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//��-----------------------------��
//��--	�ywWinMain�z ------------��
//��--	�@�\�F�G���g���|�C���g --��
//��-----------------------------��
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(74);
#endif
	Window* window = Window::GetInstance();
	window->SetHinstance(&hInstance);
	window->SetWindowName("DirectX12Demo");
	window->SetWindowSize(1280, 720);
	window->CreateShowWindow(nCmdShow, (WNDPROC)WndProc);

	GameLoopManager* glm = GameLoopManager::GetInstance();
	std::unique_ptr<OGL::GameScene> entryScene = std::make_unique<OGL::Demo1>();
	glm->BeginGameLoopManager(60, entryScene.get());
	// Main message loop
	MSG msg = { 0 };
	while (1) {
		if (!glm->IsLoopFlg()) {
			SendMessage(*Window::GetInstance()->GetHwnd(), WM_CLOSE, 0, 0);
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0) == 0) {
				glm->EndGameLoopManager();
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//��----------------------------------��
//��--	�yWndProc�z ------------------��
//��--	�@�\�F�E�B���h�E�v���V�[�W��--��
//��--		�@���L�q���܂��B�@�@�@�@--��
//��----------------------------------��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) {
	case	WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}