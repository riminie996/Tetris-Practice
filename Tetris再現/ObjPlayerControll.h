#pragma once
#include "CCounter.h"
#include "GameHead.h"
#include "ObjBlock.h"
#include "Tetris.h"
//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"

using namespace GameL;
using namespace Tetris;

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
private:
	bool m_button_once_status[(int)E_PLAYER_CONTROLL::ButtonTypeCount];
	bool m_button_flag[(int)E_PLAYER_CONTROLL::ButtonTypeCount];

};