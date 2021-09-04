#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CookieMan.h"
#include "CandyMap.h"
#define PAN_Y		30		//��iMap�x�} y�b�V�U�����Z��->�]��
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class��CookieMan
	/////////////////////////////////////////////////////////////////////////////
	CookieMan::CookieMan() :JUMP_TOP(100)
	{
		Init();
	}
	void CookieMan::Init() {
		const int X_POS = 100;
		const int Y_POS = 250 + PAN_Y;
		const int JUMP_TIME = 2;
		const int MAX_HEIGHT = 100 + PAN_Y;
		nowX = groundX = X_POS;
		groundY = Y_POS;
		nowY = 200;
		maxHeight = MAX_HEIGHT;
		upFlag = downFlag = false;
		isBig = false;
		isOut = false;
		isOnFloor = false;
		check = checkY = 0;
		bonusTime = 0;
		isLastMap = false;
	}
	void CookieMan::LoadBitmap() {
		if (testLoad.isBitmapLoad() == false) {
			testLoad.LoadBitmap(IDB_testLoad);
			cookieMan.AddBitmap(IDB_cookieMan1, RGB(255, 255, 255));
			cookieMan.AddBitmap(IDB_cookieMan1_run1, RGB(255, 255, 255));
			cookieMan.AddBitmap(IDB_cookieMan1_run2, RGB(255, 255, 255));
			cookieMan.AddBitmap(IDB_cookieMan1_run3, RGB(255, 255, 255));
			cookieManDown.AddBitmap(IDB_cookieMan1_down4, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_1, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_2, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_3, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_4, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_3, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_4, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_3, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_4, RGB(255, 255, 255));
			cookieManBonusTime.AddBitmap(IDB_bonus_3, RGB(255, 255, 255));
			cookieManOneJump.AddBitmap(IDB_JUMP_0, RGB(255, 255, 255));
			cookieManJump.AddBitmap(IDB_jump_3, RGB(255, 255, 255));
			cookieManJump.AddBitmap(IDB_jump_4, RGB(255, 255, 255));
			cookieManJump.AddBitmap(IDB_jump_5, RGB(255, 255, 255));
			cookieManJump.AddBitmap(IDB_jump_7, RGB(255, 255, 255));
			cookieManBig.AddBitmap(IDB_cookieManBig, RGB(255, 255, 255));			//�H���ܤj
			cookieManBig.AddBitmap(IDB_cookieManBig_run1, RGB(255, 255, 255));
			cookieManBig.AddBitmap(IDB_cookieManBig_run2, RGB(255, 255, 255));
			cookieManBig.AddBitmap(IDB_cookieManBig_run3, RGB(255, 255, 255));
			cookieManJumpOneBig.AddBitmap(IDB_jumpBig0, RGB(255, 255, 255));
			cookieManJumpBig.AddBitmap(IDB_jumpBig2, RGB(255, 255, 255));
			cookieManJumpBig.AddBitmap(IDB_jumpBig3, RGB(255, 255, 255));
			cookieManJumpBig.AddBitmap(IDB_jumpBig4, RGB(255, 255, 255));
			cookieManJumpBig.AddBitmap(IDB_jumpBig5, RGB(255, 255, 255));
		}
	}
	void CookieMan::SetMovingUp(bool flag) {
		if (bonusTime == 2) {
			upFlag = flag;
			return;
		}
		if (flag == true) {
			//�{�b�H����b�a�O
			if (isOnFloor == true) {
				check = 0;
				checkY = nowY;
				upFlag = flag;
			}
			//�H�����A���Ť����D�B����G�q��
			if (check == 1) {
				upFlag = true;
			}
			//�H�����A���Ť����D�@�����w
			if (isOnFloor == false && check == 0) {
				check = 1;
				upFlag = flag;
				checkY = nowY;
			}
		}
	}
	void CookieMan::SetMovingDown(bool flag) {
		//���b�a�O�ɤ~�i�H�ۤU
		//�S���ۤU��downFlag set False
		cookieManDown.Reset(); //�ۤU�ʵe ���s	   
		if (flag == false) {//�ۤU�䬰false 
			downFlag = flag;
		}
		if (isOnFloor&& flag == true) {//��b�a�O��and �ۤU�䬰true
			downFlag = flag;
			nowY = nowY;
		}
	}
	void CookieMan::OnMove()//��q���̰����Z���O150*2==300
	{
		const int JUMP_DISTANCE = 30;	//�V�W�[�t��
		const int GRAVITY = 15;		//�V�U�[�t��
		gravity = GRAVITY;
		//�P�_�����̰��W��
		cookieManDown.SetDelayCount(1);    //�ۤU�ʵe
		cookieManDown.OnMove();
		cookieManJump.SetDelayCount(4);  //���_�Ӱʵe
		cookieManJump.OnMove();
		cookieManOneJump.SetDelayCount(4);
		cookieManJump.OnMove();
		cookieManJumpBig.SetDelayCount(4);
		cookieManJumpBig.OnMove();
		cookieManJumpOneBig.SetDelayCount(4);
		cookieManJumpOneBig.OnMove();
		cookieMan.SetDelayCount(2);  //���q�ʵe
		cookieMan.OnMove();
		cookieManBig.SetDelayCount(2);
		cookieManBig.OnMove();
		if (bonusTime == 1) {
			cookieManBonusTime.SetDelayCount(25);
			cookieManBonusTime.OnMove();
			if (cookieManBonusTime.GetCurrentBitmapNumber() == 8)
			{
				upFlag = 0;
				bonusTime = 2;
				cookieManBonusTime.Reset();
			}
			if (nowY >= 20) {
				nowY -= 2;
			}
			return;
		}
		if (bonusTime == 2)
		{
			if (upFlag == 1 && nowY >= 0)
			{
				nowY -= 12;
			}
			if (nowY <= 380) {
				nowY += 6;
			}
			return;
		}
		if (check == 1) {
			upFlag = 1;		//�������A
			if (nowY <= checkY - 150 || nowY <= 0) {
				upFlag = 0;
				check = 2;
			}
		}
		if (upFlag == 1) {
			if (nowY >= 0)
			{
				//CAudio::Instance()->Play(JUMP_1, false);
				nowY -= JUMP_DISTANCE;
			}
			if ((nowY <= checkY - 200 && check == 0)) {
				upFlag = 0;
			}
			if (nowY <= 0)
			{
				upFlag = 0;
			}
		}
		if (isOnFloor == false) //���_�Ӫ��A,�a�O�W��
		{
			nowY += gravity;
		}
		else if (nowY >= groundY &&downFlag == false)//��b�a�O�W
		{
			nowY = groundY;
			upFlag = false;
		}
		if (nowY > SIZE_Y)//�W�X���Y
		{
			isOut = true; //���J�a�O�U
		}
		if (isLastMap == true && nowX <= SIZE_X) {//������i�a�Ϫ��̫�@���F
			this->RunToEnd();
		}
	}
	void CookieMan::OnShow()
	{
		if (bonusTime == 1) {
			cookieManBonusTime.SetTopLeft(nowX, nowY);
			cookieManBonusTime.OnShow();
		}
		else if (bonusTime == 2)
		{
			cookieManOneJump.SetTopLeft(nowX, nowY);
			cookieManOneJump.OnShow();
		}
		else if (downFlag == 1) {
			cookieManDown.SetTopLeft(nowX, nowY);
			cookieManDown.OnShow();
		}
		else if (isBig == true && check == 0 && isOnFloor == false) {
			cookieManJumpOneBig.SetTopLeft(nowX, nowY);
			cookieManJumpOneBig.OnShow();
		}
		else if (isBig == true && check != 0 && isOnFloor == false)
		{
			cookieManJumpBig.SetTopLeft(nowX, nowY);
			cookieManJumpBig.OnShow();
		}
		else if (isBig == true) {
			cookieManBig.SetTopLeft(nowX, nowY);
			cookieManBig.OnShow();
		}
		else if (check == 0 && isOnFloor == false)//||isOnFloor==false)
		{
			cookieManOneJump.SetTopLeft(nowX, nowY);
			cookieManOneJump.OnShow();
		}
		else if (check != 0 && isOnFloor == false)//||isOnFloor==false)
		{
			cookieManJump.SetTopLeft(nowX, nowY);
			cookieManJump.OnShow();
		}
		else {
			cookieMan.SetTopLeft(nowX, nowY);
			cookieMan.OnShow();
		}
	}
	void CookieMan::SetBig(bool bigState) {
		if (isBig == true && bigState == false) { //���ܪ��A: �ܤj���A�Y�N�����A�ܦ^��l���A
			isBig = bigState;
			nowY = SIZE_Y - cookieMan.Height() - 2 * 50;
			groundY = nowY;
		}
		else if (isBig == false && bigState == true) {						// ���ܪ��A:�@�q�p�ܦ��j���A1
			isBig = bigState;
			nowY = SIZE_Y - cookieManBig.Height() - 2 * 50;
			groundY = nowY;
		}
		else if (isBig == false) {						//�����쪬�A
			isBig = bigState;
		}
	}
	bool CookieMan::IsOutBorder() {
		return isOut;
	}
	void CookieMan::SetIsOnFloor(bool onFloor) {
		isOnFloor = onFloor;
	}
	void CookieMan::MoveX(int x) {
		nowX += x;
	}
	void CookieMan::RunToEnd() {
		if (nowX <= SIZE_X) {
			this->MoveX(10);
		}
	}
	void CookieMan::SetNowY(int y)
	{
		nowY = y;
	}
	void CookieMan::SetIsLastMap(bool mode) {
		isLastMap = mode;
	}
	int CookieMan::GetX1()
	{
		return nowX;
	}
	int CookieMan::GetY1()
	{
		return nowY;
	}
	int CookieMan::GetX2()
	{
		if (isBig) return nowX + cookieManBig.Width();
		else if (downFlag)return nowX + cookieManDown.Width();
		else return nowX + cookieMan.Width();
	}
	int CookieMan::GetY2()
	{
		if (isBig&&downFlag == false)return nowY + cookieManBig.Height();
		else if (downFlag)
			return nowY + cookieManDown.Height();
		else return nowY + cookieMan.Height();
	}
}
