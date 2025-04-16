//使用するヘッダーファイル
#include "CCounter.h"
#include <math.h>

//使用するネームスペース

CCounter::CCounter(float start, float min, float max,int mode)
{
	StartValue = start;
	NowValue = start;
	MinValue = min;
	MaxValue = max;
	Mode = mode;
}

void CCounter::Add(float add)
{
	NowValue += add;

	if(Mode != FREE)
	Adjust();
}
void CCounter::Sub(float sub)
{
	NowValue -= sub;

	if(Mode != FREE)
	Adjust();
}

void CCounter::Adjust()
{
	if (Mode == STOP)
	{
		if (NowValue < MinValue)
			NowValue = MinValue;
		if (MaxValue < NowValue)
			NowValue = MaxValue;
	}
	else if (Mode == LOOP)
	{
		// 3~13の場合　1になったとき 11にしたい
		if (NowValue < MinValue)
		{
			float num = MinValue - NowValue;
			NowValue = MaxValue - num;
		}
		if (MaxValue < NowValue)
		{
			float num = NowValue - MaxValue;
			NowValue = MinValue + num;
		}
	}
}
bool CCounter::GetMinReached()
{
	if (MinValue == NowValue)
		return true;
	return false;
}
bool CCounter::GetMaxReached()
{
	if (MaxValue == NowValue)
		return true;
	return false;
}