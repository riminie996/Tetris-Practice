#pragma once
#include "GameHead.h"
#include "Tetris.h"
//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"

using namespace GameL;
using namespace Tetris;



class ObjRisingTimer : public CObj
{
public:
	ObjRisingTimer() {}
	~ObjRisingTimer() {};
	void Init();
	void Action();
	void Draw();
	void TimerReset() { m_ct_interval.Reset(); }
	void SetTimer(int sec);
private:
	CCounter m_ct_interval;
	int m_random_seed;
};