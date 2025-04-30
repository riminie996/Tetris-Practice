#include "ObjGuide.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "Function.h"

void ObjRisingTimer::Init()
{
	m_ct_interval = { CCounter(RISING_INTERVAL_SEC,0.0f,Tetris::RISING_INTERVAL_SEC,STOP) };


}
void ObjRisingTimer::Action()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	if (!oBlock->GetIsPause())
	{
		m_ct_interval.Sub(FRAME_TO_SEC);
	}
	if (m_ct_interval.GetMinReached())
	{
		m_ct_interval.Reset();
		std::random_device rand;
		std::uniform_int_distribution<int> dist(1, 7);

		oBlock->AddGarbageLines(dist(rand));
		
		/*m_rising_lines++;*/
	}
}
void ObjRisingTimer::Draw()
{

	wchar_t wcr[64];
	Font::StrDraw(L"RISING TIMER", RISING_TEXT_OFFSET_X, RISING_TEXT_OFFSET_Y, RISING_FONT_SIZE, ColorA::White);
	//std::wstring str = std::to_wstring(m_mino_count);
	swprintf_s(wcr, L"%02d:%02d", (int)m_ct_interval.NowValue / 60, (int)m_ct_interval.NowValue);
	Font::StrDraw(wcr, RISING_TIMER_OFFSET_X, RISING_TIMER_OFFSET_Y, RISING_FONT_SIZE, ColorA::White);
}
void ObjRisingTimer::SetTimer(int sec)
{
	m_ct_interval = { CCounter(sec,0.0f,sec,STOP) };
}