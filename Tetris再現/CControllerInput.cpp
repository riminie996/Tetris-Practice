#include "CControllerInput.h"
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")


bool CConInput::m_controller_used;

bool CConInput::GetControllerIsActive()
{
	JOYINFOEX JoyInfo;
	return joyGetPosEx(JOYSTICKID1, &JoyInfo) == JOYERR_NOERROR;
}
bool CConInput::GetConInput(Controller_Input_DualShock4 type)
{
    JOYINFOEX JoyInfo;
    unsigned long input_state = 0;


	//DUALSHOCK 4
	if (joyGetPosEx(JOYSTICKID1, &JoyInfo) == JOYERR_NOERROR) {
		//if (JoyInfo.dwXpos < INPUT_N)      input_state |=		INPUT_LEFT;
		//if (JoyInfo.dwXpos > INPUT_P)      input_state |=		INPUT_RIGHT;
		//if (JoyInfo.dwYpos < INPUT_N)      input_state |=		INPUT_UP;
		//if (JoyInfo.dwYpos > INPUT_P)      input_state |=		INPUT_DOWN;
		if(type == Controller_Input_DualShock4::INPUT_UP)	return (JoyInfo.dwPOV > 27000 && JoyInfo.dwPOV < 36000) || JoyInfo.dwPOV < 9000;
		if(type == Controller_Input_DualShock4::INPUT_RIGHT	)return JoyInfo.dwPOV > 0 && JoyInfo.dwPOV < 18000;
		if (type == Controller_Input_DualShock4::INPUT_DOWN)return JoyInfo.dwPOV > 9000 && JoyInfo.dwPOV < 27000;
		if (type == Controller_Input_DualShock4::INPUT_LEFT)return JoyInfo.dwPOV > 18000 && JoyInfo.dwPOV < 36000;
		if(type == Controller_Input_DualShock4::INPUT_Y		)return JoyInfo.dwButtons & JOY_BUTTON1;
		if(type == Controller_Input_DualShock4::INPUT_B		)return JoyInfo.dwButtons & JOY_BUTTON2;
		if(type == Controller_Input_DualShock4::INPUT_A		)return JoyInfo.dwButtons & JOY_BUTTON3;
		if(type == Controller_Input_DualShock4::INPUT_X		)return JoyInfo.dwButtons & JOY_BUTTON4;
		if(type == Controller_Input_DualShock4::INPUT_L1	)return JoyInfo.dwButtons & JOY_BUTTON5;
		if(type == Controller_Input_DualShock4::INPUT_R1	)return JoyInfo.dwButtons & JOY_BUTTON6;
		if(type == Controller_Input_DualShock4::INPUT_L2	)return JoyInfo.dwButtons & JOY_BUTTON7;
		if(type == Controller_Input_DualShock4::INPUT_R2	)return JoyInfo.dwButtons & JOY_BUTTON8;
		if(type == Controller_Input_DualShock4::INPUT_SHARE	)return JoyInfo.dwButtons & JOY_BUTTON9;
		if(type == Controller_Input_DualShock4::INPUT_OPTION)return JoyInfo.dwButtons & JOY_BUTTON10;
		if(type == Controller_Input_DualShock4::INPUT_L3	)return JoyInfo.dwButtons & JOY_BUTTON11;
		if(type == Controller_Input_DualShock4::INPUT_R3	)return JoyInfo.dwButtons & JOY_BUTTON12;
		if (type == Controller_Input_DualShock4::INPUT_PS) return JoyInfo.dwButtons & JOY_BUTTON13;
			if (type == Controller_Input_DualShock4::INPUT_TOUCHPAD) return JoyInfo.dwButtons & JOY_BUTTON14;
		//if (JoyInfo.dwButtons & JOY_BUTTON15)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON16)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON17)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON18)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON19)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON20)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON21)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON22)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON23)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON24)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON25)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON26)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON27)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON28)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON29)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON30)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON31)  input_state |= INPUT_B_UP;
		//if (JoyInfo.dwButtons & JOY_BUTTON32)  input_state |= INPUT_B_UP;

	}
	return false;
}