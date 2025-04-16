#pragma once

//�i�r���@�����Ă킩��͂��߂ăQ�[���v���O���~���O���
//�֗��ȃJ�E���^�[���������̂ŎQ�l�ɂ����Ă��������̗p

enum ACTION_MODE
{
	FREE,
	STOP,		//�X�g�b�v
	LOOP,		//���[�v
	ROUND_TRIP,	//����
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