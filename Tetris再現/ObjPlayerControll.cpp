#include "ObjMino.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameHead.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "Function.h"
#include "GameL\Audio.h"


//èâä˙âª
void ObjPlayerControll::Init()
{
	for (int i = 0; i < (int)E_PLAYER_CONTROLL::ButtonTypeCount; i++)
	{
		m_button_flag[i] = false;
		m_button_once_status[i] = false;
	}
}

//êiçs
void ObjPlayerControll::Action()
{
	for (int i = 0; i < (int)E_PLAYER_CONTROLL::ButtonTypeCount; i++)
	{

		m_button_once_status[i] = false;
		bool status = GetButtonInput((E_PLAYER_CONTROLL)i);

		if (status)
		{
			if (m_button_flag[i] == true)
			{
				m_button_once_status[i] = true;
				m_button_flag[i] = false;
			}
		}
		else
		{
			m_button_flag[i] = true;
		}
	}
}

//ï`âÊ
void ObjPlayerControll::Draw()
{


}


bool ObjPlayerControll::GetButtonInput(E_PLAYER_CONTROLL cont)
{
	switch (cont)
	{
	case E_PLAYER_CONTROLL::Button_LEFT:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_LEFT);
	case E_PLAYER_CONTROLL::Button_RIGHT:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_RIGHT);
	case E_PLAYER_CONTROLL::Button_UP:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_UP);
	case E_PLAYER_CONTROLL::Button_DOWN:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_DOWN);
	case E_PLAYER_CONTROLL::Button_A:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_A);
	case E_PLAYER_CONTROLL::Button_B:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_B);
	case E_PLAYER_CONTROLL::Button_X:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_X);
	case E_PLAYER_CONTROLL::Button_Y:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_Y);
	case E_PLAYER_CONTROLL::Button_L:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_L1);
	case E_PLAYER_CONTROLL::Button_R:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_R1);
	case E_PLAYER_CONTROLL::Button_ZL:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_L2);
	case E_PLAYER_CONTROLL::Button_ZR:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_R2);
	case E_PLAYER_CONTROLL::Button_OPTION:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_OPTION);
	case E_PLAYER_CONTROLL::Button_SHARE:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_SHARE);
	case E_PLAYER_CONTROLL::Button_TOUCHPAD:
		return CConInput::GetConInput(Controller_Input_DualShock4::INPUT_TOUCHPAD);
	}
}
bool ObjPlayerControll::GetButtonInputOnce(E_PLAYER_CONTROLL cont)
{
	return GetButtonInput(cont) && m_button_once_status[(int)cont];
}