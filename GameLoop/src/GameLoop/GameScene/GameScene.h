//==========================
//===【GameScene.h】
//==========================
//    ゲームの本体処理を記述します。
//    このクラスを継承して使用して下さい
//==========================
#pragma	once
//=====================//
//===【インクルード記述】 ===//
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
