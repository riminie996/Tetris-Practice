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
	//完全にランダムナシード
	m_random_random_seed = rand();

	m_common_key_flag = false;

	m_bag_round_count = 0;
	Audio::Start(bgm_1);

}

//初期化
void ObjBlock::Init()
{
	InitField();
	m_lines_complete = false;
	m_gameover = false;
	m_gameclear = false;

	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		m_bag[i] = true;
	}

	m_hold_type = Mino_Empty;
	m_hold_flag = true;

	//ネクスト作成する前に巡目リセット
	m_mino_count = 0;
	m_bag_round_count = 0;
	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		m_next[i] = Mino_Empty;
	}
	NextCreate();
	MinoCreateFromNext();
 	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		//NEXTの数+1で、最初ミノがつくられる
		NextCreate();
	}

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

	//100ラインチーズモード専用
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

	m_rising_lines = 0;
	m_btb = false;
	m_ren = REN_NONE;
	m_rising_remain = 0;

}

//進行
void ObjBlock::Action()
{
	ObjPracticeOption* oOpt = (ObjPracticeOption*)Objs::GetObj(OBJ_PRACTICEOPTION);

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
	else if (Input::GetVKey(VK_SPACE) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_TOUCHPAD))
	{
		if (m_common_key_flag == true)
		{
			oOpt = new ObjPracticeOption();
			Objs::InsertObj(oOpt, OBJ_PRACTICEOPTION, PRIO_OPTION);
		}
		m_common_key_flag = false;
	}
	else
	{
		m_common_key_flag = true;
	}

	for (auto itr = m_list_garbage.begin(); itr != m_list_garbage.end(); itr++)
	{
		itr->rising_time_remain_sec.Sub(FRAME_TO_SEC);
		if (itr->rising_time_remain_sec.GetMinReached())
			itr->rising_reach = true;
	}
}

//描画
void ObjBlock::Draw()
{
	//文字色

	FieldDraw(FIELD_1P_POS_X + FIELD_OFFSET_X, FIELD_1P_POS_Y + FIELD_OFFSET_Y, m_field);

	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		float x = FIELD_1P_POS_X + NEXT_OFFSET_X;
		float y = i * NEXT_HEIGHT + FIELD_1P_POS_Y + NEXT_OFFSET_Y;
		Draw::SetColor(texNext, Color::White);
		Draw::CenterDraw(texNext, x, y);
		//0 - 6の場合
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


	//オプションの描画
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
	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		if (m_next[i] == Mino_Empty)
		{
			MINO_TYPE mino_type = BagToType();

			m_bag[mino_type] = false;


			m_next[i] = mino_type;
			return;
		}
	}
}
void ObjBlock::MinoCreateFromNext()
{
	//ミノを作成する段階でゲームオーバーの場合、作成しない
	if (GetGameOverFlag())
		return;

	if (m_next[0] != Mino_Empty)
	{
		MinoCreate(m_next[0]);
	}

	//順番入れ替え
	for (int i = 0; i < NEXT_AMOUNT - 1; i++)
	{
		m_next[i] = m_next[i + 1];
	}

	m_next[NEXT_AMOUNT - 1] = Mino_Empty;
}

MINO_TYPE ObjBlock::BagToType()
{

	Bag_All_Check();
	/// ランダム	

	//バッグ内に残っているミノの種類のカウント
	int bag_mino_count = 0;
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		if (m_bag[i] == true)
			bag_mino_count++;
	}

	int rnd = 0;
	std::random_device rand;

	if (m_bag_round_count <= Tetris::E_PRACTICE_OPTION::TetriminoOrderFixed_End && m_practice_options.option_flag[m_bag_round_count])
	{
		return m_practice_options.fixed_mino_type[m_bag_round_count][MINO_MAX_TYPE - bag_mino_count];
	}
	else
		rnd = rand() % bag_mino_count;
	

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
	return Mino_S;
}

//ミノが着地しているかどうか返す
bool ObjBlock::BlockPosCheck(int x,int y)
{
	//場外判定
	if (x < 0 || x >= FIELD_WIDTH ||
		y < 0 || y >= FIELD_HEIGHT)
		return true;

	if (m_field[y][x] != BlockEmpty)
	{
		//あった
		return true;
	}
	return false;
}

