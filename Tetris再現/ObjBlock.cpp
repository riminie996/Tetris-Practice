#include "ObjBlock.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameL\WinInputs.h"
#include "GameL\Audio.h"
#include "CControllerInput.h"
#include "Function.h"
#include <stdlib.h>
#include <time.h>
#include <random>


ObjBlock::ObjBlock()
{
	std::random_device rand;
	//���S�Ƀ����_���i�V�[�h
	m_random_random_seed = rand();

	m_common_key_flag = false;

	m_bag_round_count = 0;
	Audio::Start(bgm_1);

}

//������
void ObjBlock::Init()
{
	InitField();


	m_lines_complete = false;
	m_gameover = false;
	m_gameclear = false;


	m_hold_type = Mino_Empty;
	m_hold_flag = true;



	m_pause_flag = false;

	m_random_random_engine = std::mt19937(m_random_random_seed);


	ObjRisingTimer* oRisingTimer = (ObjRisingTimer*)Objs::GetObj(OBJ_RISING);
		
	if (oRisingTimer != nullptr && m_practice_options.rising_timer_sec <= 0)
	{
		oRisingTimer->SetStatus(false);
	}
	if (m_practice_options.rising_timer_sec > 0)
	{
		if (oRisingTimer == nullptr)
		{
			oRisingTimer = new ObjRisingTimer();
			Objs::InsertObj(oRisingTimer, OBJ_RISING, PRIO_RISING);
		}
		oRisingTimer->SetTimer(m_practice_options.rising_timer_sec);
	}
	m_list_garbage.clear();

	//100���C���`�[�Y���[�h��p
	Obj100LineCheez* o100line = (Obj100LineCheez*)Objs::GetObj(OBJ_100LINECHEEZ);

	if (o100line != nullptr && m_practice_options.gamemode != E_GAME_MODE::mode_100LineCheez)
	{
		o100line->SetStatus(false);
	}
	if (m_practice_options.gamemode == E_GAME_MODE::mode_100LineCheez)
	{
		if (o100line == nullptr)
		{
			o100line = new Obj100LineCheez();
			Objs::InsertObj(o100line, OBJ_100LINECHEEZ, PRIO_100LINECHEEZ);
		}

		o100line->Init();
		o100line->GiveCheezLine(Tetris::Cheez100::INIT_CHEEZ_LINE);
		GarbageRising(Tetris::Cheez100::INIT_CHEEZ_LINE);
	}
	//4��REN���[�h��p
	if (m_practice_options.gamemode == E_GAME_MODE::mode_4WRen)
	{
		//4��REN���[�h
		FieldMapImport("Field/4WRen/Field.txt", *m_field);
	}
	if (m_practice_options.gamemode == E_GAME_MODE::mode_Spin)
	{
		NextClear();
		m_practice_order.RandomFieldImport(*m_field);

		NextCreate(m_practice_order.GetMinoListFront());
		MinoCreateFromNext();
		for (int i = 0; i < NEXT_AMOUNT; i++)
		{
			MINO_TYPE mino = m_practice_order.GetMinoListFront();
			if (mino != Mino_Empty)
				NextCreate(mino);
			else 
				break;

		}
	}
	else
	{
		NextClear();
		NextCreate();
		MinoCreateFromNext();
		for (int i = 0; i < NEXT_AMOUNT; i++)
		{
			//NEXT�̐�+1�ŁA�ŏ��~�m��������
			NextCreate();
		}
	}
	//
	m_btb = false;
	m_ren = REN_NONE;
}

