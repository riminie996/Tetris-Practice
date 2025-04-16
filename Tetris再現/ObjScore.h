#pragma once
#include "GameHead.h"
#include "GameL\SceneObjManager.h"

using namespace GameL;

class ObjScore :public CObj
{
public:
	ObjScore() {}
	~ObjScore() {}
	void Init();
	void Action();
	void Draw();

	void AddScore(int add) { m_score.Add(add); }
private:
	CCounter m_score;
};