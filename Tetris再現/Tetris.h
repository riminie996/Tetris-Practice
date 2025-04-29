#pragma once
//���̃Q�[������Ŏg���萔�Ƃ�
#include "GameL\DrawTexture.h"
#include "Structure.h"


enum MINO_TYPE
{
	Mino_Empty = -1,
	Mino_S,
	Mino_Z,
	Mino_T,
	Mino_J,
	Mino_L,
	Mino_O,
	Mino_I,
};
enum DIRECTION
{
	Left,
	Right,
	Up,
	Down,
};

enum ROTATE_ANGLE
{
	RotateAngle0,
	RotateAngle90,
	RotateAngle180,
	RotateAngle270,
	RotateAngle_Count,
};




enum class E_TSPIN_PATTERN
{
	NoTSpin,
	TSpinMini,
	TSpin,
};


namespace Tetris
{
	const int MINO_MAX_TYPE = 7;		//�~�m�̍ő�̎��
	const int MINO_BLOCK_AMOUNT = 4;

	const int FIELD_WIDTH = 10;		//�t�B�[���h�̕�(�}�X��)
	const int FIELD_HEIGHT = 40;		//�t�B�[���h�̍���(�}�X��)
	const int MINO_INIT_POS_X = 3;
	const int MINO_INIT_POS_Y = 20;

	const int NEXT_AMOUNT = 5;		//�l�N�X�g��\�����鐔

	const float NEXT_HEIGHT = 128.0f;		//�l�N�X�g����l�N�l�N�܂ł̍���
	const float NEXT_OFFSET_X = 448.0f;	//�l�N�X�g��X���W�����p
	const float NEXT_OFFSET_Y = 64.0f;		//�l�N�X�g��Y���W�����p

	const float FIELD_OFFSET_X = 32.0f;		//�t�B�[���h��X���W�����p
	const float FIELD_OFFSET_Y = -632.0f;	//�t�B�[���h��Y���W�����p

	const float HOLD_OFFSET_X = -64.0f;	//�z�[���h��X���W�����p
	const float HOLD_OFFSET_Y = 64.0f;		//�z�[���h��Y���W�����p

	const float NEXT_MOJI_OFFSET_Y = -48.0f;

	const float FIELD_1P_POS_X = 192.0f;	//1P��X���W
	const float FIELD_1P_POS_Y = 0.0f;		//1P��Y���W

	const float BLOCK_PIXELS = 32.0f;		//�u���b�N�̃s�N�Z����

	const int MINO_MOVE_DISTANCE = 1;

	const int FRAME_MINO_FALL = 60;
	const int FRAME_MINO_LANDING = 30;
	const int FRAME_MINO_INPUT_MOVE = 11;
	const float FRAME_MINO_SOFTDROP = 1.5f;
	const int FRAME_MINO_MOVE_SPEED = 2;
	const int LOCKDOWN_MAX_MOVECOUNT = 15;
	const float ROTATION_ANGLE = 90.0f;

	const float MINO_ROTATION_AXIS = 1.0f;
	const float MINO_ROTATION_AXIS_I = 1.5f;

	const int MINO_DATABASE_COL = 4;
	const int MINO_DATABASE_LOW = 4;

	const int BLOCK_GAMEOVER_LINE_BOTTOM = 20;

	const GameL::RECT_F BLOCK_LIGHT_SRC_POS = { 0,256.0f,BLOCK_PIXELS,BLOCK_PIXELS };

	const float SCORE_TEXT_OFFSET_X = 0.0f;
	const float SCORE_TEXT_OFFSET_Y = 128.0f;
	const float SCORE_NUMBER_OFFSET_X = 0.0f;
	const float SCORE_NUMBER_OFFSET_Y = 160.0f;
	const float SCORE_FONT_SIZE = 32.0f;
	const float GAMEOVER_TEXT_X = WINDOW_SIZE_W / 2.0f;
	const float GAMEOVER_TEXT_Y = WINDOW_SIZE_H / 2.0f;
	const float GAMEOVER_TEXT_SIZE = 64.0f;
	const float OPTION_TEXT_OFFSET_X = 0.0f;
	const float OPTION_TEXT_OFFSET_Y = 256.0f;
	const float OPTION_TEXT_SIZE = 32.0f;
	const float COUNT_TEXT_X = 0.0f;
	const float COUNT_TEXT_Y = 224.0f;
	const float COUNT_TEXT_SIZE = 32.0f;
	const Point GAMEMODE_POS = {1280,720};


