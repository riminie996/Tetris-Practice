#include "ObjPracticeOption.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "Function.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "GameL\UserData.h"

void ObjPracticeOption::Init()
{
	m_now_selected_option = 0;
	m_common_key_flag = false;
}
void ObjPracticeOption::Action()
{

	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	ObjFixedMinoSettings* oFixed = (ObjFixedMinoSettings*)Objs::GetObj(OBJ_FIXEDMINOSELECT);

	if (oFixed != nullptr)return;

	m_p_option = oBlock->GetOptions();

	if (Input::GetVKey(VK_UP) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_UP))
	{
		if (m_common_key_flag == true)
		{
			if (m_now_selected_option > 0)
				m_now_selected_option--;
		}
		m_common_key_flag = false;
	}
	else if (Input::GetVKey(VK_DOWN) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_DOWN))
	{
		if (m_common_key_flag == true)
		{
			if (m_now_selected_option < E_PRACTICE_OPTION::Option_Count - 1)
				m_now_selected_option++;
		}
		m_common_key_flag = false;
	}
	else if ((Input::GetVKey(VK_LEFT) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_LEFT)))
	{
		if (m_common_key_flag == true)
		{
			ChangeParameter(Left);
		}
		m_common_key_flag = false;
	}
	else if (Input::GetVKey(VK_RIGHT) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_RIGHT))
	{
		if (m_common_key_flag == true)
		{
			ChangeParameter(Right);
		}
		m_common_key_flag = false;
	}
	else if (Input::GetVKey(VK_SPACE) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_TOUCHPAD))
	{
		if (m_common_key_flag == true)
		{
			this->SetStatus(false);
			Save::Seve();
		}
		m_common_key_flag = false;
	}

	else
	{
		m_common_key_flag = true;
	}
}
void ObjPracticeOption::Draw()
{

	float x = 0.0f, y = 0.0f, font_size = 32.0f;

	ColorScreenDraw(Color::Black, 0.5f);

	std::wstring str;
	int active_options_count = 1;
	for (int i = 0; i < E_PRACTICE_OPTION::Option_Count; i++)
	{
		float option_text_offset_y = font_size * active_options_count;

		str = Tetris::GetOptionName((E_PRACTICE_OPTION)i);
		switch (i)
		{

		case E_PRACTICE_OPTION::RisingTimer:
			str += std::to_wstring(m_p_option->rising_timer_sec) + L"秒";
			break;
		case E_PRACTICE_OPTION::User_AutoRepeatRate:
			str += std::to_wstring(USER_DATA->m_frame_AutoRepeatRate) + L"フレーム";
			break;
		case E_PRACTICE_OPTION::User_DelayerAutoShift:
			str += std::to_wstring(USER_DATA->m_frame_DelayerAutoShift) + L"フレーム";
			break;
		case E_PRACTICE_OPTION::User_SDF:
			str += std::to_wstring(USER_DATA->m_SDF_frame) + L"フレーム";
			break;
		case E_PRACTICE_OPTION::User_ReverseRotate:
			
				if (USER_DATA->m_reverse_rotate)
					str += L"\tON";
				else
					str += L"\tOFF";
			break;
		case E_PRACTICE_OPTION::User_AccidentalHardDrop:
			break;
		case E_PRACTICE_OPTION::User_NextCreateDelayFrame:
			break;
		default:
			if (m_p_option->option_flag[i])
				str += L"\tON";
			else
				str += L"\tOFF";
			break;
		}


		COLOR_A text_color = m_now_selected_option == i ? ColorA::Red : ColorA::White;

		Font::StrDraw(str.c_str(), x, y + option_text_offset_y, font_size, text_color);
		active_options_count++;


	}
}

void ObjPracticeOption::ChangeParameter(DIRECTION dir)
{
	int add = dir == Left ? -1 : 1;

	//ミノ巡固定
	if (m_now_selected_option <= E_PRACTICE_OPTION::TetriminoOrderFixed_End)
	{
		m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
		if (m_p_option->option_flag[m_now_selected_option])
		{
			ObjFixedMinoSettings* oFixed = new ObjFixedMinoSettings(m_now_selected_option);
			Objs::InsertObj(oFixed, OBJ_FIXEDMINOSELECT, PRIO_FIXED_MINO_SETTINGS);

		}

	}

	switch (m_now_selected_option)
	{
	case E_PRACTICE_OPTION::NoNaturalDrop:
	case E_PRACTICE_OPTION::ShowController:
	case E_PRACTICE_OPTION::InfiniteHold:
		m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
		break;
	case E_PRACTICE_OPTION::RisingTimer:
		m_p_option->rising_timer_sec += add;
		break;
	case E_PRACTICE_OPTION::User_AutoRepeatRate:
		USER_DATA->m_frame_AutoRepeatRate += add;
		break;
	case E_PRACTICE_OPTION::User_DelayerAutoShift:
		USER_DATA->m_frame_DelayerAutoShift += add;
		break;
	case E_PRACTICE_OPTION::User_SDF:
		USER_DATA->m_SDF_frame += add;
		break;
	case E_PRACTICE_OPTION::User_ReverseRotate:
		USER_DATA->m_reverse_rotate = !USER_DATA->m_reverse_rotate;
		break;
	case E_PRACTICE_OPTION::User_AccidentalHardDrop:
		USER_DATA->m_accidental_harddrop_frame += add;
		break;
	case E_PRACTICE_OPTION::User_NextCreateDelayFrame:
		USER_DATA->m_next_create_delay_frame += add;
		break;
	}
}