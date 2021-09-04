#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CookieMan.h"
#include "BackGround.h"
namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���}�G�a��:BackGround
	///////////////////////////////////////////////////////////////////////////////
	BackGround::BackGround() : MH(1200), MW(1920)
	{
		index = 0;
		choose = 0;
		int status = 1;
		moveDistance = 5;
		moveSignal = true;
	}
	void BackGround::LoadBitmap()
	{
		if (sky_1.isBitmapLoad() == false)
		{
			sky_1.LoadBitmap(IDB_sky_3);
			sky_2.LoadBitmap(IDB_sky_3);
			bonus_1.LoadBitmap(IDB_BONUSMAP1);
			bonus_2.LoadBitmap(IDB_BONUSMAP1);
			sky_shine.AddBitmap(IDB_skyShine_1);
			sky_shine.AddBitmap(IDB_skyShine_2);
			sky_shine.AddBitmap(IDB_skyShine_3);
			sky_shine.AddBitmap(IDB_skyShine_1);
		}
	}
	void BackGround::OnMove()
	{
		//time += 5;
		//�����C���H���O���b����
		if (moveSignal == true)
		{
			index++;
		}
		else
		{
			index = index;
		}
		//���ʪ��Z���W�L�Ϥ����j�p
		if (moveDistance * index >= MW)
		{
			index = 0;
		}
		sky_1.SetTopLeft(0 - moveDistance * index, SIZE_Y - MH);
		sky_2.SetTopLeft(MW - moveDistance * index, SIZE_Y - MH);
		bonus_1.SetTopLeft(0 - moveDistance * index, SIZE_Y - MH);
		bonus_2.SetTopLeft(bonus_1.Width() - moveDistance * index, SIZE_Y - MH);
		//���`�Ҧ����Ϥ��]�w
		if (status == 1)
		{
			sky_shine.SetDelayCount(10);
			sky_shine.OnMove();
			if (sky_shine.GetCurrentBitmapNumber() == 3)
			{
				status = 0;
			}
		}
	}
	void BackGround::OnShow()
	{
		//���`�Ҧ���show
		if (choose == 0)
		{
			sky_1.ShowBitmap();
			sky_2.ShowBitmap();
			if (status == 1)
			{
				sky_shine.OnShow();
			}
		}
		//bonustime��show
		if (choose == 1)
		{
			bonus_1.ShowBitmap();
			bonus_2.ShowBitmap();
		}
	}
	void BackGround::SetMoveSpeed(int speed)
	{
		moveDistance = speed;
	}
	void BackGround::MoveState(bool state)
	{
		moveSignal = state;
	}
}
