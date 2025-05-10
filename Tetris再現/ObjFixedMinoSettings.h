#pragma once

//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

#include <random>
using namespace Tetris;
using namespace GameL;



class ObjFixedMinoSettings : public CObj
{
public:
	ObjFixedMinoSettings(int round);
	~ObjFixedMinoSettings() {};
	void Init();
	void Action();
	void Draw();
private:
	int m_round;
	int m_mino_order[MINO_MAX_TYPE];
	int m_now_selected;
	int m_order;
	void SettingEnd();
};

