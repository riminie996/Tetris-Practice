#include "ObjMino.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameHead.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "Function.h"
#include "GameL\Audio.h"
namespace Tetris
{
	namespace Controll
	{
		const Point BASE_POS = { 640,360 };
		const Point BUTTON_LEFT_NUMBER_OFFSET = {88,116 };
		const Point BUTTON_RIGHT_NUMBER_OFFSET = {145,116 };
		const RECT_F RECT_BUTTON_L = { 105.0f,31.0f,73.0f,16.0f };
		const RECT_F RECT_BUTTON_R={ 462.0f,31.0f,73.0f,16.0f };
		const RECT_F RECT_BUTTON_UP={ 120.0f,85.0f,34.0f,40.0f };
		const RECT_F RECT_BUTTON_DOWN={ 120.0f,141.0f,34.0f,40.0f };
		const RECT_F RECT_BUTTON_A={ 528.0f,114.0f,38.0f,38.0f };
		const RECT_F RECT_BUTTON_B={ 485.0f,157.0f,38.0f,38.0f };
	}
}

//初期化
void ObjPlayerControll::Init()
{
	//押されていない場合、0
	for (int i = 0; i < (int)E_PLAYER_CONTROLL::ButtonTypeCount; i++)
	{
		m_button_status[i] = 0;
	}
}

//進行
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

//描画
void ObjPlayerControll::Draw()
{

	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);

	if (!oBlock->GetOptions()->option_flag[E_PRACTICE_OPTION::ShowController])return;

	Draw::Draw(texControllerBase, Tetris::Controll::BASE_POS.x, Tetris::Controll::BASE_POS.y);
	std::wstring strnum = std::to_wstring(m_button_status[(int)E_PLAYER_CONTROLL::Button_LEFT]);
	Font::StrDraw(strnum.c_str(), Tetris::Controll::BASE_POS.x + Tetris::Controll::BUTTON_LEFT_NUMBER_OFFSET.x, Tetris::Controll::BASE_POS.y + Tetris::Controll::BUTTON_LEFT_NUMBER_OFFSET.y,32.0f,ColorA::White);
	strnum = std::to_wstring(m_button_status[(int)E_PLAYER_CONTROLL::Button_RIGHT]);
	Font::StrDraw(strnum.c_str(), Tetris::Controll::BASE_POS.x + Tetris::Controll::BUTTON_RIGHT_NUMBER_OFFSET.x, Tetris::Controll::BASE_POS.y + Tetris::Controll::BUTTON_RIGHT_NUMBER_OFFSET.y, 32.0f, ColorA::White);
	
	RECT_F rect = Tetris::Controll::RECT_BUTTON_UP;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_UP])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect); 

	rect = Tetris::Controll::RECT_BUTTON_DOWN;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_DOWN])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect);

	rect = Tetris::Controll::RECT_BUTTON_A;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_A])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect);

	rect = Tetris::Controll::RECT_BUTTON_B;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_B])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect);

	rect = Tetris::Controll::RECT_BUTTON_L;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_L])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect);


	rect = Tetris::Controll::RECT_BUTTON_R;
	if (m_button_status[(int)E_PLAYER_CONTROLL::Button_R])
		Draw::Draw(texControllerInput, Tetris::Controll::BASE_POS.x + rect.m_x, Tetris::Controll::BASE_POS.y + rect.m_y, rect);
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
//1フレーム目のみ判定を行う。
bool ObjPlayerControll::GetButtonInputOnce(E_PLAYER_CONTROLL cont)
{
	return m_button_status[(int)cont] == 1;
}
unsigned int ObjPlayerControll::GetButtonLongPressFrame(E_PLAYER_CONTROLL cont)
{
	return m_button_status[(int)cont];
}