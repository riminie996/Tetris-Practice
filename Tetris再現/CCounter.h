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
	CCounter() { used_flag = false; };
	CCounter(float start, float min, float max, int mode);
	~CCounter() { };
	void Add(float add);
	void Sub(float sub);
	void Reset() { NowValue = StartValue; }
	float MinValue;			//�ŏ��̒l
	float MaxValue;			//�ő�̒l
	float NowValue;			//���݂̒l
	bool GetMinReached();
	bool GetMaxReached();
	bool GetUse();

private:
	float StartValue;		//�J�n�l
	int Mode;				//���[�h
	bool round_trip_flag;	//�t���t���O(�����p)
	bool used_flag;
	void Adjust();
};