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
	CCounter() {};
	CCounter(float start, float min, float max,int mode);
	~CCounter() { };
	void Add(float add);
	void Sub(float sub);
	void Reset() {NowValue = StartValue;}
	float NowValue;
	bool GetMinReached();
	bool GetMaxReached();
	float MinValue;
	float MaxValue;
private:
	float StartValue;
	int Mode;
	void Adjust();
};