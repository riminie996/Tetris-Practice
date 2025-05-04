#pragma once

//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

using namespace GameL;

namespace Tetris
{
	namespace Cheez100
	{
		const int CHEEZ_LINES = 100;
		const int INIT_CHEEZ_LINE = 9;
	}
}

class Obj100LineCheez : public CObj
{
public:
	Obj100LineCheez();
	~Obj100LineCheez() {};
	void Init();
	void Action();
	void Draw();

	Tetris::RisingGarbage::ST_FIELD_GARBAGE GiveCheezLine(int clear_lines);
	void ReportClearCheezLine(int lines);
	int GetLinesRemain() { return m_clear_line_remains; }
private:
	int m_cheez_remains;
	int m_clear_line_remains;
	CNumTexSource m_num_score;
};

