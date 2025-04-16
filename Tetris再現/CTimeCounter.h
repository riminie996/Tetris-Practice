#pragma once
//使用するヘッダー
#include <time.h>

//使用するネームスペース

// クラス CTimeCounterの使い方---------------------------
// 1.CTimeCounter型のオブジェクトを作成します
// クラス内で宣言するなら 
// CTimeCounter オブジェクト名 { CTimeCounter(開始値,終了値,ms間隔値) };
// 2.各クラスのAction関数などでオブジェクトからAction関数かActionLoop関数を呼び出す。
// 例：オブジェクト名.Action();
// 3.ハンバーグの出来上がり
class CTimeCounter
{
public:
	CTimeCounter();
	CTimeCounter(int start, int end, int msInterval);
	~CTimeCounter() { };
	void Start(int start, int end, int msInterval);//スタート
	void Stop();
	void Action();//アクション
	void ActionLoop();//アクション

	bool GetProgress() { return isProgress; }
	bool GetStoped() { return isStoped; }
	bool GetEndValueReached();
	int NowValue, EndValue;

private:
	clock_t ckStart, ckEnd, ckNow, ckLast;
	bool isStoped;//停止中かどうか
	bool isProgress;//進行中かどうか
	bool isUnused;//未使用
	int StartValue, msInterval_Value;
};