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
		const Point PPS_POS = { 704,80 };
		const Point ATTACK_LINES_POS = { 704,160 };
		const Point APM_POS = { 704,240 };
		const Point NUMBER_OFFSET = { 128,32 };
		const RECT_F RECT_UMBER_COMMA = { 460,0,14,64 };
	}
}
//èâä˙âª
void ObjScore::Init()
{
	m_score = { CCounter(0.0f,0.0f,9999999.0f,STOP) };
	m_time = { CCounter(0.0f,0.0f,0.0f,FREE) };
	m_piece_placed = 0;
	m_attack_lines = 0;
	m_pps = 0.0f;
	m_apm = 0.0f;
	m_num_score.SetTex(texNumber, Tetris::ClearText::RECT_NUMBER.x, Tetris::ClearText::RECT_NUMBER.y);
	m_num_score.SetCommaRect(Tetris::Score::RECT_UMBER_COMMA, 3);
}

//êiçs
void ObjScore::Action()
{
	m_time.Add(1.0f / 60.0f);
}

//ï`âÊ
void ObjScore::Draw()
{
	//îí


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
}

void ObjScore::AddMinoCount()
{
	 m_piece_placed++;
	 m_pps = CalcPiecesPerSeconds();
}
void ObjScore::AddAttackLines(int add)
{ 
	m_attack_lines += add;
	m_apm = CalcAttackPerMinutes();
}

float ObjScore::CalcPiecesPerSeconds()
{
	if (m_piece_placed != 0)
	{
		return m_time.NowValue / m_piece_placed;
	}
	return 0.0f;
}
float ObjScore::CalcAttackPerMinutes()
{
	if (m_attack_lines != 0)
	{
		return m_time.NowValue * 60.0f / m_attack_lines;
	}
	return 0.0f;
}

void ObjScore::Reset()
{
	m_score.Reset();
	m_time.Reset();
	m_piece_placed = 0;
	m_attack_lines = 0;
	m_pps = 0.0f;
	m_apm = 0.0f;
}