void ObjBlock::Bag_All_Check()
{
	bool all_check = false;

	//全てがfalseの場合、ネクスト作成処理無限ループを起こしてしまう可能性があるため、
	//事前にすべてがfalseでないか調べておく
	//すべてがfalseの場合は、すべてtrueに変えよう
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		if (m_bag[i] == true)
			break;
		if (i == MINO_MAX_TYPE - 1 && m_bag[i] == false)
		{
			//全てfalseでした
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
	//消されたライン数に応じてスコアを加算
	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);

	int lines_count = 0;
	bool perfect_clear = false;
	bool btb = m_btb;
	int clear_garbage_lines = 0;//おじゃま消したかどうか Cheez100ラインモード用に追加

	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
			bool is_garbage = false;
		for (int x = 0; x < FIELD_WIDTH;)
		{

			//X座標のループを抜ける
			if (m_field[y][x] == BlockEmpty)break;

			//Cheez100ライン
			if (is_garbage == false && m_field[y][x] == BlockWall)
			{
				is_garbage = true;
			}

			x++;
			//一番右まで到達した
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
			//X座標のループを抜ける
			if (m_field[FIELD_HEIGHT - 1][x] != BlockEmpty)break;
			x++;
			//一番右まで到達した
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


		//BTBの判定は最後に行う。
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

		//TSPINの引数は、TSD20モード用
		oScore->AddClearLines(lines_count , tspin);

		Obj100LineCheez* oCheez = (Obj100LineCheez*)Objs::GetObj(OBJ_100LINECHEEZ);
		if (oCheez != nullptr)
		{
			oCheez->ReportClearCheezLine(clear_garbage_lines);
			Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage = oCheez->GiveCheezLine(clear_garbage_lines);
			AddGarbageLines(garbage);
		}


	}
	else//0ライン
	{
		m_ren = REN_NONE;
	}
	oScore->AddMinoCount();
}

void ObjBlock::InitField()
{
	// FIELD_HEIGHT = ブロック置ける範囲
	// FIELD_HEIGHT + 1 =　一番下の床を考慮した範囲
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
	//前回のホールドを記憶
	MINO_TYPE old_hold = m_hold_type;
	m_hold_type = type;

	if (old_hold == Mino_Empty)
	{
		MinoCreateFromNext();
		//ホールドに何もなかった場合
		NextCreate();
	 }
	else
	{
		//すでに何かがあった場合
		MinoCreate(old_hold);
	}
}

void ObjBlock::DrawFontOption(float x, float y, float font_size) 
{
	//文字
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
			str += std::to_wstring(m_practice_options.rising_timer_sec) + L"秒";
			break;
		case E_PRACTICE_OPTION::GameMode:
			str += Tetris::PracticeOption::GetStrGameMode(m_practice_options.gamemode);
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
	//　一番下の床の分考慮
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		//+2は左右壁の分考慮
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			int bx = x * BLOCK_PIXELS + offset_x;
			int by = y * BLOCK_PIXELS + offset_y;
			//土台を描画。
			Draw::Draw(texBase, bx, by);


			
			//GameHead MapObjectsを参照
			if (field[y][x] != BlockEmpty)
			{
				BlockDraw(bx, by, field[y][x]);
			}
			
		}
	}
}

void ObjBlock::LineClear(int line_y)
{
	//ラインそろった
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		//ブロック消える
		m_field[line_y][i] = BlockEmpty;
	}
	//そのラインから上に、ずらしていく
	for (int i = line_y; i > 0; i--)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1段下げる
			m_field[i][j] = m_field[i - 1][j];
		}
	}
}

//ゲームオーバー条件(40ラインより上にブロックが到達)もここで判定する。
void ObjBlock::GarbageRising(int height)
{
	if (height <= 0)return;

	//ラインそろった
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		//かならず1ラインに1つ以上ブロックが存在するはずなので、
		//せりあがる高さ分のラインのみブロックがあるか判定する。
		if (m_field[height - 1][i] != BlockEmpty)
		{
			SetGameOverFlag(true);
		}
	}

	//そのラインから上に、ずらしていく
	for (int i = 0; i < FIELD_HEIGHT - height; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1段下げる
			m_field[i][j] = m_field[i + height][j];
		}
	}

	//70%の確率で同じ列
	std::uniform_int_distribution<int>dist(0,FIELD_WIDTH - 1);
	int space_x = dist(m_random_random_engine);//空間X

	for (int i = FIELD_HEIGHT - height; i < FIELD_HEIGHT; i++)
	{
		//30%の確率で再抽選 100ラインチーズの場合100%再抽選
		if (dist(m_random_random_engine) < 3 || m_practice_options.gamemode == E_GAME_MODE::mode_100LineCheez)
		{
			space_x = dist(m_random_random_engine);
		}

		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			//1段下げる
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
	m_rising_lines += height;
	m_rising_remain = 1;
	Audio::Start(AudioIds::se_Garbage);
}
void ObjBlock::AddGarbageLines(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage)
{
	//m_rising_lines += height;
	//m_rising_remain = 1;
	m_list_garbage.push_back(garbage);
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
	NextCreate();
}
void ObjBlock::AttackGarbage(int lines)
{
	//相殺なし
	if (m_practice_options.gamemode == E_GAME_MODE::mode_100LineCheez)
		return;

	//相殺の処理
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
	
	//ライン消去スコア加算
	if (tspin == E_TSPIN_PATTERN::TSpin)
		add_score += Score::ADD_SCORE_TSPIN[lines];
	else
		add_score += Score::ADD_SCORE_LINES[lines];

	//REN
	add_score += ren * Score::ADD_REN_SCORE;

	//BTBの場合、1.5倍
	if (btb)
		add_score *= Score::BTB_SCORE_SCALE;
	
	//TSpinMiniボーナス
	if (tspin == E_TSPIN_PATTERN::TSpinMini)
		add_score += Score::ADD_TSPINMINI_SCORE;

	//パーフェクトの場合、スコアボーナス
	if (perfect)
		add_score += Score::ADD_SCORE_PERFECT[lines];

	return add_score;
}