#ifndef COOKIE_MAN
#define COOKIE_MAN
#include "Sound.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C���H��:cookieMan
	/////////////////////////////////////////////////////////////////////////////
	class CookieMan {
	public:
		CookieMan();
		int  GetX1();					// ���l���W�� x �y��
		int  GetY1();					// ���l���W�� y �y��
		int  GetX2();					// ���l�k�U�� x �y��
		int  GetY2();					// ���l�k�U�� y �y��
		void Init();
		void LoadBitmap();
		void SetMovingUp(bool);		//���_��
		void SetMovingDown(bool flag);		//�w�U
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
		CAnimation cookieMan, cookieManDown, cookieManJump, cookieManOneJump, cookieManBonusTime, cookieManJumpOneBig, cookieManJumpBig;		// ���l���ʵe
		CAnimation cookieManBig;
		CMovingBitmap testLoad;
		const int JUMP_TOP;	//�����̰��I
		bool isBig;
		bool isOnFloor;
		bool upFlag;//�������A
		bool downFlag;
		bool isOut;
		int groundX, groundY;
		int nowX, nowY;
		int maxHeight;
		int gravity;//���O
		int check;
		int checkY;//���D�����I
		bool isLastMap;
	};
}
#endif
