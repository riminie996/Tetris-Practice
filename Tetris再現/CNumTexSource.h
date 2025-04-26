#pragma once
#include "GameL\DrawTexture.h"
#include <string>
using namespace GameL;
using std::string;

//数字切り取り位置用クラス
//数字を等間隔で横に並べた画像を用意する。
class CNumTexSource
{
public:
	CNumTexSource() {
		m_set_tex = false;
		min_digit = 0;
		decimal_digit = 0;
		m_comma_flag = false;
	};
	~CNumTexSource() {};
	
	int min_digit;
	int decimal_digit;
	void SetTex(int tex, int width, int height,int offset_x=0,int offset_y = 0);
	void SetCommaRect(RECT_F rectangle,int decimal);
	void Draw(string numstr, int x, int y,int width);
	void CenterDraw(string numstr, int x, int y,int width);
	void Draw(float val,int x,int y,int width);
	void Draw(int val,int x,int y,int width);
	void CenterDraw(int val,int x,int y,int width);
	void LeftCenterDraw(int val,int x,int y,int width);
private:
	RECT_F m_num_src[10];
	RECT_F m_comma;
	int m_tex;
	bool m_set_tex;
	bool m_comma_flag;

};