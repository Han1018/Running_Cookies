#include "Sound.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為糖果地圖:CandyMap
	/////////////////////////////////////////////////////////////////////////////
	class CandyMap {
	public:
		CandyMap();
		void Init();
		bool IsAlive();											// 是否活著
		void OnMove();											// 移動
		void OnShow();
		void LoadBitmap();
		int IsEnd();//地圖播放完畢
		int GetMH();
		int GetMW();
		int GetOnShowX(int realX);//在視窗畫面上相對的X
		int GetOnShowY(int realY);//在視窗畫面上相對的Y
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
		int HitCookieMan(CookieMan * cookie);					// 是否碰到擦子
		int mapChoose, mapNow, totalScore;
	private:
		CMovingBitmap bmpCandyGold, bmpCoin1, bmpCandySmile, bmpSkyStop1, bmpGroundStop1, bmpBonusTime;			// 球的圖
		CMovingBitmap bmpHeart, bmpFire, bmpTrap1, bmpWall1, bmpMagnet, bmpBig;
		CAnimation animationTrap, animationGroundStop, animationShine;
		CMovingBitmap bmpBarBorder;
		CMovingBitmap lifeScroll;
		const int MAX_LIFE;
		int MOVE_WIDTH;
		int TURBO_MOVE_WIDTH;
		int mapXTimes;
		int map[42][9][20];
		int effectMap[42][9][20];//物件的特殊效果
		int mapY[42][9][20];//物件的y高度
		int mapX[42][9][20];//物件的X位址
		int magnetYDistance[42][9][20];
		int magnetXDistance[42][9][20];
		int LIFE;
		int life, lifeX, lifeY;
		int MW, MH;					//圖的寬高
		int manX1, manX2, manY1, manY2;
		int x, y;					// 圓心的座標
		int allDistance;
		int index;
		int turbleIndex;
		bool onFloor;
		int bigIndex;
		int turbleDistance;
		bool turble;			//是否加速模式
		bool bigState;
		bool is_alive;				// 是否活著
		bool testMode;	//測試功能模式
		bool magnetMode; //磁鐵模式
		int magnetTimes;
		int magnet;
		bool HitRectangle(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		int HitRectangleBox(int tx1, int ty1, int tx2, int ty2);
	};
}
