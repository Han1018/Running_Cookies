namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C���I��:background
	/////////////////////////////////////////////////////////////////////////////
	class BackGround {
	public:
		BackGround();
		void OnShow();
		void LoadBitmap();
		void OnMove();
		void SetMoveSpeed(int speed);
		void MoveState(bool state);
		int choose;
		int status;
	private:
		CMovingBitmap sky_1, sky_2, bonus_1, bonus_2;
		CAnimation sky_shine;
		int index;
		bool moveSignal;
		const int MH, MW;//�a�ϰ��e
		int moveDistance;
	};
}