//�i�s
void ObjBlock::Action()
{
	ObjPracticeOption* oOpt = (ObjPracticeOption*)Objs::GetObj(OBJ_PRACTICEOPTION);

	ObjPlayerControll* ctrl = (ObjPlayerControll*)Objs::GetObj(OBJ_PLAYERCONTROLL);
	m_pause_flag = oOpt != nullptr;
	if (m_pause_flag)return;

	if (Input::GetVKey('R') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_OPTION))
	{
		if (m_common_key_flag == true)
			Restart();
		m_common_key_flag = false;
	}
	else if (Input::GetVKey('X') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_SHARE))
	{
		if (m_common_key_flag == true)
			FieldMapExport(m_field);
		m_common_key_flag = false;
	}
	else
	{
		m_common_key_flag = true;
	}
	if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_TOUCHPAD))
	{
		//if (m_common_key_flag == true)
		//{
			oOpt = new ObjPracticeOption();
			Objs::InsertObj(oOpt, OBJ_PRACTICEOPTION, PRIO_OPTION);
		//}
		//m_common_key_flag = false;
	}

	for (auto itr = m_list_garbage.begin(); itr != m_list_garbage.end(); itr++)
	{
		itr->rising_time_remain_sec.Sub(FRAME_TO_SEC);
		if (itr->rising_time_remain_sec.GetMinReached())
			itr->rising_reach = true;
	}
}

//�`��
void ObjBlock::Draw()
{
	//�����F

	FieldDraw(FIELD_1P_POS_X + FIELD_OFFSET_X, FIELD_1P_POS_Y + FIELD_OFFSET_Y, m_field);

	for (int i = 0; i < m_practice_options.next_displayed_count; i++)
	{
		float x = FIELD_1P_POS_X + NEXT_OFFSET_X;
		float y = i * NEXT_HEIGHT + FIELD_1P_POS_Y + NEXT_OFFSET_Y;
		Draw::SetColor(texNext, Color::White);
		Draw::CenterDraw(texNext, x, y);
		//0 - 6�̏ꍇ
		if (m_next[i] > Mino_Empty)
		{
			Mino_Shape_Draw(x, y, m_next[i]);
		}
	}

	float hx = FIELD_1P_POS_X + HOLD_OFFSET_X;
	float hy = FIELD_1P_POS_Y + HOLD_OFFSET_Y;
	Draw::CenterDraw(texNext, hx, hy);
	if (m_hold_type != Mino_Empty)
	{
		Mino_Shape_Draw(hx, hy, m_hold_type);
	}
	Font::StrCenterDraw(L"NEXT", FIELD_1P_POS_X + NEXT_OFFSET_X, FIELD_1P_POS_Y + NEXT_OFFSET_Y + NEXT_MOJI_OFFSET_Y, BLOCK_PIXELS, ColorA::White);
	Font::StrCenterDraw(L"HOLD", hx, hy + NEXT_MOJI_OFFSET_Y, BLOCK_PIXELS, ColorA::White);


	//�I�v�V�����̕`��
	DrawFontOption(OPTION_TEXT_OFFSET_X, OPTION_TEXT_OFFSET_Y, OPTION_TEXT_SIZE);
	if (m_gameover == true)
	{
		Font::StrCenterDraw(L"GAME OVER", GAMEOVER_TEXT_X, GAMEOVER_TEXT_Y, GAMEOVER_TEXT_SIZE, ColorA::White);
	}
	RisingLinesBlockDraw(/*m_rising_lines*/);
	std::wstring str_mode = Tetris::PracticeOption::GetStrGameMode(m_practice_options.gamemode);
	Font::StrRightDraw(str_mode.c_str(), GAMEMODE_POS.x, GAMEMODE_POS.y - GAMEOVER_TEXT_SIZE, GAMEOVER_TEXT_SIZE, ColorA::White);
	

}

void ObjBlock::SetBlock(int x, int y, MapObjects type)
{
	m_field[y][x] = type;

}


