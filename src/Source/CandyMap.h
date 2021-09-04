#include "Sound.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���}�G�a��:CandyMap
	/////////////////////////////////////////////////////////////////////////////
	class CandyMap {
	public:
		CandyMap();
		void Init();
		bool IsAlive();											// �O�_����
		void OnMove();											// ����
		void OnShow();
		void LoadBitmap();
		int IsEnd();//�a�ϼ��񧹲�
		int GetMH();
		int GetMW();
		int GetOnShowX(int realX);//�b�����e���W�۹諸X
		int GetOnShowY(int realY);//�b�����e���W�۹諸Y
		void SetBigTrans();
		void SetTurbleTrans();
		void SetMagnetTrans();
		void SetQucikMove();
		void SetAddLife();
		int GetNowY();
		bool GetIsTurbleMode();
		bool GetIsBigMode();
		bool GetIsOnFloor();
		int GetLifeNow();
		int HitCookieMan(CookieMan * cookie);					// �O�_�I�����l
		int mapChoose, mapNow, totalScore;
	private:
		CMovingBitmap bmpCandyGold, bmpCoin1, bmpCandySmile, bmpSkyStop1, bmpGroundStop1, bmpBonusTime;			// �y����
		CMovingBitmap bmpHeart, bmpFire, bmpTrap1, bmpWall1, bmpMagnet, bmpBig;
		CAnimation animationTrap, animationGroundStop, animationShine;
		CMovingBitmap bmpBarBorder;
		CMovingBitmap lifeScroll;
		const int MAX_LIFE;
		int MOVE_WIDTH;
		int TURBO_MOVE_WIDTH;
		int mapXTimes;
		int map[42][9][20];
		int effectMap[42][9][20];//���󪺯S��ĪG
		int mapY[42][9][20];//����y����
		int mapX[42][9][20];//����X��}
		int magnetYDistance[42][9][20];
		int magnetXDistance[42][9][20];
		int LIFE;
		int life, lifeX, lifeY;
		int MW, MH;					//�Ϫ��e��
		int manX1, manX2, manY1, manY2;
		int x, y;					// ��ߪ��y��
		int allDistance;
		int index;
		int turbleIndex;
		bool onFloor;
		int bigIndex;
		int turbleDistance;
		bool turble;			//�O�_�[�t�Ҧ�
		bool bigState;
		bool is_alive;				// �O�_����
		bool testMode;	//���ե\��Ҧ�
		bool magnetMode; //���K�Ҧ�
		int magnetTimes;
		int magnet;
		bool HitRectangle(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		int HitRectangleBox(int tx1, int ty1, int tx2, int ty2);
	};
}
