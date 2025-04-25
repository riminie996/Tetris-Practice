#pragma once
#include "CCounter.h"
#include "GameHead.h"
#include "ObjBlock.h"
#include "Tetris.h"
#include "ObjPlayerControll.h"
//GameL�̃w�b�_�[
#include "GameL\SceneObjManager.h"

using namespace GameL;
using namespace Tetris;



class ObjMino : public CObj
{
public:
	ObjMino(MINO_TYPE minoType);
	~ObjMino() {};
	void Init();
	void Action();
	void Draw();
	MINO_TYPE GetMinoType() { return m_mino_type; }
	E_TSPIN_PATTERN GetTSpinFlag() {return m_tspin_pattern;}
private:
	bool m_mino_first_action;

	//�~�m�̍��W
	int m_px;
	int m_py;
	//�~�m�̊e�u���b�N�̍��W
	int m_mino_x[MINO_BLOCK_AMOUNT];
	int m_mino_y[MINO_BLOCK_AMOUNT];
	//�X�[�p�[���[�e�[�V�����V�X�e���p
	//�p�x���L������
	ROTATE_ANGLE m_rotate_angle;

	CCounter m_ct_landing;//���n���Ă���u�����܂ł̎���


	//���b�N�_�E���֘A
	int m_move_count;
	int m_down_Block_line;
	CCounter m_ct_fall;//�����܂ł̎���
	CCounter m_ct_softdrop;//�����܂ł̎���
	CCounter m_ct_arr;//���ړ��̑��x(��������)


	//���
	MINO_TYPE m_mino_type;
	E_TSPIN_PATTERN m_tspin_pattern;

	void MinoMove(int direction);//�~�m������
	void MinoHardDrop();//�N�C�b�N�h���b�v

	void RotateMino(int direction);//�~�m��]
	void RotationMino(int* minoPosX, int* minoPosY, float rotation,float axis);//�~�m��]����
	bool SuperRotationSystem(int* minoPosX, int* minoPosY, int rotate_dir, int mino_type);//
	E_TSPIN_PATTERN TSpinCheck(int blockPosX, int blockPosY, ROTATE_ANGLE angle);

	//�~�m�����n���Ă��邩�ǂ������f����
	bool GetMinoBlockFixed();
	bool MinoOverlapCheck(int x, int y);//�����Ƀu���b�N�����łɂ��邩�`�F�b�N
	void SetFieldMino();//�u���b�N�Ƀ~�m��z�u
	void GhostDraw();//�S�[�X�g��`��

	void SpinSoundPlay(bool spin_result);

	DIRECTION GetRotateKey(E_PLAYER_CONTROLL ctrl);
	
};