void ObjBlock::MinoCreate(MINO_TYPE type)
{
	ObjMino* oMino = new ObjMino(type);
	Objs::InsertObj(oMino, OBJ_MINO, 2);

}
void ObjBlock::NextCreate()
{
	//for (int i = 0; i < m_practice_option.next_displayed_count; i++)
	//{
	//	if (m_next[i] == Mino_Empty)
	//	{
	//		MINO_TYPE mino_type = BagToType();

	//		m_bag[mino_type] = false;


	//		m_next[i] = mino_type;
	//		return;
	//	}
	//}
	MINO_TYPE mino_type = BagToType();
	NextCreate(mino_type);
}
void ObjBlock::NextCreate(MINO_TYPE type)
{
	if (type == Mino_Empty)return;

	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		if (m_next[i] == Mino_Empty)
		{
			//if (m_bag[type])
			//{
				m_bag[type] = false;
				m_next[i] = type;
				return;
			//}
			//else
			//{
			//	throw std::exception("�o�b�O�V�X�e���̃~�m�^�C�v��false�ɂȂ��Ă���");
			//}
		}
	}
}
void ObjBlock::MinoCreateFromNext()
{
	//�~�m���쐬����i�K�ŃQ�[���I�[�o�[�̏ꍇ�A�쐬���Ȃ�
	if (GetGameOverFlag())
		return;

	if (m_next[0] != Mino_Empty)
	{
		MinoCreate(m_next[0]);
	}
	else
	{
		return;
	}

	//���ԓ���ւ�
	for (int i = 0; i < NEXT_AMOUNT - 1; i++)
	{
		m_next[i] = m_next[i + 1];
	}

	m_next[NEXT_AMOUNT - 1] = Mino_Empty;
}
void ObjBlock::NextClear()
{

	//�l�N�X�g�쐬����O�ɏ��ڃ��Z�b�g
	m_mino_count = 0;
	m_bag_round_count = 0;
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		m_bag[i] = true;
	}
	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		m_next[i] = Mino_Empty;
	}
}
MINO_TYPE ObjBlock::BagToType()
{

	Bag_All_Check();
	/// �����_��	

	//�o�b�O���Ɏc���Ă���~�m�̎�ނ̃J�E���g
	int bag_mino_remain = 0;
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		if (m_bag[i] == true)
			bag_mino_remain++;
	}

	int rnd = 0;
	std::random_device rand;

	if (m_bag_round_count <= Tetris::E_PRACTICE_OPTION::TetriminoOrderFixed_End && 
		m_practice_options.option_flag[m_bag_round_count] && 
		m_practice_options.fixed_mino_type[m_bag_round_count][MINO_MAX_TYPE - bag_mino_remain] != Mino_Empty)
	{
		return m_practice_options.fixed_mino_type[m_bag_round_count][MINO_MAX_TYPE - bag_mino_remain];
	}
	else
		rnd = rand() % bag_mino_remain;
	
	//�o�b�O���L���ȃ~�m��ʉ߂����Ƃ��ɉ��Z�����J�E���g
	int count = 0;
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{

		if (m_bag[i] == true) 
		{
			if (rnd == count)
			{
				return (MINO_TYPE)i;
			}
			count++;
		}
	}

	throw std::exception("�o�b�O�V�X�e���̃~�m�^�C�v�����ׂ�false�ɂȂ��Ă���");
}

//�~�m�����n���Ă��邩�ǂ����Ԃ�
bool ObjBlock::BlockPosCheck(int x,int y)
{
	//��O����
	if (x < 0 || x >= FIELD_WIDTH ||
		y < 0 || y >= FIELD_HEIGHT)
		return true;

	if (m_field[y][x] != BlockEmpty)
	{
		//������
		return true;
	}
	return false;
}

void ObjBlock::Bag_All_Check()
{
	bool all_check = false;

	//�S�Ă�false�̏ꍇ�A�l�N�X�g�쐬�����������[�v���N�����Ă��܂��\�������邽�߁A
	//���O�ɂ��ׂĂ�false�łȂ������ׂĂ���
	//���ׂĂ�false�̏ꍇ�́A���ׂ�true�ɕς��悤
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		if (m_bag[i] == true)
			break;
		if (i == MINO_MAX_TYPE - 1 && m_bag[i] == false)
		{
			//�S��false�ł���
			all_check = true;
		}
	}
	if (all_check == true)
	{
		for (int i = 0; i < MINO_MAX_TYPE; i++)
		{
			m_bag[i] = true;
		}
		m_bag_round_count++;
	}


}

