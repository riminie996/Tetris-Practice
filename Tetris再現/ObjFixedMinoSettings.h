#pragma once

//GameL�̃w�b�_�[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

#include <random>
using namespace Tetris;
using namespace GameL;



class ObjFixedMinoSettings : public CObj
{
public:
	ObjFixedMinoSettings();
	~ObjFixedMinoSettings() {};
	void Init();
	void Action();
	void Draw();

private:
};

