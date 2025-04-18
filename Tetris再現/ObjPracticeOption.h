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
};