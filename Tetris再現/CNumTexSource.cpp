#include "CNumTexSource.h"
using std::string;
void CNumTexSource::SetTex(int tex, int width, int height,int offset_x,int offset_y)
{
	m_set_tex = true;
	m_tex = tex;

	for (int i = 0; i < 10; i++)
	{
		m_num_src[i] = { (float)offset_x + (float)width * i,(float)offset_y,(float)width,(float)height };
	}
}

void CNumTexSource::SetCommaRect(RECT_F rectangle,int decimal) 
{ 
	m_comma_flag = true;
	m_comma = rectangle;
	decimal_digit = decimal;
}
/// <summary>
/// 数字を描画するメソッド。
/// </summary>
/// <param name="numstr">文字列に変換した数値情報</param>
/// <param name="x">位置X</param>
/// <param name="y">位置Y</param>
/// <param name="width">文字の幅</param>
void CNumTexSource::Draw(string numstr, int x, int y, int width)
{
	if (m_set_tex == false)return;

	int pos = numstr.find(".");
	string  digit_str;

	if (pos != 1)
	{
		//最低桁数に到達していない場合、先頭に0を挿入
		if (numstr.length() < min_digit)
		{
			for (int i = 0; i < min_digit - numstr.length();)
			{
				numstr.insert(numstr.begin(), '0');
			}
		}
	}
	else
	{
	//	digit_str = numstr.substr(0, pos);
		//最低桁数に到達していない場合、先頭に0を挿入
		if (pos - 1/*小数点考慮*/ < min_digit)
		{
			for (int i = 0; i < min_digit - numstr.length();)
			{
				numstr.insert(numstr.begin(), '0');
			}
		}
	}


	if (m_comma_flag)
	{
		if (pos != -1)
		{
			digit_str = numstr.substr(0, pos);
			for (int i = 0; i < decimal_digit + 1; i++)
			{
				digit_str.insert(digit_str.end(), numstr[pos + i]);
			}
			numstr = digit_str;
		}

	}
	else
	{
		numstr = numstr.substr(0,pos);
	}
	x -= (numstr.length() - 1) * width;

	for (int i = 0; i < numstr.length(); i++)
	{
		//1文字だけ取り出して数値に変換する。
		string num = numstr.substr(i, 1);

		if (num == ".")
		{
			Draw::Draw(m_tex, x, y, m_comma);

		}
		else
		{
			Draw::Draw(m_tex, x, y, m_num_src[atoi(num.c_str())]);
		}

		x += width;
	}
}
void CNumTexSource::Draw(float val, int x, int y, int width)
{
	Draw(std::to_string(val), x, y, width);
}


//各数字を中心基準で描画するだけで、
//真ん中寄せなどになることはない。
void CNumTexSource::CenterDraw(string numstr, int x, int y, int width)
{
	Draw(numstr, x - width / 2, y - m_num_src[0].m_height / 2, width);
}
void CNumTexSource::CenterDraw(int val, int x, int y, int width)
{
	Draw(std::to_string(val), x - width / 2, y - m_num_src[0].m_height / 2, width);
}
void CNumTexSource::LeftCenterDraw(int val, int x, int y, int width)
{
	Draw(std::to_string(val), x , y - m_num_src[0].m_height / 2, width);
}