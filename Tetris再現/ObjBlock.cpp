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
	//for (int i = 0; i < E_PRACTICE_OPTION::Option_Count; i++)
	//{
	//	m_practice_options[i] = false;
	//}
	m_common_key_flag = false;

	m_bag_round_count = 0;
	Audio::Start(bgm_1);
	m_practice_options.rising_timer_sec = 0;
}

//初期化
void ObjBlock::Init()
{
	InitField();
	m_lines_complete = false;
	m_gameover = false;

	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		m_bag[i] = true;
	}
	for (int i = 0; i < NEXT_AMOUNT; i++)
	{
		// -1 = empty
		m_next[i] = Mino_Empty;
	}
	m_hold_type = Mino_Empty;
	m_hold_flag = true;

	//ランダム関連
	//unsigned int now = (unsigned int)time(0);
	//srand(now);

	//ネクスト作成する前に巡目リセット
	m_mino_count = 0;
	m_bag_round_count = 0;
	for (int i = 0; i < NEXT_AMOUNT + 1; i++)
	{
		//NEXTの数+1で、最初ミノがつくられる
		NextCreate();
	}


	m_pause_flag = false;

	std::random_device rand;
	//完全にランダムナシード
	m_random_random_seed = rand();

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
	
	m_rising_lines = 0;
	m_btb = false;
	m_ren = -1;
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
}

//描画
void ObjBlock::Draw()
{
	//文字色
	float c[4] = { 1.0f,1.0f,1.0f,1.0f };
	if (m_gameover == false)
	{
		FieldDraw(FIELD_1P_POS_X + FIELD_OFFSET_X, FIELD_1P_POS_Y + FIELD_OFFSET_Y,m_field);

		for (int i = 0; i < NEXT_AMOUNT; i++)
		{
			float x = FIELD_1P_POS_X + NEXT_OFFSET_X;
			float y = i * NEXT_HEIGHT + FIELD_1P_POS_Y + NEXT_OFFSET_Y;
			Draw::CenterDraw(texNext, x, y);
			//0 - 6の場合
			if (m_next[i] > Mino_Empty)
			{
				Next_Mino_Draw(x, y, m_next[i]);
			}
		}
		//								64 = 中心に描画したいため、枠のピクセル数である128÷2の位置

		float hx = FIELD_1P_POS_X + HOLD_OFFSET_X;
		float hy = FIELD_1P_POS_Y + HOLD_OFFSET_Y;
		Draw::CenterDraw(texNext, hx, hy);
		if (m_hold_type != Mino_Empty)
		{
			Next_Mino_Draw(hx, hy, m_hold_type);
		}
		Font::StrCenterDraw(L"NEXT", FIELD_1P_POS_X + NEXT_OFFSET_X, FIELD_1P_POS_Y + NEXT_OFFSET_Y + NEXT_MOJI_OFFSET_Y, BLOCK_PIXELS, c);
		Font::StrCenterDraw(L"HOLD", hx, hy + NEXT_MOJI_OFFSET_Y, BLOCK_PIXELS, c);

		wchar_t wcr[64];
		std::wstring str = std::to_wstring(m_mino_count);
		swprintf_s(wcr, L"%s", str.c_str());
		Font::StrDraw(wcr, COUNT_TEXT_X, COUNT_TEXT_Y, COUNT_TEXT_SIZE, ColorA::White);



		//オプションの描画
		DrawFontOption(OPTION_TEXT_OFFSET_X, OPTION_TEXT_OFFSET_Y, OPTION_TEXT_SIZE);
	}
	else if (m_gameover == true)
	{
		Font::StrCenterDraw(L"GAME OVER", GAMEOVER_TEXT_X, GAMEOVER_TEXT_Y, GAMEOVER_TEXT_SIZE, c);
	}

	RisingLinesBlockDraw(m_rising_lines);


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
	//ミノを作成する段階でゲームオーバーの場合、作成しない
	if (m_gameover == true)
		return;


	MINO_TYPE mino_type = BagToType();

	m_bag[mino_type] = false;

	if (m_next[0] != Mino_Empty)
	{
		MinoCreate(m_next[0]);
	}

	//順番入れ替え
	for (int i = 0; i < NEXT_AMOUNT - 1; i++)
	{
		m_next[i] = m_next[i+1];
	
	}
	m_next[NEXT_AMOUNT - 1] = mino_type;
	

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

	//ランダムインスタンス作成
	std::uniform_int_distribution<int> dist(0, bag_mino_count - 1);

	int rnd = 0;

	if (m_bag_round_count <= Tetris::E_PRACTICE_OPTION::TetriminoOrderFixed_End && m_practice_options.option_flag[m_bag_round_count])
	{
		return m_practice_options.fixed_mino_type[m_bag_round_count][MINO_MAX_TYPE - bag_mino_count];
	}
	else
		rnd = dist(m_random_random_engine);
	

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
	int lines_count = 0;
	bool perfect_clear = false;
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < FIELD_WIDTH;)
		{
			//X座標のループを抜ける
			if (m_field[y][x] == BlockEmpty)break;

			x++;
			//一番右まで到達した
			if (x == FIELD_WIDTH)
			{
				lines_count++;
				LineClear(y);
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
		//消されたライン数に応じてスコアを加算
		ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);
		oScore->AddScore(ADD_SCORE_LINES[lines_count]);

		if (perfect_clear)
		{
			Audio::Start(se_Perfect);
		}

		ObjMino* mino = (ObjMino*)Objs::GetObj(OBJ_MINO);
		E_TSPIN_PATTERN tspin = mino->GetTSpinFlag();

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
		}


		m_ren++;

		int attack_lines = CalcSendGarbageLines(lines_count, m_ren, m_btb, tspin, perfect_clear);
		AttackGarbage(attack_lines);
		
	}
	else//0ライン
	{
		m_ren = -1;
	}
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
		//ホールドに何もなかった場合
		NextCreate();
	 }
	else
	{
		//すでに何かがあった場合
		MinoCreate(old_hold);
	}
}

