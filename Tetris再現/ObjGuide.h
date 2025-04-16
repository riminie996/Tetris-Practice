#pragma once
//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

using namespace Tetris;
using namespace GameL;



class ObjGuide : public CObj
{
public:
	ObjGuide(){}
	~ObjGuide() {};
	void Init();
	void Action();
	void Draw();
	void ShowDPC() { m_is_show_dpc = true; }
	void Reset();
private:
	bool m_is_show_dpc;
	int m_pattern;
	MapObjects m_field[Tetris::FIELD_HEIGHT][Tetris::FIELD_WIDTH] = { BlockEmpty };
};

