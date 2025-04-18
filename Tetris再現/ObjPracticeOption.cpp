#include "ObjPracticeOption.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "Function.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"

void ObjPracticeOption::Init()
{
	m_now_selected_option = 0;
	m_common_key_flag = false;
}
void ObjPracticeOption::Action()
{

	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
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
			//�~�m���Œ�
			if (m_now_selected_option <= E_PRACTICE_OPTION::TetriminoOrderFixed_End)
			{
				m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
				if (m_p_option->option_flag[m_now_selected_option])
				{

				}

			}

			switch (m_now_selected_option)
			{
			case E_PRACTICE_OPTION::NoNaturalDrop:
			case E_PRACTICE_OPTION::DPCGuide:
			case E_PRACTICE_OPTION::InfiniteHold:
				m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
				break;
			case E_PRACTICE_OPTION::RisingTimer:
				m_p_option->rising_timer_sec--;
				break;
			}
		}
		m_common_key_flag = false;
	}
	else if (Input::GetVKey(VK_RIGHT) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_RIGHT))
	{
		if (m_common_key_flag == true)
		{
			if (m_now_selected_option <= E_PRACTICE_OPTION::TetriminoOrderFixed_End)
			{
				m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
				if (m_p_option->option_flag[m_now_selected_option])
				{

				}

			}

			switch (m_now_selected_option)
			{
			case E_PRACTICE_OPTION::NoNaturalDrop:
			case E_PRACTICE_OPTION::DPCGuide:
			case E_PRACTICE_OPTION::InfiniteHold:
				m_p_option->option_flag[m_now_selected_option] = !m_p_option->option_flag[m_now_selected_option];
				break;
			case E_PRACTICE_OPTION::RisingTimer:
				m_p_option->rising_timer_sec++;
				break;
			}
		}
		m_common_key_flag = false;
	}
	else if (Input::GetVKey(VK_SPACE) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_TOUCHPAD))
	{
		if (m_common_key_flag == true)
		{
			this->SetStatus(false);
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

	Draw::SetOpacity(texBlack32, 0.5f);
	GameL::RECT_F rect_tile = { 0.0f, 0.0f, 32.0f, 32.0f };
	DrawFill(texBlack32, rect_tile);

	std::wstring str;
	int active_options_count = 1;
	for (int i = 0; i < E_PRACTICE_OPTION::Option_Count; i++)
	{
		float option_text_offset_y = font_size * active_options_count;


		if (i <= E_PRACTICE_OPTION::TetriminoOrderFixed_End)
			str = L"�e�g���~�m" + std::to_wstring(i + 1) + L"���ڌŒ�";
		switch (i)
		{
		case E_PRACTICE_OPTION::NoNaturalDrop:
			str = L"���R�����Ȃ�";
			break;
		case E_PRACTICE_OPTION::DPCGuide:
			str = L"";
			break;
		case E_PRACTICE_OPTION::InfiniteHold:
			str = L"�����z�[���h";
			break;
		case E_PRACTICE_OPTION::RisingTimer:
			str = L"���肠����^�C�}�[:" + std::to_wstring(m_p_option->rising_timer_sec);
			break;
		}

		if (m_p_option->option_flag[i])
			str += L"\tON";
		else
			str += L"\tOFF";

		COLOR_A text_color = m_now_selected_option == i ? ColorA::Red : ColorA::White;

		Font::StrDraw(str.c_str(), x, y + option_text_offset_y, font_size, text_color);
		active_options_count++;


	}
}