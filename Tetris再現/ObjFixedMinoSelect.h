#pragma once

//GameL�̃w�b�_�[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

#include <random>
using namespace Tetris;
using namespace GameL;



class ObjFixedMinoSelect : public CObj
{
public:
	ObjFixedMinoSelect();
	~ObjFixedMinoSelect() {};
	void Init();
	void Action();
	void Draw();

private:
};

