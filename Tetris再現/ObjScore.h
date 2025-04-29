#pragma once
#include "GameHead.h"
#include "GameL\SceneObjManager.h"
#include "CNumTexSource.h"

using namespace GameL;

class ObjScore :public CObj
{
public:
	ObjScore();
	~ObjScore() {}
	void Init();
	void Action();
	void Draw();

	void AddScore(int add) { m_score.Add(add); }
	void AddMinoCount();
	void AddAttackLines(int add);
//	void AddClearLines(int add);
	void AddClearLines(int add,E_TSPIN_PATTERN tspin);
	void TimerStart() { m_timeract = true; }
	void TimerStop() { m_timeract = false; }
	void Reset();
private:
	CCounter m_score;
	CCounter m_time;
	int m_piece_placed;
	int m_attack_lines;
	int m_clear_lines;
	float m_pps;
	float m_apm;
	CNumTexSource m_num_score;
	int m_tspin_lines;
	bool m_timeract;

	float CalcPiecesPerSeconds();
	float CalcAttackPerMinutes();
};