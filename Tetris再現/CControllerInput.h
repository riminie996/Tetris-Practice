#pragma once
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")


enum class Controller_Input_DualShock4
{
	INPUT_UP		,
	INPUT_DOWN		,
	INPUT_LEFT		,
	INPUT_RIGHT		,
	INPUT_L1		,
	INPUT_L2		,
	INPUT_R1		,
	INPUT_R2		,
	INPUT_A			,
	INPUT_B			,
	INPUT_X			,
	INPUT_Y			,
	INPUT_SHARE		,
	INPUT_OPTION	,
	INPUT_L3		,
	INPUT_R3		,
	INPUT_TOUCHPAD	,
	INPUT_PS		,
	INPUT_TYPE_COUNT,
	INPUT_P = 0xBFFF,  //アナログパッドの閾値。適当
	INPUT_N = 0x3FFF,  //アナログパッドの閾値。適当
};

enum BUTTON_NUM
{
	button_A,
	button_B,
	button_X,
	button_Y,
	button_Up,
	button_Down,
	button_Left,
	button_Right,
	button_Option,
	button_Share,
	button_L1,
	button_L2,
	button_L3,
	button_R1,
	button_R2,
buttonTypeCount,
};


typedef class CConInput
{
public:
	CConInput();
	static bool GetControllerIsActive();
	static bool GetConInput(Controller_Input_DualShock4 type);

private:
	static bool m_controller_used;
	static bool _is_pressed[(int)Controller_Input_DualShock4::INPUT_TYPE_COUNT];
}InputC;

//
// コントローラー入力について
// A 1
// B 2
// C 4
// D 8
// RIGHT 256
// UP 512
// DOWN 1024
// LEFT 2048
// だとしたら
// 
//
