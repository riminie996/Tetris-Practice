#pragma once
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Function.h"
#include "CAnime3.h"
#include "CNumTexSource.h"

namespace Tetris
{
	namespace ClearText
	{
		const Point DISPLAYED_REN_POS = { 128,664 };
		const Point DISPLAYED_REN_NUMBER_POS = { 88,640 };
		const Point DISPLAYED_LINES_POS = { 128,512 };
		const Point DISPLAYED_TSPIN_POS = { 128,448 };
		const Point DISPLAYED_BTB_POS = { 128,576 };
		const Point DISPLAYED_PERFECT_POS = { 0,0 };

		//テクスチャ切り取り位置
		const RECT_F RECT_REN = { 0.0f,0.0f,124.0f,64.0f };
		const RECT_F RECT_LINES[4] = 
		{
			{ 0.0f,256.0f, 168.0f,64.0f },
			{ 0.0f,320.0f, 198.0f,64.0f },
			{ 0.0f,384.0f, 162.0f,64.0f },
			{0.0f,64.0f, 200.0f,64.0f} ,
		};
		const RECT_F RECT_TSPIN = { 0.0f,128.0f, 174.0f,64.0f };
		const RECT_F RECT_TSPINMINI = { 0.0f,192.0f, 120.0f,64.0f };
		const RECT_F RECT_BTB = { 0.0f,448.0f, 378.0f,64.0f };

		const Point RECT_NUMBER = { 45,64 };
	}
}

class ObjClearText : public CObj
{
public:
	ObjClearText(){}
	~ObjClearText(){}
	void Init();
	void Action();
	void Draw();
	void AnimeStart(int ren, int lines, bool btb, E_TSPIN_PATTERN tspin,bool perfect);
private:
	int m_ren;
	int m_lines;
	E_TSPIN_PATTERN m_tspin_type;

	CAnime3 m_ani_ren;
	CAnime3 m_ani_lines;
	CAnime3 m_ani_tspin;
	CAnime3 m_ani_btb;
	CAnime3 m_ani_perfect;
	CNumTexSource m_num_ren;
};