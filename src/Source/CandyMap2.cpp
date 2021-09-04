#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CookieMan.h"
#include "CandyMap2.h"
#define MAP_SIZE_X		1000
#define	MAP_TIMES		20	//�a��map�x�}�i��
#define PAN_Y			30	//��iMap�x�} y�b�V�U�����Z��->�]��
#define LIFE_X    100;
#define LIFE_Y     0;
#define CANDY_GOLD		1
#define CANDY_SMILE		2
#define COIN1			3
#define HEART			4
#define FIRE			5
#define WALL_1			6		//�}���U���a�O
#define TRAP_1			7		//���a�O�Z��100	�@�q���N�L��
#define	SKY_STOP_1		8		//�����ݶZ��300 �o�ۤU�~�L�o�F
#define GROUND_STOP_1	9		//���a�O�Z��150	�o�G�q���~�L���F
#define BONUSTIME       10
#define MAGNET				11
#define BIG							12
#define RUN_OUT  1
namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���}�G�a��:CandyMap2
	///////////////////////////////////////////////////////////////////////////////
	CandyMap2::CandyMap2() :MAX_LIFE(4000)
	{
		CandyMap2::Init();
	}
	void CandyMap2::Init()
	{
		LIFE = MAX_LIFE;			//�]�m�_�l�ͩR
		MOVE_WIDTH = 10;
		TURBO_MOVE_WIDTH = 20;
		life = LIFE;
		lifeX = LIFE_X;
		lifeY = LIFE_Y;
		MW = 50;
		MH = 50;
		x = y = 0;
		index = 0;
		allDistance = 0;
		turbleIndex = 0;
		bigIndex = 0;
		magnetTimes = 0;
		magnet = 0;
		manY1 = 250 + PAN_Y;
		turbleDistance = 0;
		is_alive = true;
		onFloor = false;
		magnetMode = false;
		bigState = false;
		turble = false;
		mapChoose = 0;	//�w�] : 0 -->  �Ψӿ�� MAP�x�}�q�ĴX�i�}�l�A�Q����ĴX��MAP�i�H�q�o�̳]�w 
						//0��groundstop/fire/trap
						//5��heart
						//8��BIG
						//17�� skystop
						//21��magnet
		mapNow = 0;
		mapXTimes = MAP_SIZE_X / MW;
		int map_init[MAP_TIMES][9][20] =
		{
			{
				//0
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 6, 6, 6 ,6, 6, 0, 0, 0, 6, 6 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0 },
			},
			{
				//1 has groundStop
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0 },
				{ 6, 6, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
				{ 0, 0, 0, 0, 7, 0, 0, 7, 0, 2, 2, 0, 0, 7, 0, 0, 9, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
				{ 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
				{ 6, 6, 6, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6 },
				{ 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6 },
			},
			{
				//4			//�����n�A��_�a��񧨤l
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 11, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//5
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 0, 0 },
				{ 9, 0, 0, 0, 0, 0, 0, 2, 3, 2, 0, 0, 2, 3, 2, 0, 0, 1, 0, 0 },
				{ 9, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 1, 0 },
				{ 9, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//6
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 3, 2, 3, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//7		//�n��groundstop
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 9, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//8
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 1, 1, 12, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6 },
			},
			{
				//9 has groundStop
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0, 0 },
				{ 9, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0 },
				{ 9, 0, 0, 0, 11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 , 0, 0, 0 },
				{ 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//10			
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 9, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 9, 0, 0, 5, 0, 12, 0, 9, 0, 4, 0, 9, 0, 0, 1, 1, 1, 4 },
				{ 0, 0, 9, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//11			
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0 },
				{ 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0 },
				{ 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6 },
				{ 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//12			
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//13		
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2 },
				{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2 },
				{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//14
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 2, 2, 2, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0 },
				{ 1, 1, 1, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0 },
				{ 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
			},
			{
				//15
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0 },
				{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0 },
			},
			{	//16	bonustime		
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
				{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
			},
			{   //17
				{ 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			},
			{   //18
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			},
			{   //19
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			}
		};
		for (int k = 0; k < MAP_TIMES; k++)
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					map[k][i][j] = map_init[k][i][j];//��l�Ʀa�Ϫ���}�C
					effectMap[k][i][j] = 0;				//��l�ƪ���S��\��}�C
					mapX[k][i][j] = 0;						//��l�ƪ���X�b��}
					mapY[k][i][j] = 0;						//��l�ƪ���Y�b��}
					magnetYDistance[k][i][j] = 0;//��l�ƺ��K�C�qY�l�ު��Z��
					magnetXDistance[k][i][j] = 0;//��l�ƺ��K�C�qX�l�ު��Z��
				}
			}
		}
	}
	void CandyMap2::LoadBitmap()
	{
		if (bmpCandyGold.isBitmapLoad() == false)  //load�L�N������load
		{
			bmpCandyGold.LoadBitmap(IDB_candyGold, RGB(255, 255, 255));
			bmpCoin1.LoadBitmap(IDB_coin1, RGB(255, 255, 255));
			bmpCandySmile.LoadBitmap(IDB_candySmile, RGB(255, 255, 255));
			bmpSkyStop1.LoadBitmap(IDB_skyStop1, RGB(255, 255, 255));
			bmpBarBorder.LoadBitmap(IDB_barBorder, RGB(255, 255, 255));
			bmpHeart.LoadBitmap(IDB_heart, RGB(255, 255, 255));
			bmpFire.LoadBitmap(IDB_fire, RGB(255, 255, 255));
			bmpWall1.LoadBitmap(IDB_wall_1, RGB(255, 255, 255));
			lifeScroll.LoadBitmap(IDB_life, RGB(255, 255, 255));
			bmpBonusTime.LoadBitmap(IDB_BONUSTIME, RGB(255, 255, 255));
			bmpMagnet.LoadBitmap(IDB_magnet, RGB(255, 255, 255));
			bmpBig.LoadBitmap(IDB_big, RGB(255, 255, 255));
			bmpTrap1.LoadBitmap(IDB_trap_1, RGB(255, 255, 255));//�R��Ҧ�
			animationTrap.AddBitmap(IDB_trap_1, RGB(255, 255, 255));//�ʵe�Ҧ�
			animationTrap.AddBitmap(IDB_trap_2, RGB(255, 255, 255));
			animationTrap.AddBitmap(IDB_trap_3, RGB(255, 255, 255));
			animationTrap.AddBitmap(IDB_trap_4, RGB(255, 255, 255));
			animationTrap.AddBitmap(IDB_trap_5, RGB(255, 255, 255));
			animationTrap.AddBitmap(IDB_trap_6, RGB(255, 255, 255));
			animationTrap.AddBitmap(IDB_trap_7, RGB(255, 255, 255));
			bmpGroundStop1.LoadBitmap(IDB_groundStop1, RGB(255, 255, 255));//�R��Ҧ�
			animationGroundStop.AddBitmap(IDB_groundStop1, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop2, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop3, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop4, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop5, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop6, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop7, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop8, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop9, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop10, RGB(255, 255, 255));
			animationGroundStop.AddBitmap(IDB_groundStop11, RGB(255, 255, 255));
			animationShine.AddBitmap(IDB_shine1);
			animationShine.AddBitmap(IDB_shine2);
			animationShine.AddBitmap(IDB_shine3);
			CAudio::Instance()->Load(COIN_2, "sounds\\coin01.mp3");
		}
	}
	void CandyMap2::OnShow()
	{
		if ((allDistance) >= MAP_SIZE_X)//�W�L����SIZE_X�� Map��index
		{
			index = 0;
			allDistance = allDistance % 1000;//�W0
			mapChoose += 1;
		}
		//�̫�@�i�a��
		//�C������
		if (this->IsEnd() == 1) {
			MOVE_WIDTH = 0;
		}
		if (turble == true && turbleIndex<110)//�i�JTurble�Ҧ��A�p��TURBLE�Ҧ��ɶ�
		{
			if ((MOVE_WIDTH < TURBO_MOVE_WIDTH))
			{
				MOVE_WIDTH += 1; //�]�wMOVE_WIDTH��TURBO_MOVE_WIDTH(�κ��[���覡
			}
			else
			{
				MOVE_WIDTH = 20;
			}
			turbleIndex++;//turble�ɶ��W�[
						  //Turble�ɶ���A�i�浲���ʵe
			if ((turbleIndex >= 105) && (turbleIndex <= 110)) {
				MOVE_WIDTH -= 1;				//�v���ܮz
				if (turbleIndex >= 100 || MOVE_WIDTH <= 10) {
					turble = false;
					turbleIndex = 0;
					MOVE_WIDTH = 10;
				}
			}
		}
		else MOVE_WIDTH = 10;
		//�p���ܤj���A�ɶ�
		if (bigState == true && bigIndex<200) {
			bigIndex++;
			if (bigIndex >= 200) {
				bigState = false;
				bigIndex = 0;
			}
		}
		//�p����K���A�ɶ�
		if (magnetMode == true) {
			magnetTimes++;
			if (magnetTimes >= 200) {
				magnetMode = false;
				magnetTimes = 0;
			}
		}
		int effect = 0;		//���󪺯S��ĪG
		int yHeight = 0;
		int yHeight1 = 0;
		int oldX = 0;	//����W�@�Ӫ��A��X
		int oldY = 0;//����W�@�Ӫ��A��Y 
		int magnetY = 0;//���K���A��Y�ݲ��ʪ��Z��
		int magnetX = 0;//���K���A��Y�ݲ��ʪ��Z��
		allDistance += MOVE_WIDTH;
		//�C�@�����h�]�@�i�w�ơA�~�|���s�e�o�ĪG
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				effect = effectMap[mapChoose % MAP_TIMES][i][j];
				oldX = mapX[mapChoose % MAP_TIMES][i][j];
				oldY = mapY[mapChoose % MAP_TIMES][i][j];
				magnetY = magnetYDistance[mapChoose % MAP_TIMES][i][j];
				magnetX = magnetXDistance[mapChoose % MAP_TIMES][i][j];
				yHeight = y + MH * i + PAN_Y;
				yHeight1 = y + MH * i + PAN_Y;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//�e���W����
				//��ܨC�Ӧa�ϸ귽�A�o����Candy�A�U�ػ�ê��
				switch (map[mapChoose % MAP_TIMES][i][j])
				{
				case 0:
					break;
				case CANDY_GOLD:
					//�]�w���K���A�Q�l�ު��ʵe
					if ((magnetMode == true) && (GetOnShowX(j) >= (manX1)) && (GetOnShowX(j) <= (manX1 + 150)))//���K�Ҧ�
					{
						int newX = oldX - (oldX - manX1) / (MOVE_WIDTH / 2);//����Move_width���ʶZ���� �a�񪺶Z��
						int newY = oldY - (oldY - (manY2 + manY1) / 2) / (MOVE_WIDTH / 2);
						mapX[mapChoose % MAP_TIMES][i][j] = newX;
						mapY[mapChoose % MAP_TIMES][i][j] = newY;
						bmpCandyGold.SetTopLeft(newX, newY);
						bmpCandyGold.ShowBitmap();
					}
					//���`�Ҧ�������ʵe
					else {
						bmpCandyGold.SetTopLeft(((MW * j) - allDistance), yHeight);//�]�wcandy1���۹��m(j��X�b,i��Y�b)
						bmpCandyGold.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = ((MW * j) - allDistance);
						mapY[mapChoose % MAP_TIMES][i][j] = yHeight;
					}
					break;
				case COIN1:
					//�]�w���K���A�Q�l�ު��ʵe
					if ((magnetMode == true) && (GetOnShowX(j) > (manX1)) && (GetOnShowX(j) <= (manX1 + 150)))
					{
						int newX = oldX - (oldX - manX1) / (MOVE_WIDTH / 2);
						int newY = oldY - (oldY - (manY2 + manY1) / 2) / (MOVE_WIDTH / 2);
						mapX[mapChoose % MAP_TIMES][i][j] = newX;
						mapY[mapChoose % MAP_TIMES][i][j] = newY;
						bmpCoin1.SetTopLeft(newX, newY);
						bmpCoin1.ShowBitmap();
					}
					//���`�Ҧ�������ʵe
					else
					{
						bmpCoin1.SetTopLeft(((MW * j) - allDistance), yHeight);
						bmpCoin1.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = ((MW * j) - allDistance);
						mapY[mapChoose % MAP_TIMES][i][j] = yHeight;
					}
					break;
				case CANDY_SMILE:
					//�]�w���K���A�Q�l�ު��ʵe
					if ((magnetMode == true) && (GetOnShowX(j) > (manX1)) && (GetOnShowX(j) <= (manX1 + 150)))
					{
						if (MOVE_WIDTH == 1)MOVE_WIDTH = 10;
						int newX = oldX - (oldX - manX1) / (MOVE_WIDTH / 2);
						int newY = oldY - (oldY - (manY2 + manY1) / 2) / (MOVE_WIDTH / 2);
						mapX[mapChoose % MAP_TIMES][i][j] = newX;
						mapY[mapChoose % MAP_TIMES][i][j] = newY;
						bmpCandySmile.SetTopLeft(newX, newY);
						bmpCandySmile.ShowBitmap();
					}
					//���`�Ҧ�������ʵe
					else
					{
						bmpCandySmile.SetTopLeft(((MW * j) - allDistance), yHeight);
						bmpCandySmile.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = ((MW * j) - allDistance);
						mapY[mapChoose % MAP_TIMES][i][j] = yHeight;
					}
					break;
				case SKY_STOP_1:
					//�]�w�Q�������ʵe
					if (effect == RUN_OUT)
					{
						bmpSkyStop1.SetTopLeft(oldX, oldY);
						bmpSkyStop1.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = oldX - 19;
						mapY[mapChoose % MAP_TIMES][i][j] = oldY - 10;
					}
					//���`�Ҧ�������ʵe
					else
					{
						bmpSkyStop1.SetTopLeft(((MW * j) - allDistance), 0);
						bmpSkyStop1.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = ((MW * j) - allDistance);
						mapY[mapChoose % MAP_TIMES][i][j] = 0;
					}
					break;
				case GROUND_STOP_1:
					//�]�w�Q�������ʵe
					if (effect == RUN_OUT)
					{
						animationGroundStop.SetTopLeft(oldX, oldY);
						animationGroundStop.OnShow();
						mapX[mapChoose % MAP_TIMES][i][j] = oldX - 19;
						mapY[mapChoose % MAP_TIMES][i][j] = oldY - 15;
					}
					//���`�Ҧ�������ʵe
					else
					{
						bmpGroundStop1.SetTopLeft(((MW * j) - allDistance), SIZE_Y - bmpGroundStop1.Height() - 2 * MH);
						bmpGroundStop1.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = ((MW * j) - allDistance);
						mapY[mapChoose % MAP_TIMES][i][j] = SIZE_Y - bmpGroundStop1.Height() - 2 * MH;
					}
					break;
				case HEART:
					bmpHeart.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpHeart.ShowBitmap();
					break;
				case FIRE:
					bmpFire.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpFire.ShowBitmap();
					break;
				case TRAP_1:
					//�]�w�Q�������ʵe
					if (effect == RUN_OUT)
					{
						animationTrap.SetTopLeft(oldX, oldY);
						animationTrap.OnShow();
						mapX[mapChoose % MAP_TIMES][i][j] = oldX - 19;
						mapY[mapChoose % MAP_TIMES][i][j] = oldY - 15;
					}
					//���`�Ҧ�������ʵe
					else
					{
						bmpTrap1.SetTopLeft(((MW * j) - allDistance), (SIZE_Y - bmpTrap1.Height()) - 2 * MH);
						bmpTrap1.ShowBitmap();
						mapX[mapChoose % MAP_TIMES][i][j] = (MW * j) - allDistance;
						mapY[mapChoose % MAP_TIMES][i][j] = (SIZE_Y - bmpTrap1.Height()) - 2 * MH;
					}
					break;
				case WALL_1:
					bmpWall1.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpWall1.ShowBitmap();
					break;
				case BONUSTIME:
					bmpBonusTime.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpBonusTime.ShowBitmap();
					break;
				case MAGNET:
					bmpMagnet.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpMagnet.ShowBitmap();
					break;
				case BIG:
					bmpBig.SetTopLeft(((MW * j) - allDistance), yHeight);
					bmpBig.ShowBitmap();
					break;
				default:
					ASSERT(0);
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//�ĤG����(��O�w��)
				//
				switch (map[(mapChoose + 1) % MAP_TIMES][i][j])
				{
				case 0:
					break;
				case CANDY_GOLD:
					bmpCandyGold.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpCandyGold.ShowBitmap();
					break;
				case COIN1:
					bmpCoin1.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpCoin1.ShowBitmap();
					break;
				case CANDY_SMILE:
					bmpCandySmile.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpCandySmile.ShowBitmap();
					break;
				case SKY_STOP_1:
					bmpSkyStop1.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), 0);
					bmpSkyStop1.ShowBitmap();
					break;
				case GROUND_STOP_1:
					bmpGroundStop1.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), (SIZE_Y - bmpGroundStop1.Height()) - 2 * MH);
					bmpGroundStop1.ShowBitmap();
					break;
				case HEART:
					bmpHeart.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpHeart.ShowBitmap();
					break;
				case FIRE:
					bmpFire.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpFire.ShowBitmap();
					break;
				case TRAP_1:
					bmpTrap1.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), (SIZE_Y - bmpTrap1.Height()) - 2 * MH);
					bmpTrap1.ShowBitmap();
					break;
				case WALL_1:
					bmpWall1.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpWall1.ShowBitmap();
					break;
				case BONUSTIME:
					bmpBonusTime.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpBonusTime.ShowBitmap();
					break;
				case MAGNET:
					bmpMagnet.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpMagnet.ShowBitmap();
					break;
				case BIG:
					bmpBig.SetTopLeft(MAP_SIZE_X + ((MW * j) - allDistance), yHeight);
					bmpBig.ShowBitmap();
					break;
				default:
					ASSERT(0);
				}
			}
		}
		//lifeScroll�ͩR�����A�ʵe
		for (int i = 0; i <life / 20; i++) {
			lifeScroll.SetTopLeft(lifeX + i * 2, y);
			lifeScroll.ShowBitmap();
		}
		animationShine.SetTopLeft(lifeX + (((life / 20) - 1) * 2), y);
		animationShine.OnShow();
		bmpBarBorder.SetTopLeft(lifeX, y);
		bmpBarBorder.ShowBitmap();
	}
	bool CandyMap2::GetIsTurbleMode() {
		return turble;
	}
	bool CandyMap2::GetIsBigMode() {
		return bigState;
	}
	int CandyMap2::GetNowY() {
		return manY1;
	}
	int CandyMap2::GetOnShowX(int realX) {
		return MW * realX - allDistance;
	}
	int CandyMap2::GetOnShowY(int realY) {
		return MW * realY - allDistance;
	}
	bool CandyMap2::GetIsOnFloor() {
		return onFloor;
	}
	bool CandyMap2::IsAlive()
	{
		return is_alive;
	}
	int CandyMap2::IsEnd()
	{
		//�p���U��}�O�_�b���I
		int map = mapChoose + ((allDistance + 640) / 1000);
		if (map == (MAP_TIMES - 5) || mapChoose == (MAP_TIMES - 5)) {
			return 1;//�̫�T�ӬObonustime
		}
		else return 0;
	}
	void CandyMap2::OnMove()
	{
		animationTrap.SetDelayCount(1);
		animationTrap.OnMove();
		animationGroundStop.SetDelayCount(1);
		animationGroundStop.OnMove();
		animationShine.SetDelayCount(2);
		animationShine.OnMove();
		index += 1;
		life--;
	}
	int CandyMap2::GetMH()
	{
		return MH;
	}
	int CandyMap2::GetMW()
	{
		return MW;
	}
	int CandyMap2::HitCookieMan(CookieMan* cookie)
	{
		manX1 = cookie->GetX1();
		manY1 = cookie->GetY1();
		manX2 = cookie->GetX2();
		manY2 = cookie->GetY2();
		return HitRectangleBox(manX1, manY1, manX2, manY2);
	}
	int CandyMap2::HitRectangleBox(int tx1, int ty1, int tx2, int ty2)//�H��X1,Y1,X2,Y2
	{
		int judgeMap = 0;
		if (((allDistance + tx1) / MW) + 1 >= mapXTimes)	//�P�_CookieMap��U����m�O�_�i�J�U�@��Map�x�}�A�O���ܯx�}index++
			judgeMap = (mapChoose + 1) % MAP_TIMES;
		else judgeMap = mapChoose % MAP_TIMES;
		int countX = (((allDistance + tx1) / MW) + 1) % mapXTimes;	//�d�򤺪��Omap���ĴX�Ӥ�� //�[1��� �U�@�����L���
		int x1 = countX * MW;									// �P�_�I����������W��x�y��
		int y1 = SIZE_Y - MH;								// �P�_�I����������W��y�y��
		int x2 = x1 + MW;//bmp.Width();						// �P�_�I��������k�U��x�y��
		int y2 = y1 + MH;//bmp.Height();					// �P�_�I��������k�U��y�y��
						 // �˴��y���x�λP�ѼƯx�άO�_���涰
		int i = 0;
		int hitTimes = 0;
		////////////////////////////////////////////////////////////////////////
		//�����H���}��a�O�����A
		//
		if (map[judgeMap][ty2 / MH][countX] == WALL_1)//�P�_���L���a�O
		{
			if (ty2 >= ((ty2 / MH)*MH + PAN_Y)) {
				onFloor = true;
			}
			else {
				onFloor = false;
			}
		}
		else {
			onFloor = false;
		}
		//
		//MagnetMode���}�G����
		//�]�w�d�򤺤~��Q�l��
		if (magnetMode == true)
		{
			for (int i = 0; i < 9; i++) {
				if (map[judgeMap][i][countX] < 6 && map[judgeMap][i][countX] > 0)
				{
					if (map[judgeMap][i][countX] == FIRE)
					{
						turble = true;
					}
					else  if (map[judgeMap][i][countX] == HEART)
					{
						life += 300;
						if (life >= MAX_LIFE)life = MAX_LIFE;
						//bigState = true;
					}
					else
					{
						hitTimes += 1;
					}
					map[judgeMap][i][countX] = 0;
				}
			}
		}
		//
		//�����H���d��̸I���쪺���~
		int indexY;//�n���w��Y�b
		for (i = 0; i < (ty2 - ty1) / MH; i++)
		{
			//�P�_CookieMan�d�򤺪��F�� 0<X<6���Y�F�i�H�[���ƪ�
			//�����A�Y�쪺�F��O�\��/�[��
			//magnetMode���}�G�����g�b�e���A�o�̤�����C
			indexY = ty1 / MH + i;
			if (indexY > 8)indexY = 8;
			if (map[judgeMap][indexY][countX] < 6 && map[judgeMap][indexY][countX] > 0 && magnetMode == false)
			{
				CAudio::Instance()->Play(COIN_2, false);
				hitTimes += 1;
				if (map[judgeMap][indexY][countX] == FIRE)
				{
					turble = true;
				}
				if (map[judgeMap][indexY][countX] == HEART)
				{
					life += 300;
					if (life > MAX_LIFE)life = MAX_LIFE;
				}
				map[judgeMap][indexY][countX] = 0;
			}
			//�P�_CookieMan�d�򤺪��F�� 6<X<10���Y�F�i�H�[���ƪ�
			//�����A�����I���쪺�O"��ê��"
			//
			else if ((map[judgeMap][indexY][countX] > 5) && (map[judgeMap][indexY][countX]< 10))  	//>5�O��ê���B���Oturble�[�t�Ҧ�
			{
				if (turble == false && bigState == false) {
					if (map[judgeMap][indexY][countX]> 6) {
						life -= 150;
						return -1;
					}
					else if ((map[judgeMap][indexY][countX] == 6) && map[judgeMap][((ty1 - MH) / MH) + i][countX] == 6)//
					{
						//life -= 150;
						return -1;
					}
				}
				//�]�w��ê�����S��ʵe
				else {
					if (map[judgeMap][indexY][countX] == GROUND_STOP_1)
					{
						effectMap[judgeMap][4][countX] = RUN_OUT;
						effectMap[judgeMap][5][countX] = RUN_OUT;
						effectMap[judgeMap][6][countX] = RUN_OUT;
					}
					if (map[judgeMap][indexY][countX] == TRAP_1)
					{
						effectMap[judgeMap][6][countX] = RUN_OUT;
					}
					if (map[judgeMap][indexY][countX] == SKY_STOP_1)
					{
						effectMap[judgeMap][0][countX] = RUN_OUT;
						effectMap[judgeMap][1][countX] = RUN_OUT;
						effectMap[judgeMap][2][countX] = RUN_OUT;
						effectMap[judgeMap][3][countX] = RUN_OUT;
						effectMap[judgeMap][4][countX] = RUN_OUT;
						effectMap[judgeMap][5][countX] = RUN_OUT;
					}
					//effectMap[judgeMap][ty1 / MH + i][countX] = RUN_OUT;
				}
			}
			//�P�_CookieMan�d�򤺪��F�� X=10���Y�F�i�Jbonustime
			//�����A����bonusTime
			//
			else if ((map[judgeMap][indexY][countX] == BONUSTIME))
			{
				map[judgeMap][indexY][countX] = 0;
				return -2;
			}
			else if (map[judgeMap][indexY][countX] == MAGNET)//���նi�J���K�Ҧ�
			{
				magnetMode = 1;
				map[judgeMap][indexY][countX] = 0;
			}
			else if (map[judgeMap][indexY][countX] == BIG)//���նi�JBIG�Ҧ�
			{
				bigState = 1;
				map[judgeMap][indexY][countX] = 0;
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		totalScore = hitTimes;
		return hitTimes;
	}
	bool CandyMap2::HitRectangle(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2)
	{
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}
	int CandyMap2::GetLifeNow() {
		return life;
	}
	void CandyMap2::SetBigTrans() {
		if (bigState == true)bigState = false;
		else bigState = true;
	}
	void CandyMap2::SetTurbleTrans() {
		if (turble == true)turble = false;
		else turble = true;
	}
	void CandyMap2::SetMagnetTrans() {
		if (magnetMode == true)magnetMode = false;
		else magnetMode = true;
	}
	void CandyMap2::SetQucikMove() {
		allDistance += 100;
	}
	void CandyMap2::SetAddLife() {
		life += 200;
		if (life >= MAX_LIFE)life = MAX_LIFE;
	}
}
