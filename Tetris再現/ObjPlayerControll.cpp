#include "ObjMino.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameHead.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "Function.h"
#include "GameL\Audio.h"


//������
void ObjPlayerControll::Init()
{
	//������Ă��Ȃ��ꍇ�A0
	for (int i = 0; i < (int)E_PLAYER_CONTROLL::ButtonTypeCount; i++)
	{
		m_button_status[i] = 0;
	}
}

//�i�s
void ObjPlayerControll::Action()
{
	for (int i = 0; i < (int)E_PLAYER_CONTROLL::ButtonTypeCount; i++)
	{

		bool status = GetButtonInput((E_PLAYER_CONTROLL)i);

		if (status)
		{
			m_button_status[i]++;

		}
		else
		{
			m_button_status[i] = 0;
		}
	}
}

//�`��
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
//1�t���[���ڂ̂ݔ�����s���B
bool ObjPlayerControll::GetButtonInputOnce(E_PLAYER_CONTROLL cont)
{
	return m_button_status[(int)cont] == 1;
}
unsigned int ObjPlayerControll::GetButtonLongPressFrame(E_PLAYER_CONTROLL cont)
{
	return m_button_status[(int)cont];
}