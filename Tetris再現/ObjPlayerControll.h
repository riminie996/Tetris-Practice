#pragma once
#include "GameHead.h"
#include "Tetris.h"
#include "Structure.h"
using namespace GameL;



enum class E_PLAYER_CONTROLL
{
	Button_LEFT,
	Button_RIGHT,
	Button_UP,
	Button_DOWN,
	Button_A,
	Button_B,
	Button_X,
	Button_Y,
	Button_L,
	Button_R,
	Button_ZL,
	Button_ZR,
	Button_OPTION,
	Button_SHARE,
	Button_TOUCHPAD,
	ButtonTypeCount,
};

class ObjPlayerControll : public CObj
{
public:
	ObjPlayerControll() {}
	~ObjPlayerControll() {};
	void Init();
	void Action();
	void Draw();
	bool GetButtonInput(E_PLAYER_CONTROLL cont);
	bool GetButtonInputOnce(E_PLAYER_CONTROLL cont);
	unsigned int GetButtonLongPressFrame(E_PLAYER_CONTROLL cont);
private:
	unsigned int m_button_status[(int)E_PLAYER_CONTROLL::ButtonTypeCount];
};