#pragma once
#include "GameHead.h"
#include "Tetris.h"
//GameLÇÃÉwÉbÉ_Å[
#include "GameL\SceneObjManager.h"

using namespace GameL;
using namespace Tetris;



class ObjPracticeOption : public CObj
{
public:
	ObjPracticeOption() {}
	~ObjPracticeOption() {};
	void Init();
	void Action();
	void Draw();

private:
	ST_PRACTICE_OPTION* m_p_option;
	bool m_common_key_flag;
	int m_now_selected_option;
};