#include "ObjFixedMinoSettings.h"
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


ObjFixedMinoSettings::ObjFixedMinoSettings(int round)
{
	m_round = round;
}

//èâä˙âª
void ObjFixedMinoSettings::Init()
{
	for (int i = 0; i < MINO_MAX_TYPE; i++)
		m_mino_order[i] = -1;

	m_now_selected = 0;
	m_order = 0;
}

//êiçs
void ObjFixedMinoSettings::Action()
{
	ObjPlayerControll* ctrl = (ObjPlayerControll*)Objs::GetObj(OBJ_PLAYERCONTROLL);
	if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_LEFT))
	{
		if (m_now_selected > 0)
			m_now_selected--;
	}
	else if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_RIGHT))
	{
		if (m_now_selected < MINO_MAX_TYPE - 1)
			m_now_selected++;
	}
	else if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_A))
	{
		m_mino_order[m_now_selected] = m_order;
		m_order++;
		if (m_order == MINO_MAX_TYPE)
			this->SetStatus(false);
	}
}

//ï`âÊ
void ObjFixedMinoSettings::Draw()
{

	ColorScreenDraw(Color::Black, 0.5f);
	float text_offset_x = -64.0f;
	float text_offset_y = -64.0f;

	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		float x = 640.0f-384.0f;
		float y = 360.0f;
		x += i * 128.0f;

		COLOR c_next = m_now_selected == i ? Color::Red : Color::White;
		Draw::SetColor(texNext, c_next);
		Draw::CenterDraw(texNext, x, y);
		Mino_Shape_Draw(x, y, (MINO_TYPE)i);
		std::wstring wstr = std::to_wstring(m_mino_order[i]);
		Font::StrDraw(wstr.c_str(), x + text_offset_y, y + text_offset_y, 32.0f, ColorA::White);
	}

}