void ObjBlock::Next_Mino_Draw(float x, float y, int type)
{
	float offset_x = 0.0f;
	float offset_y = 0.0f;
	//真ん中に表示させたいため、ずらす
	if (type == Mino_I || type == Mino_O)
	{
		offset_y = BLOCK_PIXELS;
	}
	else//3×3ブロックのやつ
	{
		offset_x = BLOCK_PIXELS / 2.0f;
		offset_y = BLOCK_PIXELS;
	}

	//初期位置
	float bx = x + ((float)MINO_DATABASE_LOW / 2.0f) * -BLOCK_PIXELS + offset_x;
	float by = y + ((float)MINO_DATABASE_COL / 2.0f) * -BLOCK_PIXELS + offset_y;
	RECT_F Color_Block = { 0,BLOCK_PIXELS * type,BLOCK_PIXELS,BLOCK_PIXELS };

	for (int i = 0; i < MINO_DATABASE_COL; i++)
	{
		for (int j = 0; j < MINO_DATABASE_LOW; j++)
		{
			if (MINO_SHAPE[type][i][j] == 1)
			{
				Draw::Draw(texBlock, bx + (j * BLOCK_PIXELS), by + (i * BLOCK_PIXELS), Color_Block);
				Draw::Draw(texBlock, bx + (j * BLOCK_PIXELS), by + (i * BLOCK_PIXELS), BLOCK_LIGHT_SRC_POS);
			}
		}
	}
}

void ObjBlock::DrawFontOption(float x, float y, float font_size) 
{
	//文字色
	float c[4] = { 1.0f,1.0f,1.0f,1.0f };
	//Font::StrDraw(L"OPTIONS:", x, y, font_size, c);
	Font::StrDraw(L"OPTIONS", x, y, font_size, ColorA::White);

	wchar_t wcr[64];
	std::wstring str;
	int active_options_count = 1;
	for (int i = 0; i < E_PRACTICE_OPTION::Option_Count; i++)
	{
		float option_text_offset_y = font_size * active_options_count;

		if (i <= E_PRACTICE_OPTION::TetriminoOrderFixed_End)
			str = L"テトリミノ" + std::to_wstring(i + 1) + L"巡目固定";

		switch (i)
		{
		case E_PRACTICE_OPTION::NoNaturalDrop:
			str = L"自然落下なし";
			break;
		case E_PRACTICE_OPTION::InfiniteHold:
			str = L"無限ホールド";
			break;
		case E_PRACTICE_OPTION::RisingTimer:
			str = L"せりあがりタイマー:" + std::to_wstring(m_practice_options.rising_timer_sec);
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
			m_gameover = true;
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
		//30%の確率で再抽選
		if (dist(m_random_random_engine) < 3)
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

void ObjBlock::RisingLinesBlockDraw(int lines)
{
	for (int i = 0; i < lines; i++)
	{
		MapObjects color = m_rising_remain == 0 ? BlockRed : BlockWall;
		BlockDraw(Tetris::RISING_LINES_OFFSET_X, Tetris::RISING_LINES_OFFSET_Y + i * (-Tetris::BLOCK_PIXELS), color);
	}
}



void ObjBlock::FieldUpdate()
{
	LinesCompleteCheck();

	if (m_rising_remain <= 0)
	{
		if (m_rising_lines > 0)
		{
			GarbageRising(m_rising_lines);
			m_rising_lines = 0;
		}
	}
	else
	{
		m_rising_remain--;
	}
}
void ObjBlock::AttackGarbage(int lines)
{
	if (m_rising_lines > 0)
	{
		m_rising_lines -= lines;
	
		if (m_rising_lines < 0)
			m_rising_lines = 0;
	}

}