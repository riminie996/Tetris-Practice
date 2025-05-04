#include "ObjScore.h"
#include "GameL/DrawFont.h"
#include "Tetris.h"

namespace Tetris
{
	namespace Score
	{
		const Point SCORE_OFFSET = { 320,648 };
		const int SCORE_DIGIT = 7;
		const int SCORE_WIDTH = 40;
		const Point PIECE_PLACED_POS = { 704,0 };
		const Point PPS_POS = { 704,70 };
		const Point ATTACK_LINES_POS = { 704,140 };
		const Point APM_POS = { 704,210 };
		const Point CLEAR_LINES_POS = { 704,280 };
		const Point TIME_POS = { 704,350 };
		const Point NUMBER_OFFSET = { 128,32 };
		const RECT_F RECT_UMBER_COMMA = { 460,0,14,64 };

	}
}
ObjScore::ObjScore()
{
	m_score = { CCounter(0.0f,0.0f,9999999.0f,STOP) };
	m_time = { CCounter(0.0f,0.0f,0.0f,FREE) };
	m_num_score.SetTex(texNumber, Tetris::ClearText::RECT_NUMBER.x, Tetris::ClearText::RECT_NUMBER.y);
	m_num_score.SetCommaRect(Tetris::Score::RECT_UMBER_COMMA, 3);
}
//‰Šú‰»
void ObjScore::Init()
{

	m_piece_placed = 0;
	m_attack_lines = 0;
	m_pps = 0.0f;
	m_apm = 0.0f;
	m_clear_lines = 0;
	m_timeract = true;
	m_tspin_lines = 0;
}

//is
void ObjScore::Action()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	if (oBlock->GetGameOverFlag() || oBlock->GetIsPause() || oBlock->GetGameClearFlag())return;

	if (m_timeract)
		m_time.Add(FRAME_TO_SEC);

	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_ULTRA)
	{
		if (m_time.GetMaxReached())
		{
			oBlock->SetGameOverFlag(true);
		}
	}


	m_pps = CalcPiecesPerSeconds();
	m_apm = CalcAttackPerMinutes();
}

//•`‰æ
void ObjScore::Draw()
{
	//”’
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);

	Draw::SetOpacity(texNumber, 1.0f);
	Draw::SetScaleX(texNumber, 1.0f);
	Draw::SetScaleY(texNumber, 1.0f);

	m_num_score.min_digit = Tetris::Score::SCORE_DIGIT;
	m_num_score.Draw((int)m_score.NowValue, Tetris::FIELD_1P_POS_X + Tetris::Score::SCORE_OFFSET.x, Tetris::FIELD_1P_POS_Y + Tetris::Score::SCORE_OFFSET.y, Tetris::Score::SCORE_WIDTH);

	Draw::SetScaleX(texNumber, 0.5f);
	Draw::SetScaleY(texNumber, 0.5f);
	m_num_score.min_digit = 0;
	Font::StrDraw(L"Piece Placed", Tetris::Score::PIECE_PLACED_POS.x, Tetris::Score::PIECE_PLACED_POS.y, SCORE_FONT_SIZE, ColorA::White);
	m_num_score.Draw(m_piece_placed, Tetris::Score::PIECE_PLACED_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::PIECE_PLACED_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
	Font::StrDraw(L"Piece Per Seconds", Tetris::Score::PPS_POS.x, Tetris::Score::PPS_POS.y, SCORE_FONT_SIZE, ColorA::White);
	m_num_score.Draw(m_pps, Tetris::Score::PPS_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::PPS_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
	Font::StrDraw(L"Attack Lines", Tetris::Score::ATTACK_LINES_POS.x, Tetris::Score::ATTACK_LINES_POS.y, SCORE_FONT_SIZE, ColorA::White);
	m_num_score.Draw(m_attack_lines, Tetris::Score::ATTACK_LINES_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::ATTACK_LINES_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
	Font::StrDraw(L"Attack Per Minutes", Tetris::Score::APM_POS.x, Tetris::Score::APM_POS.y, SCORE_FONT_SIZE, ColorA::White);
	m_num_score.Draw(m_apm, Tetris::Score::APM_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::APM_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_TSD20)
	{
		Font::StrDraw(L"TSpin Lines", Tetris::Score::CLEAR_LINES_POS.x, Tetris::Score::CLEAR_LINES_POS.y, SCORE_FONT_SIZE, ColorA::White);
		m_num_score.Draw(m_tspin_lines, Tetris::Score::CLEAR_LINES_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::CLEAR_LINES_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
	}
	else if(oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_100LineCheez)
	{
		Obj100LineCheez* o100Line = (Obj100LineCheez*)Objs::GetObj(OBJ_100LINECHEEZ);
		if (o100Line != nullptr)
		{
			Font::StrDraw(L"Lines Remaining", Tetris::Score::CLEAR_LINES_POS.x, Tetris::Score::CLEAR_LINES_POS.y, SCORE_FONT_SIZE, ColorA::White);
			m_num_score.Draw(o100Line->GetLinesRemain(), Tetris::Score::CLEAR_LINES_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::CLEAR_LINES_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
		}
	}
	else
	{
		Font::StrDraw(L"Clear Lines", Tetris::Score::CLEAR_LINES_POS.x, Tetris::Score::CLEAR_LINES_POS.y, SCORE_FONT_SIZE, ColorA::White);
		m_num_score.Draw(m_clear_lines, Tetris::Score::CLEAR_LINES_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::CLEAR_LINES_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);

	}
	Font::StrDraw(L"Time", Tetris::Score::TIME_POS.x, Tetris::Score::TIME_POS.y, SCORE_FONT_SIZE, ColorA::White);

	float time = m_time.NowValue;
	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_ULTRA)
		time = m_time.MaxValue - m_time.NowValue;
	m_num_score.Draw(time, Tetris::Score::TIME_POS.x + Tetris::Score::NUMBER_OFFSET.x, Tetris::Score::TIME_POS.y + Tetris::Score::NUMBER_OFFSET.y, 20.0f);
}

void ObjScore::AddMinoCount()
{
	 m_piece_placed++;
}
void ObjScore::AddAttackLines(int add)
{ 
	m_attack_lines += add;
	m_apm = CalcAttackPerMinutes();
}

float ObjScore::CalcPiecesPerSeconds()
{
	if (m_time.NowValue != 0)
	{
		return m_piece_placed / m_time.NowValue;
	}
	return 0.0f;
}
float ObjScore::CalcAttackPerMinutes()
{
	if (m_time.NowValue != 0)
	{
		return m_attack_lines /  m_time.NowValue  * 60.0f;
	}
	return 0.0f;
}

void ObjScore::Reset()
{
	m_score.Reset();
	m_time.Reset();
	Init();

	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_ULTRA)
	{
		m_time = { CCounter(0.0f,0.0f,Tetris::ULTRA_MODE_SEC,STOP) };
	}
	else
	{
		m_time = { CCounter(0.0f,0.0f,0.0f,FREE) };
	}


}

void ObjScore::AddClearLines(int add, E_TSPIN_PATTERN tspin)
{
	m_clear_lines += add;


	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_40Line &&
		m_clear_lines >= 40)
	{
		TimerStop();
		oBlock->SetGameClearFlag(true);
	}
	if (oBlock->GetOptions()->gamemode == Tetris::E_GAME_MODE::mode_TSD20)
	{
		if (tspin != E_TSPIN_PATTERN::NoTSpin)
		{
			m_tspin_lines += add;
		}
		if (m_tspin_lines >= 40)
		{
			TimerStop();

			oBlock->SetGameClearFlag(true);
		}
	}
		
	return;
}