void ObjBlock::LinesCompleteCheck()
{
	//�����ꂽ���C�����ɉ����ăX�R�A�����Z
	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);

	int lines_count = 0;
	bool perfect_clear = false;
	bool btb = m_btb;
	int clear_garbage_lines = 0;//������܏��������ǂ��� Cheez100���C�����[�h�p�ɒǉ�

	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
			bool is_garbage = false;
		for (int x = 0; x < FIELD_WIDTH;)
		{

			//X���W�̃��[�v�𔲂���
			if (m_field[y][x] == BlockEmpty)break;

			//Cheez100���C��
			if (is_garbage == false && m_field[y][x] == BlockWall)
			{
				is_garbage = true;
			}

			x++;
			//��ԉE�܂œ��B����
			if (x == FIELD_WIDTH)
			{
				lines_count++;
				LineClear(y);

				if (is_garbage)
					clear_garbage_lines++;
			}
		}
	}
	if (lines_count >= 1)
	{
		for (int x = 0; x < FIELD_WIDTH;)
		{
			//X���W�̃��[�v�𔲂���
			if (m_field[FIELD_HEIGHT - 1][x] != BlockEmpty)break;
			x++;
			//��ԉE�܂œ��B����
			if (x == FIELD_WIDTH)
			{
				perfect_clear = true;
			}
		}


		if (perfect_clear)
		{
			Audio::Start(se_Perfect);
		}

		ObjMino* mino = (ObjMino*)Objs::GetObj(OBJ_MINO);
		E_TSPIN_PATTERN tspin = mino->GetTSpinFlag();


		//BTB�̔���͍Ō�ɍs���B
		if (tspin != E_TSPIN_PATTERN::NoTSpin)
		{
			Audio::Start(AudioIds::se_Line_TSpin);
			m_btb = true;
		}
		else if (lines_count >= 4)
		{
			Audio::Start(AudioIds::se_Line_Tetris);
			m_btb = true;
		}
		else
		{
			Audio::Start(AudioIds::se_Line_Clear);
			m_btb = false;
			btb = false;
		}

		m_ren++;

		int attack_lines = CalcSendGarbageLines(lines_count, m_ren, btb, tspin, perfect_clear);
		AttackGarbage(attack_lines);

		ObjClearText* text = (ObjClearText*)Objs::GetObj(OBJ_CLEARTEXT);
		text->AnimeStart(m_ren, lines_count, btb, tspin, perfect_clear);

		int add_score = CalcScore(lines_count, m_ren, tspin, btb, perfect_clear);
		oScore->AddScore(add_score);

		oScore->AddAttackLines(attack_lines);

		//TSPIN�̈����́ATSD20���[�h�p
		oScore->AddClearLines(lines_count , tspin);

		Obj100LineCheez* oCheez = (Obj100LineCheez*)Objs::GetObj(OBJ_100LINECHEEZ);
		if (oCheez != nullptr)
		{
			oCheez->ReportClearCheezLine(clear_garbage_lines);
			Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage = oCheez->GiveCheezLine(clear_garbage_lines);
			AddGarbageLines(garbage);
		}


	}
	else//0���C��
	{
		m_ren = REN_NONE;
	}
	oScore->AddMinoCount();
	if (m_practice_options.gamemode == E_GAME_MODE::mode_Spin)
	{
		m_practice_order.NormaLinesSub(lines_count);
	}
}

void ObjBlock::InitField()
{
	// FIELD_HEIGHT = �u���b�N�u����͈�
	// FIELD_HEIGHT + 1 =�@��ԉ��̏����l�������͈�
	for (int i = 0; i < FIELD_HEIGHT; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{

			m_field[i][j] = BlockEmpty;

		}
	}

}

void ObjBlock::SetHoldType(MINO_TYPE type)
{
	//�O��̃z�[���h���L��
	MINO_TYPE old_hold = m_hold_type;
	m_hold_type = type;

	if (old_hold == Mino_Empty)
	{
		MinoCreateFromNext();
		//�z�[���h�ɉ����Ȃ������ꍇ
		NextCreate();
	 }
	else
	{
		//���łɉ������������ꍇ
		MinoCreate(old_hold);
	}
}

