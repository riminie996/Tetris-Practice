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
/// ������`�悷�郁�\�b�h�B
/// </summary>
/// <param name="numstr">������ɕϊ��������l���</param>
/// <param name="x">�ʒuX</param>
/// <param name="y">�ʒuY</param>
/// <param name="width">�����̕�</param>
void CNumTexSource::Draw(string numstr, int x, int y, int width)
{
	if (m_set_tex == false)return;

	int pos = numstr.find(".");
	string  digit_str;

	if (pos != 1)
	{
		//�Œጅ���ɓ��B���Ă��Ȃ��ꍇ�A�擪��0��}��
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
		//�Œጅ���ɓ��B���Ă��Ȃ��ꍇ�A�擪��0��}��
		if (pos - 1/*�����_�l��*/ < min_digit)
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
		//1�����������o���Đ��l�ɕϊ�����B
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


//�e�����𒆐S��ŕ`�悷�邾���ŁA
//�^�񒆊񂹂ȂǂɂȂ邱�Ƃ͂Ȃ��B
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