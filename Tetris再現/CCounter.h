#pragma once

//ナビつき　つくってわかるはじめてゲームプログラミングより
//便利なカウンターがあったので参考にさせていただき採用

enum ACTION_MODE
{
	FREE,
	STOP,		//ストップ
	LOOP,		//ループ
	ROUND_TRIP,	//往復
};

class CCounter
{
public:
	CCounter() { used_flag = false; };
	CCounter(float start, float min, float max, int mode);
	~CCounter() { };
	void Add(float add);
	void Sub(float sub);
	void Reset() { NowValue = StartValue; }
	float MinValue;			//最少の値
	float MaxValue;			//最大の値
	float NowValue;			//現在の値
	bool GetMinReached();
	bool GetMaxReached();
	bool GetUse();

private:
	float StartValue;		//開始値
	int Mode;				//モード
	bool round_trip_flag;	//逆走フラグ(往復用)
	bool used_flag;
	void Adjust();
};