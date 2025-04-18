#include "ObjScore.h"
#include "GameL/DrawFont.h"
#include "Tetris.h"

using namespace Tetris;
//初期化
void ObjScore::Init()
{
	m_score = { CCounter(0.0f,0.0f,9999999.0f,STOP) };
}

//進行
void ObjScore::Action()
{

}

//描画
void ObjScore::Draw()
{
	//白
	float c[4] = { 1.0f,1.0f,1.0f,1.0f };
	wchar_t score[8];
	Font::StrDraw(L"SCORE", SCORE_TEXT_OFFSET_X, SCORE_TEXT_OFFSET_Y, SCORE_FONT_SIZE, c);
	swprintf_s(score, L"%07.0f", m_score.NowValue);
	Font::StrDraw(score, SCORE_TEXT_OFFSET_X, SCORE_TEXT_OFFSET_Y + SCORE_FONT_SIZE, SCORE_FONT_SIZE, c);
}