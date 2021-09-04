/*
* mygame.cpp: 本檔案儲遊戲本身的class的implementation
* Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
*
* This file is part of game, a free game development framework for windows.
*
* game is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* game is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* History:
*   2002-03-04 V3.1
*          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*      2. Demo the use of CInteger in CGameStateRun.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
*   2006-02-08 V4.2
*      1. Revise sample screens to display in English only.
*      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
*      3. Rename OnInitialUpdate() -> OnInit().
*      4. Fix the bug that OnBeginState() of GameStateInit is not called.
*      5. Replace AUDIO_CANYON as AUDIO_NTUT.
*      6. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
*         event driven.
*   2006-12-30
*      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
*         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*   2010-03-23 V4.6
*      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <stdio.h>
#define ABOUT_PAGE 1
#define SKILL_PAGE 2
bool stage[3] = { false,false,false };//遊戲關卡破關狀態
int stageScore = 0; //分數
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
		backGround = 0;
		page = 1;
	}
	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
								//
								// 開始載入資料
								//
		logo.LoadBitmap(IDB_start);
		about.LoadBitmap(IDB_about, RGB(255, 255, 255));
		explain.LoadBitmap(IDB_explain, RGB(255, 255, 255));
		aboutBackground.LoadBitmap(IDB_aboutBackground);
		explainBackground1.LoadBitmap(IDB_explainBackground1);
		explainBackground2.LoadBitmap(IDB_explainBackground2);
		explainBackground3.LoadBitmap(IDB_explainBackground3);
		explainBackground4.LoadBitmap(IDB_explainBackground4);
		page1.LoadBitmap(IDB_page1);
		page2.LoadBitmap(IDB_page2);
		page3.LoadBitmap(IDB_page3);
		page4.LoadBitmap(IDB_page4);
		back.LoadBitmap(IDB_BACK);
		playHover = false;
		play.LoadBitmap(IDB_PLAY, RGB(255, 255, 255));
		pointer.LoadBitmap(IDB_pointer, RGB(255, 255, 255));
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
								//
								// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
								//
	}
	void CGameStateInit::OnBeginState()
	{
	}
	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}
	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)//這個地方可以改成偵測滑鼠的位址選擇指定關卡
	{
		if (point.x>195 && point.x < 445 && point.y>235 && point.y<301)
		{
			GotoGameState(GAME_STATE_STAGE);
		}
		if (point.x > 195 && point.x < 445 && point.y>315 && point.y < 381)
		{
			backGround = ABOUT_PAGE;
		}
		if (backGround == ABOUT_PAGE)
		{
			if (point.x > 500 && point.x < 597 && point.y>420 && point.y < 447) {
				backGround = 0;
			}
		}
		if (backGround == SKILL_PAGE)
		{
			if (point.x > 500 && point.x < 597 && point.y>420 && point.y < 447) {
				backGround = 0;
			}
			if (point.x > 450 && point.x < 485 && point.y>420 && point.y < 458) page = 4;
			if (point.x > 400 && point.x < 435 && point.y>420 && point.y < 458) page = 3;
			if (point.x > 350 && point.x < 385 && point.y>420 && point.y < 458) page = 2;
			if (point.x > 300 && point.x < 335 && point.y>420 && point.y < 458) page = 1;
		}
		else if (point.x > 195 && point.x < 445 && point.y>395 && point.y < 461)
		{
			backGround = SKILL_PAGE;
		}
	}
	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		if (point.x > 195 && point.x < 445 && point.y>235 && point.y < 301) {//第一關
			playHover = true;
			pointerPosition = 240;
		}
		else if (point.x > 195 && point.x <445 && point.y>315 && point.y<381) {//第二關
			playHover = true;
			pointerPosition = 320;
		}
		else if (point.x > 195 && point.x < 445 && point.y>395 && point.y < 461) {//Back鍵
			playHover = true;
			pointerPosition = 400;
		}
		else playHover = false;
	}
	void CGameStateInit::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
		if (backGround == 0) {
			page = 1;
			logo.SetTopLeft(0, 0);
			logo.ShowBitmap();
			explain.SetTopLeft(195, 395);
			explain.ShowBitmap();
			about.SetTopLeft(195, 315);
			about.ShowBitmap();
			play.SetTopLeft(195, 235);
			play.ShowBitmap();
			if (playHover == true) {
				pointer.SetTopLeft(80, pointerPosition);
				pointer.ShowBitmap();
			}
		}
		if (backGround == ABOUT_PAGE)//about
		{
			aboutBackground.SetTopLeft(0, 0);
			aboutBackground.ShowBitmap();
			back.SetTopLeft(500, 420);
			back.ShowBitmap();
		}
		if (backGround == SKILL_PAGE)//說明密技頁面
		{
			if (page == 1)
			{
				explainBackground1.SetTopLeft(0, 0);
				explainBackground1.ShowBitmap();
			}
			if (page == 2)
			{
				explainBackground2.SetTopLeft(0, 0);
				explainBackground2.ShowBitmap();
			}
			if (page == 3)
			{
				explainBackground3.SetTopLeft(0, 0);
				explainBackground3.ShowBitmap();
			}
			if (page == 4)
			{
				explainBackground4.SetTopLeft(0, 0);
				explainBackground4.ShowBitmap();
			}
			back.SetTopLeft(500, 420);
			back.ShowBitmap();
			page4.SetTopLeft(450, 420);
			page4.ShowBitmap();
			page3.SetTopLeft(400, 420);
			page3.ShowBitmap();
			page2.SetTopLeft(350, 420);
			page2.ShowBitmap();
			page1.SetTopLeft(300, 420);
			page1.ShowBitmap();
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的選擇狀態(Stage)
	/////////////////////////////////////////////////////////////////////////////
	CGameStateStage::CGameStateStage(CGame *g) : CGameState(g)
	{
	}
	CGameStateStage::~CGameStateStage()
	{
	}
	void CGameStateStage::OnInit()
	{
		land1.LoadBitmap(IDB_LAND_1);
		land2.LoadBitmap(IDB_LAND_2);
		land1_Hover.LoadBitmap(IDB_LAND_1_HOVER);
		land2_Hover.LoadBitmap(IDB_LAND_2_HOVER);
		back.LoadBitmap(IDB_BACK);
		lock.LoadBitmap(IDB_lock, RGB(255, 255, 255));
		//lock.LoadBitmap(IDB_lock, RGB(255, 255, 255));
		back_Hover.LoadBitmap(IDB_back_hover);
		stageGround.LoadBitmap(IDB_STAGE_GROUND);
	}
	void CGameStateStage::OnLButtonDown(UINT nFlags, CPoint point)//這個地方可以改成偵測滑鼠的位址選擇指定關卡
	{
		if (point.x > 500 && point.x < 600 && point.y>420 && point.y < 449)
		{
			GotoGameState(GAME_STATE_INIT);
		}
		if (point.x > 70 && point.x < 289 && point.y>102 && point.y <  322)
		{
			GotoGameState(GAME_STATE_RUN);
		}
		if (point.x >350 && point.x < 579 && point.y>102 && point.y < 322 && stage[0] == true)
		{
			GotoGameState(GAME_STATE_RUN_2);
		}
	}
	void CGameStateStage::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		if (point.x > 70 && point.x < 289 && point.y>102 && point.y <  322)
			Land1Hover = true;
		else Land1Hover = false;
		if (point.x > 350 && point.x < 579 && point.y>102 && point.y < 322)	Land2Hover = true;
		else Land2Hover = false;
		if (point.x > 500 && point.x < 597 && point.y>420 && point.y < 447)
			backHover = true;
		else backHover = false;
	}
	void CGameStateStage::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		const char KEY_A = 65;
		const char KEY_S = 83;
		if (nChar == KEY_A) {
			stage[0] = true;
			stage[1] = true;
		}
		if (nChar == KEY_S) {
			stage[0] = false;
			stage[1] = false;
		}
	}
	void CGameStateStage::OnShow()
	{
		stageGround.SetTopLeft(0, 0);
		stageGround.ShowBitmap();
		if (Land1Hover == true) {
			land1_Hover.SetTopLeft(70, 102);
			land1_Hover.ShowBitmap();
		}
		else
		{
			land1.SetTopLeft(70, 87);
			land1.ShowBitmap();
		}
		if (stage[0] == false) {
			land2_Hover.SetTopLeft(350, 87);
			land2_Hover.ShowBitmap();
		}
		else if (Land2Hover == true) {
			land2_Hover.SetTopLeft(350, 102);
			land2_Hover.ShowBitmap();
		}
		else {
			land2.SetTopLeft(350, 87);
			land2.ShowBitmap();
		}
		if (backHover == true) {
			back_Hover.SetTopLeft(500, 420);
			back_Hover.ShowBitmap();
		}
		else {
			back.SetTopLeft(500, 420);
			back.ShowBitmap();
		}
		if (stage[0] == false) {
			lock.SetTopLeft(156, 25);
			lock.ShowBitmap();
		}
		if (stage[1] == false) {
			lock.SetTopLeft(436, 25);
			lock.ShowBitmap();
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////
	CGameStateOver::CGameStateOver(CGame *g)
		:CGameState(g)
	{
	}
	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		score.SetInteger(0);
		score.SetTopLeft(230, 250);
	}
	void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point) {
		if (point.x > 205 && point.x < 406 && point.y>406 && point.y < 473)
		{
			GotoGameState(GAME_STATE_INIT);
		}
	}
	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		bmpEndGame1.LoadBitmap(IDB_endGame1);
		bmpConfirm.LoadBitmap(IDB_CONFIRM, RGB(255, 255, 255));
		gameOver.LoadBitmap(IDB_gameover, RGB(255, 255, 255));
		congratulation.LoadBitmap(IDB_congratulation, RGB(255, 255, 255));
		score.LoadBitmap();
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
								//
								// 開始載入資料
								//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
								//
								// 最終進度為100%
								//
		ShowInitProgress(100);
	}
	void CGameStateOver::OnMove()
	{
		//counter--;
		//if (counter < 0)
		//	GotoGameState(GAME_STATE_INIT);
	}
	void CGameStateOver::OnShow()
	{
		if (score.GetInteger() != stageScore) {
			score.Add(stageScore);
		}
		//GAMEOVER畫面
		bmpEndGame1.SetTopLeft(0, 0);
		bmpEndGame1.ShowBitmap();
		bmpConfirm.SetTopLeft(205, 406);
		bmpConfirm.ShowBitmap();
		if (stage[2] == 0) {//GAMEOVER死亡狀態
			gameOver.SetTopLeft(100, 100);
			gameOver.ShowBitmap();
		}
		else if (stage[2] == 1) {//勝利狀態
			congratulation.SetTopLeft(100, 100);
			congratulation.ShowBitmap();
		}
		score.ShowBitmap();
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////
	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
		bonusTimeState = 0;
	}
	CGameStateRun::~CGameStateRun()
	{
		//delete [] ball;
	}
	void CGameStateRun::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 0;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;
		const int CANDY_PER_ROW = 15;
		const int CANDY_GAP = 10;
		cMan.Init();
		cMap.Init();
		cMap.LoadBitmap();
		cBackGround.LoadBitmap();
		stage[2] = 0;
		help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
	}
	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
								//
								// 開始載入資料
								//
		cMan.LoadBitmap();
		cBackGround.LoadBitmap();
		//background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
		//
		// 完成部分Loading動作，提高進度
		//
		ShowInitProgress(50);
		Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
					//
					// 繼續載入其他資料
					//
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
		corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
		hits_left.LoadBitmap();
	}
	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_Q = 81;
		const char KEY_W = 87;
		const char KEY_E = 69;
		const char KEY_R = 82;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_Q)	cMap.SetBigTrans();
		if (nChar == KEY_W) cMap.SetTurbleTrans();
		if (nChar == KEY_E) cMap.SetMagnetTrans();
		if (nChar == KEY_R) cMap.SetAddLife();
		if (nChar == KEY_SPACE) cMap.SetQucikMove();
		if (nChar == KEY_UP) cMan.SetMovingUp(true);
		if (nChar == KEY_DOWN)cMan.SetMovingDown(true);
	}
	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		if (nChar == KEY_UP) {
			cMan.SetMovingUp(false);
		}
		if (nChar == KEY_DOWN) {
			cMan.SetMovingDown(false);
		}
	}
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(true);
	}
	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(false);
	}
	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}
	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingRight(true);
	}
	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingRight(false);
	}
	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		if (cMap.IsEnd()) {
			cMan.SetIsLastMap(true);
			if (cMan.GetX1() > SIZE_X) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				success = 1;
				stageScore = hits_left.GetInteger();
				GotoGameState(GAME_STATE_OVER);
			}
		}
		else {
			cMan.SetIsLastMap(false);
		}
		cMan.OnMove();
		cMap.OnMove();
		cBackGround.OnMove();
		int score = cMap.HitCookieMan(&cMan);	//判斷每次移動index會撞到的分數或是不適障礙物 score=-1 表示撞到障礙物
		if (score >= 0) {
			hits_left.Add(score);//碰撞後加分
		}
		if (score == -1) {//初始得分狀態
			cBackGround.status = 1;
		}
		cMan.SetBig(cMap.GetIsBigMode());
		cMan.SetIsOnFloor(cMap.GetIsOnFloor());
		cMan.SetNowY(cMap.GetNowY());
		if (cMap.GetLifeNow() <= 0 || cMan.IsOutBorder() == 1) //cMap.IsEnd() == 1 
		{
			CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
			CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
			success = 0;
			stageScore = hits_left.GetInteger();
			GotoGameState(GAME_STATE_OVER);
		}
		if (cMap.IsEnd() == 1)//跑完關卡
		{
			cBackGround.MoveState(false);
			stage[0] = true;
			stage[2] = true;
		}
		if (score == -2)
		{
			cMan.bonusTime = 1;
			score = 0;
		}
		if (cMan.bonusTime == 2 && bonusTimeState != 2)//進入bonusTime狀態 
		{
			bonusTimeState = 1;
		}
		if (bonusTimeState == 1)//預備bonusTime的動畫
		{
			cBackGround.choose = 1;
			cMap.mapNow = cMap.mapChoose;
			cMap.mapChoose = 35;
			cMap.SetTurbleTrans();
			cMap.SetMagnetTrans();
			bonusTimeState = 2;
		}
		if (cMap.mapChoose == 40)//BonusTime走道終點，結束回到原本地圖
		{
			cBackGround.choose = 0;
			cMap.mapChoose = cMap.mapNow;
			cMan.SetNowY(0);
			cMap.SetTurbleTrans();
			cMap.SetMagnetTrans();
			cMan.bonusTime = 0;
			bonusTimeState = 0;
		}
	}
	void CGameStateRun::OnShow()
	{
		cBackGround.OnShow();
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		help.ShowBitmap();					// 貼上說明圖
		hits_left.ShowBitmap();
		//
		//  貼上左上及右下角落的圖
		//
		corner.SetTopLeft(0, 0);
		corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		corner.ShowBitmap();
		cMap.OnShow();
		cMan.OnShow();					// 貼上人物
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////
	CGameStateRun_2::CGameStateRun_2(CGame *g) : CGameState(g)
	{
		bonusTimeState = 0;
	}
	CGameStateRun_2::~CGameStateRun_2()
	{
		//delete [] ball;
	}
	void CGameStateRun_2::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 0;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;
		const int CANDY_PER_ROW = 15;
		const int CANDY_GAP = 10;
		cMan.Init();
		cMap.Init();
		cMap.LoadBitmap();
		cBackGround.LoadBitmap();
		stage[2] = 0;
		hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
	}
	void CGameStateRun_2::OnInit()
	{
		cMan.LoadBitmap();
		cBackGround.LoadBitmap();
		hits_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\background2.mp3");	// 載入編號2的聲音ntut.mid
	}
	void CGameStateRun_2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_Q = 81;
		const char KEY_W = 87;
		const char KEY_E = 69;
		const char KEY_R = 82;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_Q)	cMap.SetBigTrans();
		if (nChar == KEY_W) cMap.SetTurbleTrans();
		if (nChar == KEY_E) cMap.SetMagnetTrans();
		if (nChar == KEY_R) cMap.SetAddLife();
		if (nChar == KEY_SPACE) cMap.SetQucikMove();
		if (nChar == KEY_UP) {
			cMan.SetMovingUp(true);
		}
		if (nChar == KEY_DOWN)
			cMan.SetMovingDown(true);
	}
	void CGameStateRun_2::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		if (nChar == KEY_UP) {
			cMan.SetMovingUp(false);
		}
		if (nChar == KEY_DOWN) {
			cMan.SetMovingDown(false);
		}
	}
	void CGameStateRun_2::OnMove()							// 移動遊戲元素
	{
		if (cMap.IsEnd()) {
			cMan.SetIsLastMap(true);
			if (cMan.GetX1() > SIZE_X) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				stageScore = hits_left.GetInteger();
				GotoGameState(GAME_STATE_OVER);
			}
		}
		else {
			cMan.SetIsLastMap(false);
		}
		cMan.OnMove();
		cMap.OnMove();
		cBackGround.OnMove();
		int score = cMap.HitCookieMan(&cMan);	//判斷每次移動index會撞到的分數或是不適障礙物 score=-1 表示撞到障礙物
		if (score >= 0) {
			hits_left.Add(score);//碰撞後加分
		}
		if (score == -1) {
			cBackGround.status = 1;
		}
		cMan.SetBig(cMap.GetIsBigMode());
		cMan.SetIsOnFloor(cMap.GetIsOnFloor());
		cMan.SetNowY(cMap.GetNowY());
		if (cMap.GetLifeNow() <= 0 || cMan.IsOutBorder() == 1)//cMap.IsEnd() == 1 ||
		{
			CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
			CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
			stageScore = hits_left.GetInteger();
			GotoGameState(GAME_STATE_OVER);
		}
		if (cMap.IsEnd() == 1)
		{
			cBackGround.MoveState(false);
			stage[1] = true;
			stage[2] = true;
		}
		if (score == -2)
		{
			cMan.bonusTime = 1;
			score = 0;
		}
		if (cMan.bonusTime == 2 && bonusTimeState != 2)//進入bonusTime狀態 
		{
			bonusTimeState = 1;
		}
		if (bonusTimeState == 1)//預備bonusTime動畫
		{
			cBackGround.choose = 1;
			cMap.mapNow = cMap.mapChoose;
			cMap.mapChoose = 15;
			cMap.SetTurbleTrans();
			cMap.SetMagnetTrans();
			bonusTimeState = 2;
		}
		if (cMap.mapChoose == 19) {
			cBackGround.choose = 0;
			cMap.mapChoose = cMap.mapNow;
			cMan.bonusTime = 0;
			cMap.SetTurbleTrans();
			cMap.SetMagnetTrans();
			cMan.SetNowY(0);
			bonusTimeState = 0;
		}
	}
	void CGameStateRun_2::OnShow()
	{
		cBackGround.OnShow();
		hits_left.ShowBitmap();
		cMap.OnShow();
		cMan.OnShow();					// 貼上人物
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////
	CGameStateBonus::CGameStateBonus(CGame *g) :CGameState(g)
	{
	}
	CGameStateBonus::~CGameStateBonus()
	{
	}
	void CGameStateBonus::OnBeginState()
	{
	}
	void CGameStateBonus::OnInit()
	{
	}
	void CGameStateBonus::OnKeyDown(UINT, UINT, UINT)
	{
	}
	void CGameStateBonus::OnKeyUp(UINT, UINT, UINT)
	{
	}
	void CGameStateBonus::OnShow()
	{
	}
}