void ObjBlock::DrawFontOption(float x, float y, float font_size) 
{
	//����
	Font::StrDraw(L"OPTIONS", x, y, font_size, ColorA::White);

	wchar_t wcr[64];
	std::wstring str;
	int active_options_count = 1;
	for (int i = 0; i < E_PRACTICE_OPTION::Option_Count; i++)
	{
		float option_text_offset_y = font_size * active_options_count;

		str = Tetris::GetOptionName((E_PRACTICE_OPTION)i);
		switch (i)
		{

		case E_PRACTICE_OPTION::RisingTimer:
			str += std::to_wstring(m_practice_options.rising_timer_sec) + L"�b";
			break;
		case E_PRACTICE_OPTION::GameMode:
			str += Tetris::PracticeOption::GetStrGameMode(m_practice_options.gamemode);
			break;
		case E_PRACTICE_OPTION::NextDisplayedCount:
			break;
		case E_PRACTICE_OPTION::User_AutoRepeatRate:
			break;
		case E_PRACTICE_OPTION::User_DelayerAutoShift:
			break;
		case E_PRACTICE_OPTION::User_SDF:
			break;
		case E_PRACTICE_OPTION::User_ReverseRotate:
			break;
		case E_PRACTICE_OPTION::User_AccidentalHardDrop:
			break;
		default:
			break;
		}

		if (m_practice_options.option_flag[i] == true)
		{
			swprintf_s(wcr, L"%s", str.c_str());
			Font::StrDraw(wcr, x, y + option_text_offset_y, font_size, ColorA::White);
			active_options_count++;
		}

	}
}

void ObjBlock::Restart()
{
	ObjMino* mino = (ObjMino*)Objs::GetObj(OBJ_MINO);
	if (mino != nullptr)
	{
		mino->SetStatus(false);
	}
	Init();
	ObjScore* score = (ObjScore*)Objs::GetObj(OBJ_SCORE);
	score->Reset();
}


void ObjBlock::SetHoldFlag(bool status)
{
	if (m_practice_options.option_flag[E_PRACTICE_OPTION::InfiniteHold])
	{
		m_hold_flag = true;
		return;
	}
	m_hold_flag = status;

}

void ObjBlock::FieldDraw(int offset_x, int offset_y, MapObjects field[FIELD_HEIGHT][FIELD_WIDTH])
{
	//�@��ԉ��̏��̕��l��
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		//+2�͍��E�ǂ̕��l��
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			int bx = x * BLOCK_PIXELS + offset_x;
			int by = y * BLOCK_PIXELS + offset_y;
			//�y���`��B
			Draw::Draw(texBase, bx, by);


			
			//GameHead MapObjects���Q��
			if (field[y][x] != BlockEmpty)
			{
				BlockDraw(bx, by, field[y][x]);
			}
			
		}
	}
}

void ObjBlock::LineClear(int line_y)
{
	//���C���������
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		//�u���b�N������
		m_field[line_y][i] = BlockEmpty;
	}
	//���̃��C�������ɁA���炵�Ă���
	for (int i = line_y; i > 0; i--)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1�i������
			m_field[i][j] = m_field[i - 1][j];
		}
	}
}

//�Q�[���I�[�o�[����(40���C������Ƀu���b�N�����B)�������Ŕ��肷��B
void ObjBlock::GarbageRising(int height)
{
	if (height <= 0)return;

	//���C���������
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		//���Ȃ炸1���C����1�ȏ�u���b�N�����݂���͂��Ȃ̂ŁA
		//���肠���鍂�����̃��C���̂݃u���b�N�����邩���肷��B
		if (m_field[height - 1][i] != BlockEmpty)
		{
			SetGameOverFlag(true);
		}
	}

	//���̃��C�������ɁA���炵�Ă���
	for (int i = 0; i < FIELD_HEIGHT - height; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1�i������
			m_field[i][j] = m_field[i + height][j];
		}
	}

	//70%�̊m���œ�����
	std::uniform_int_distribution<int>dist(0,FIELD_WIDTH - 1);
	int space_x = dist(m_random_random_engine);//���X

	for (int i = FIELD_HEIGHT - height; i < FIELD_HEIGHT; i++)
	{
		//30%�̊m���ōĒ��I 100���C���`�[�Y�̏ꍇ100%�Ē��I
		if (dist(m_random_random_engine) < 3 || m_practice_options.gamemode == E_GAME_MODE::mode_100LineCheez)
		{
			space_x = dist(m_random_random_engine);
		}

		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1�i������
			if (space_x != j)
				m_field[i][j] = BlockWall;
			else
				m_field[i][j] = BlockEmpty;
		}
	}


	Audio::Start(AudioIds::se_Rising);
}


