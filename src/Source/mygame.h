/*
* mygame.h: ���ɮ��x�C��������class��interface
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
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*   2006-02-08 V4.2
*      1. Rename OnInitialUpdate() -> OnInit().
*      2. Replace AUDIO_CANYON as AUDIO_NTUT.
*      3. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to empha
that they are
*         event driven.
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "CookieMan.h"
#include "CandyMap.h"
#include "CandyMap2.h"
#include "BackGround.h"
#include "Sound.h"
extern bool stage[3];//���d�}�����p
extern int stageScore;
namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// �o��class�����C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateInit : public CGameState
	{
	public:
		CGameStateInit(CGame* g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap logo, play, pointer, about, explain, aboutBackground, back;								// csie��logo
		CMovingBitmap explainBackground1, explainBackground2, explainBackground3, explainBackground4;
		CMovingBitmap page1, page2, page3, page4;
		bool playHover;
		int pointerPosition, backGround, page;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C������ܵe��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateStage : public CGameState
	{
	public:
		CGameStateStage(CGame* g);
		~CGameStateStage();
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnKeyDown(UINT, UINT, UINT);
	protected:
		void OnShow();
	private:
		CMovingBitmap land1, land2, back, back_Hover, stageGround, lock;
		bool Land1Hover, Land2Hover, backHover;
		CMovingBitmap land1_Hover, land2_Hover;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateRun : public CGameState
	{
	public:
		CGameStateRun(CGame* g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		int success;
	protected:
		void OnMove();
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap	help;		// ������
		CMovingBitmap	corner;		// ������
		CInteger		hits_left;	// �ѤU������
		CInteger		life;	// �ѤU������
		CookieMan 		cMan;
		CandyMap		cMap;
		int lifeIndex;
		int bonusTimeState;
		BackGround      cBackGround;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateRun_2 : public CGameState
	{
	public:
		CGameStateRun_2(CGame* g);
		~CGameStateRun_2();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnMove();
		void OnShow();
	private:
		CookieMan 	cMan;
		CandyMap2 cMap;
		CInteger		hits_left;	// �ѤU������
		CInteger		life;	// �ѤU������
		BackGround      cBackGround;
		int lifeIndex;
		int bonusTimeState;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateBonus : public CGameState
	{
	public:
		CGameStateBonus(CGame* g);
		~CGameStateBonus();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnShow();
	private:
		CookieMan 	cMan;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////
	class CGameStateOver : public CGameState
	{
	public:
		CGameStateOver(CGame* g);
		//CGameStateOver(int score);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap bmpEndGame1, bmpConfirm, gameOver, congratulation;
		CInteger score;
		int counter, endScore;	// �˼Ƥ��p�ƾ�
	};
}
