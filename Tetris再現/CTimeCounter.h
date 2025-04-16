#pragma once
//�g�p����w�b�_�[
#include <time.h>

//�g�p����l�[���X�y�[�X

// �N���X CTimeCounter�̎g����---------------------------
// 1.CTimeCounter�^�̃I�u�W�F�N�g���쐬���܂�
// �N���X���Ő錾����Ȃ� 
// CTimeCounter �I�u�W�F�N�g�� { CTimeCounter(�J�n�l,�I���l,ms�Ԋu�l) };
// 2.�e�N���X��Action�֐��ȂǂŃI�u�W�F�N�g����Action�֐���ActionLoop�֐����Ăяo���B
// ��F�I�u�W�F�N�g��.Action();
// 3.�n���o�[�O�̏o���オ��
class CTimeCounter
{
public:
	CTimeCounter();
	CTimeCounter(int start, int end, int msInterval);
	~CTimeCounter() { };
	void Start(int start, int end, int msInterval);//�X�^�[�g
	void Stop();
	void Action();//�A�N�V����
	void ActionLoop();//�A�N�V����

	bool GetProgress() { return isProgress; }
	bool GetStoped() { return isStoped; }
	bool GetEndValueReached();
	int NowValue, EndValue;

private:
	clock_t ckStart, ckEnd, ckNow, ckLast;
	bool isStoped;//��~�����ǂ���
	bool isProgress;//�i�s�����ǂ���
	bool isUnused;//���g�p
	int StartValue, msInterval_Value;
};