	const int ULTRA_MODE_SEC = 180;

	const int OVERLAP_MAX_UP_COUNT = 2;
	
	namespace Score
	{


		//���������C�����ɂ���ĉ��Z����X�R�A���ς��
		const int ADD_SCORE_LINES[] =
		{
			0,//0���C��
			100,//1���C��
			300,//2���C��
			500,//3
			800, //4
		};
		const int ADD_SCORE_TSPIN[] =
		{
			400,//0
			800,//1
			1200,//2
			1600,//3
		};
		const int ADD_SCORE_PERFECT[] =
		{
			0,
			800,//1
			1000,//2
			1800,//3
			2000,//4
		};
		const int ADD_REN_SCORE = 50;//50*REN��
		const int ADD_TSPINMINI_SCORE = 100;//50*REN��
		const float BTB_SCORE_SCALE = 1.5f;//btb�X�R�A�{��
	}

	//�J�^�`
	const int MINO_SHAPE[MINO_MAX_TYPE][MINO_DATABASE_COL][MINO_DATABASE_LOW] =
	{
		{//S�~�m
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		{//Z
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		{//T 2 = T-Spin����p
			{2,1,2,0},
			{1,1,1,0},
			{2,0,2,0},
			{0,0,0,0},
		},
		{//J
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		{//L
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		{//O
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0},
		},
		{//I
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
		},


	};
	//T-Spin����p�`�F�b�J�[
	const int TSPIN_CHECKER[RotateAngle_Count][MINO_DATABASE_COL][MINO_DATABASE_LOW] =
	{
		//1 = T-Spin Single�ȏ�̏����𖞂����̂ɕK�{, 2 = 
		{//RotateAngle0
			{1,0,1,0},
			{0,0,0,0},
			{2,0,2,0},
			{0,0,0,0},
		},
		{//RotateAngle90
			{2,0,1,0},
			{0,0,0,0},
			{2,0,1,0},
			{0,0,0,0},
		},
		{//RotateAngle180
			{2,0,2,0},
			{0,0,0,0},
			{1,0,1,0},
			{0,0,0,0},
		},
		{//RotateAngle270
			{1,0,2,0},
			{0,0,0,0},
			{1,0,2,0},
			{0,0,0,0},
		},


	};
	//�X�[�p�[���[�e�[�V�����V�X�e���֌W�̒萔---------------------
	//�ȉ�SRS�ƕ\������
	const int SRS_PATTERN = 5;

	//�ړ���
	const int SRS_LEFT_SHIFT_X[RotateAngle_Count][SRS_PATTERN] =
	{
		{0,1,1,0,1},	//0�x�̂Ƃ�
		{0, 1, 1,0, 1},	//90�x�̂Ƃ�
		{0,-1,-1,0,-1},	//180
		{0,-1,-1,0,-1},	//270
	};
	const int SRS_LEFT_SHIFT_Y[RotateAngle_Count][SRS_PATTERN] =
	{
		{0, 0,-1, 2, 2},//0
		{0, 0, 1,-2,-2},//90
		{0, 0,-1, 2, 2},//180
		{0, 0, 1,-2,-2},//270
	};
	const int SRS_RIGHT_SHIFT_X[RotateAngle_Count][SRS_PATTERN] =
	{
		{0,-1,-1,0,-1},	//0�x�̂Ƃ�
		{0, 1, 1,0, 1},	//90�x�̂Ƃ�
		{0, 1, 1,0, 1},	//180
		{0,-1,-1,0,-1},	//270
	};
	const int SRS_RIGHT_SHIFT_Y[RotateAngle_Count][SRS_PATTERN] =
	{
		{0, 0,-1, 2, 2},
		{0, 0, 1,-2,-2},
		{0, 0,-1, 2, 2},
		{0, 0, 1,-2,-2},
	};
	const int SRS_LEFT_I_SHIFT_X[RotateAngle_Count][SRS_PATTERN] =
	{
		{0,-1, 2,-1, 2},	//0�x�̂Ƃ�
		{0, 2,-1, 2,-1},	//90�x�̂Ƃ�
		{0, 1,-2, 1,-2},	//180
		{0, 1,-2,-2, 1},	//270
	};
	const int SRS_LEFT_I_SHIFT_Y[RotateAngle_Count][SRS_PATTERN] =
	{
		{0, 0, 0, -2, 1},//0
		{0, 0, 0, -1, 2},//90
		{0, 0, 0, -1, 2},//180
		{0, 0, 0,  1,-2},//270
	};
	const int SRS_RIGHT_I_SHIFT_X[RotateAngle_Count][SRS_PATTERN] =
	{
		{0,-2, 1,-2, 1},	//0�x�̂Ƃ�
		{0,-1, 2,-1, 2},	//90�x�̂Ƃ�
		{0, 2,-1,2, -1},	//180
		{0,-2, 1, 1,-2},	//270
	};
	const int SRS_RIGHT_I_SHIFT_Y[RotateAngle_Count][SRS_PATTERN] =
	{
		{0, 0, 0, 1,-2},
		{0, 0, 0,-2, 1},
		{0, 0, 0,-1, 2},
		{0, 0, 0,-2,-1},
	};

	const int FALL_ADD_SCORE = 2;

	const int RISING_MAX_LINES = 15;
	const int RISING_INTERVAL_SEC = 10;
	const float RISING_TEXT_OFFSET_X = 768.0f;
	const float RISING_TEXT_OFFSET_Y = 128.0f;
	const float RISING_TIMER_OFFSET_X = 768.0f;
	const float RISING_TIMER_OFFSET_Y = 160.0f;
	const float RISING_FONT_SIZE = 32.0f;

	const float RISING_LINES_OFFSET_X = 176.0f;	//1P��X���W
	const float RISING_LINES_OFFSET_Y = 624.0f;		//1P��Y���W



	enum E_PRACTICE_OPTION
	{
		TetriminoOrderFixed_1,
		TetriminoOrderFixed_End = TetriminoOrderFixed_1 + 7,
		NoNaturalDrop,
		ShowController,
		InfiniteHold,
		RisingTimer,
		GameMode,
		User_AutoRepeatRate,
		User_DelayerAutoShift,
		User_SDF,
		User_ReverseRotate,
		User_AccidentalHardDrop,
		User_NextCreateDelayFrame,
		Option_Count,
	};
	enum class E_GAME_MODE
	{
		mode_Standard,
		mode_40Line,
		mode_TSD20,
		mode_ULTRA,
		mode_count,
	};
	E_GAME_MODE& operator++(E_GAME_MODE& mode);
	E_GAME_MODE& operator--(E_GAME_MODE& mode);
	struct ST_PRACTICE_OPTION
	{
		bool option_flag[Option_Count];

		int rising_timer_sec;
		int rising_min_line;
		int rising_max_line;
		MINO_TYPE fixed_mino_type[TetriminoOrderFixed_End + 1][MINO_MAX_TYPE];
		Tetris::E_GAME_MODE gamemode;

		ST_PRACTICE_OPTION()
		{
			for (int i = 0; i < Option_Count; i++)
				option_flag[i] = false;
			rising_timer_sec = 0;
			rising_min_line = 1;
			rising_max_line = 6;
			for (int i = 0; i < TetriminoOrderFixed_End + 1; i++)
			{
				for (int j = 0; j < MINO_MAX_TYPE; j++)
				{
					fixed_mino_type[i][j] = (MINO_TYPE)j;
				}
			}
			gamemode = E_GAME_MODE::mode_Standard;
		}
	};

	std::wstring GetOptionName(E_PRACTICE_OPTION option);

	namespace PracticeOption
	{
		std::wstring GetStrGameMode(E_GAME_MODE mode);
	}

	int CalcSendGarbageLines(int clear_line, int ren, bool btb, E_TSPIN_PATTERN tspin, bool perfect);
	void Mino_Shape_Draw(int x, int y, MINO_TYPE type);
};

