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

#define ORDER_UNDEFINED (-1)

ObjFixedMinoSettings::ObjFixedMinoSettings(int round)
{
	m_round = round;
}

//èâä˙âª
void ObjFixedMinoSettings::Init()
{
	for (int i = 0; i < MINO_MAX_TYPE; i++)
		m_mino_order[i] = ORDER_UNDEFINED;

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
		//if (m_mino_order[m_order] == ORDER_UNDEFINED)
		//{
			m_mino_order[m_order] = m_now_selected;
			m_order++;
			if (m_order == MINO_MAX_TYPE)
			{
				SettingEnd();
			}
		//}
	}
	else if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_B))
	{

			SettingEnd();
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

		
	}
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		float text_x = 640.0f - 384.0f;
		float y = 360.0f;
		std::wstring wstr = std::to_wstring(i);
		Font::StrDraw(wstr.c_str(), text_x + m_mino_order[i] * 128.0f + text_offset_x, y + text_offset_y, 32.0f, ColorA::White);
	}
}


void ObjFixedMinoSettings::SettingEnd()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	for (int i = 0; i < MINO_MAX_TYPE; i++)
	{
		oBlock->GetOptions()->fixed_mino_type[m_round][i] = (MINO_TYPE)m_mino_order[i];
	}
	this->SetStatus(false);
}