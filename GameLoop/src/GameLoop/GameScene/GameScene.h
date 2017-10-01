//==========================
//===�yGameScene.h�z
//==========================
//    �Q�[���̖{�̏������L�q���܂��B
//    ���̃N���X���p�����Ďg�p���ĉ�����
//==========================
#pragma	once
//=====================//
//===�y�C���N���[�h�L�q�z ===//
//=====================//
#include<memory>

namespace	OGL
{
	class GameScene
	{
	public:
		GameScene();
		virtual ~GameScene();
		GameScene(const GameScene &hoge){};
		GameScene &operator=(const GameScene &){ return *this; }
	public:
		virtual	bool	Init(void)=0;
		virtual	bool	Update(void) = 0;
		virtual	bool	Draw(void) = 0;
		virtual	bool	Uninit(void) = 0;
		void operator delete(void *p) {
			return _mm_free(p);
		}
	protected:
	};	//EndOf__class_GameLoop
}	//EndOf__namespace_OGL
