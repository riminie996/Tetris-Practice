#include "Obj100LineCheez.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "Function.h"


Obj100LineCheez::Obj100LineCheez()
{

	m_num_score.SetTex(texNumber, Tetris::ClearText::RECT_NUMBER.x, Tetris::ClearText::RECT_NUMBER.y);
}

//èâä˙âª
void Obj100LineCheez::Init()
{
	m_cheez_remains = Tetris::Cheez100::CHEEZ_LINES;
	m_clear_line_remains = Tetris::Cheez100::CHEEZ_LINES;
}

//êiçs
void Obj100LineCheez::Action()
{

}

//ï`âÊ
void Obj100LineCheez::Draw()
{
	//Draw::SetOpacity(texNumber, 1.0f);
	//Draw::SetScaleX(texNumber, 1.0f);
	//Draw::SetScaleY(texNumber, 1.0f);

	//m_num_score.Draw(m_cheez_remains,RISING_TEXT_OFFSET_X, RISING_TEXT_OFFSET_Y,40);
	//m_num_score.Draw(m_clear_line_remains,RISING_TEXT_OFFSET_X, RISING_TEXT_OFFSET_Y + 60,40);
}

Tetris::RisingGarbage::ST_FIELD_GARBAGE Obj100LineCheez::GiveCheezLine(int clear_lines)
{
	Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage;
	m_cheez_remains -= clear_lines;
	if (m_cheez_remains < 0)
	{
		clear_lines += m_cheez_remains;
		m_cheez_remains = 0;
	}

	garbage.lines = clear_lines;
	garbage.rising_reach = false;
	garbage.rising_time_remain_sec = { CCounter(0.0f,0.0f,0.0f,STOP) };

	return garbage;
}
void Obj100LineCheez::ReportClearCheezLine(int lines)
{
	m_clear_line_remains -= lines;


	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	if (m_clear_line_remains == 0)
		oBlock->SetGameClearFlag(true);
}