void ObjBlock::BlockDraw(int screen_pos_x, int screen_pos_y, int type)
{
	RECT_F src = { 0,BLOCK_PIXELS * (type - 1),BLOCK_PIXELS,BLOCK_PIXELS };
	Draw::Draw(texBlock, screen_pos_x, screen_pos_y, src);
	Draw::Draw(texBlock, screen_pos_x, screen_pos_y, BLOCK_LIGHT_SRC_POS);
}

void ObjBlock::AddGarbageLines(int height)
{
	//m_rising_lines += height;
	//m_rising_remain = 1;

	//Audio::Start(AudioIds::se_Garbage);
}
void ObjBlock::AddGarbageLines(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage)
{
	//m_rising_lines += height;
	//m_rising_remain = 1;
	m_list_garbage.push_back(garbage);

	if(garbage.lines > 0)
	Audio::Start(AudioIds::se_Garbage);
}

void ObjBlock::RisingLinesBlockDraw(/*int lines*/)
{
	int block_count = 0;
	for (auto itr = m_list_garbage.begin(); itr != m_list_garbage.end(); itr++)
	{
		for (int i = 0; i < itr->lines; i++)
		{
			MapObjects color = itr->rising_reach == true ? BlockRed : BlockWall;
			BlockDraw(Tetris::RISING_LINES_OFFSET_X, Tetris::RISING_LINES_OFFSET_Y + block_count * (-Tetris::BLOCK_PIXELS), color);
			block_count++;
		}
	}
}



void ObjBlock::FieldUpdate()
{
	LinesCompleteCheck();


	for (auto itr = m_list_garbage.begin(); itr != m_list_garbage.end();)
	{

		if (itr->rising_reach == true)
		{
			GarbageRising(itr->lines);
			itr = m_list_garbage.erase(itr);
			continue;
		}
		itr++;
	}

	MinoCreateFromNext();

	if (m_practice_options.gamemode == E_GAME_MODE::mode_Spin)
	{
		NextCreate(m_practice_order.GetMinoListFront());
	}
	else
	{
		NextCreate();

	}

	//4��REN���[�h��p
	if (m_practice_options.gamemode == E_GAME_MODE::mode_4WRen)
	{
		for (int i = 0; i < FIELD_WIDTH; i++)
		{
			if (i < 3 || i >= 7)
			{
				m_field[0][i] = BlockWall;
			}
		}
	
	}
}
void ObjBlock::AttackGarbage(int lines)
{
	//���E�Ȃ�
	if (m_practice_options.gamemode == E_GAME_MODE::mode_100LineCheez)
		return;

	//���E�̏���
	for (auto itr = m_list_garbage.begin(); itr != m_list_garbage.end(); itr++)
	{
		if (lines < itr->lines)
		{
			itr->lines -= lines;
			break;
		}
		else //if (lines >= itr->lines)
		{
			lines -= itr->lines;
			itr->lines = 0;
		}
	}

}

int ObjBlock::CalcScore(int lines, int ren, E_TSPIN_PATTERN tspin, bool btb, bool perfect)
{
	int add_score = 0;
	
	//���C�������X�R�A���Z
	if (tspin == E_TSPIN_PATTERN::TSpin)
		add_score += Score::ADD_SCORE_TSPIN[lines];
	else
		add_score += Score::ADD_SCORE_LINES[lines];

	//REN
	add_score += ren * Score::ADD_REN_SCORE;

	//BTB�̏ꍇ�A1.5�{
	if (btb)
		add_score *= Score::BTB_SCORE_SCALE;
	
	//TSpinMini�{�[�i�X
	if (tspin == E_TSPIN_PATTERN::TSpinMini)
		add_score += Score::ADD_TSPINMINI_SCORE;

	//�p�[�t�F�N�g�̏ꍇ�A�X�R�A�{�[�i�X
	if (perfect)
		add_score += Score::ADD_SCORE_PERFECT[lines];

	return add_score;
}