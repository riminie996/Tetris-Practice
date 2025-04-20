//使用するヘッダーファイル
#include "CCounter.h"
#include <math.h>

//使用するネームスペース

CCounter::CCounter(float start, float min, float max, int mode)
{
	StartValue = start;
	NowValue = start;
	if (min <= max)
	{
		MinValue = min;
		MaxValue = max;
	}
	else 
	{
		MinValue = max;
		MaxValue = min;
	}
	
	Mode = mode;
	round_trip_flag = false;
	used_flag = true;
}

void CCounter::Add(float add)
{
	if (used_flag == false)return;

	if (round_trip_flag == false)
		NowValue += add;
	else if (round_trip_flag == true)
		NowValue -= add;

	Adjust();
}
void CCounter::Sub(float sub)
{
	if (used_flag == false)return;

	if (round_trip_flag == false)
		NowValue -= sub;
	else if (round_trip_flag == true)
		NowValue += sub;

	Adjust();
}

void CCounter::Adjust()
{
	if (Mode == FREE)return;
	if (used_flag == false)return;

	//最小値、最大値を超えないように止める
	else if (Mode == STOP)
	{
		if (NowValue <= MinValue)
			NowValue = MinValue;
		if (MaxValue <= NowValue)
			NowValue = MaxValue;
	}
	//現在の値が最大値を超えた場合、超えた分を現在の値にする
	else if (Mode == LOOP)
	{
		if (NowValue <= MinValue)
		{
			float over = MinValue - NowValue;
			NowValue = MaxValue - over;
		}
		if (MaxValue <= NowValue)
		{
			float over = NowValue - MaxValue;
			NowValue = MinValue + over;
		}
	}
	else if (Mode == ROUND_TRIP)
	{
		//
		if (round_trip_flag == false)
		{
			if (MaxValue < NowValue)
			{
				float over = NowValue - MaxValue;
				NowValue = MaxValue - over;
				round_trip_flag = true;
			}

		}
		else// if (round_trip_flag == true)
		{
			if (NowValue < MinValue)
			{
				float over = NowValue - MinValue;
				NowValue = MinValue + over;
				round_trip_flag = false;
			}
		}
	}
}
bool CCounter::GetMinReached()
{
	if (used_flag == false)return false;

	if (MinValue >= NowValue)
		return true;
	return false;
}
bool CCounter::GetMaxReached()
{
	if (used_flag == false)return false;

	if (MaxValue <= NowValue)
		return true;
	return false;
}
bool CCounter::GetUse()
{
	return used_flag;
}
