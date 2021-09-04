#ifndef COOKIE_MAN
#define COOKIE_MAN
#include "Sound.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲人物:cookieMan
	/////////////////////////////////////////////////////////////////////////////
	class CookieMan {
	public:
		CookieMan();
		int  GetX1();					// 擦子左上角 x 座標
		int  GetY1();					// 擦子左上角 y 座標
		int  GetX2();					// 擦子右下角 x 座標
		int  GetY2();					// 擦子右下角 y 座標
		void Init();
		void LoadBitmap();
		void SetMovingUp(bool);		//跳起來
		void SetMovingDown(bool flag);		//趴下
		void OnMove();
		void OnShow();
		void SetIsOnFloor(bool onFloor);
		void SetBig(bool turbleState);
		void SetNowY(int nowY);
		void MoveX(int x);
		void RunToEnd();
		void SetIsLastMap(bool mode);
		bool IsOutBorder();
		int bonusTime;
	private:
		CAnimation cookieMan, cookieManDown, cookieManJump, cookieManOneJump, cookieManBonusTime, cookieManJumpOneBig, cookieManJumpBig;		// 擦子的動畫
		CAnimation cookieManBig;
		CMovingBitmap testLoad;
		const int JUMP_TOP;	//跳的最高點
		bool isBig;
		bool isOnFloor;
		bool upFlag;//跳的狀態
		bool downFlag;
		bool isOut;
		int groundX, groundY;
		int nowX, nowY;
		int maxHeight;
		int gravity;//重力
		int check;
		int checkY;//跳躍的頂點
		bool isLastMap;
	};
}
#endif
