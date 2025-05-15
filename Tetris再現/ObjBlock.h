#pragma once

//GameL�̃w�b�_�[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

#include <random>
#include "ObjPracticeOrder.h"
using namespace Tetris;
using namespace GameL;



class ObjBlock : public CObj
{
public:
	ObjBlock() ;
	~ObjBlock() {};
	void Init();
	void Action();
	void Draw();

	void AddMinoCount() { m_mino_count++; }
	void SetBlock(int x,int y, MapObjects type);
	bool GetHoldFlag() { return m_hold_flag; }
	void SetHoldFlag(bool status);

	

	void SetGameOverFlag(bool status) { m_gameover = status; }
	void SetGameClearFlag(bool status) { m_gameclear = status; }
	bool GetGameOverFlag() { return m_gameover; }
	bool GetGameClearFlag() { return m_gameclear; }
	bool BlockPosCheck(int x, int y);//�����ɂ��łɃu���b�N�����邩�Ԃ�
	void SetHoldType(MINO_TYPE type);//Hold
	void LinesCompleteCheck();//���C��������������`�F�b�N
	Tetris::ST_PRACTICE_OPTION* GetOptions() {return &m_practice_options; }
	bool GetIsPause() { return m_pause_flag; }


	//���ǂƏ��̕�
	MapObjects m_field[FIELD_HEIGHT][FIELD_WIDTH];

	float field_x;
	float field_y;

	void FieldDraw(int offset_x,int offset_y,MapObjects field[FIELD_HEIGHT][FIELD_WIDTH]);


	void GarbageRising(int height);
	void GarbageRising(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage);
	void AddGarbageLines(int height);
	void AddGarbageLines(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage);

	void FieldUpdate();//�~�m���u���ꂽ�Ƃ��ɓ��鏈��
private:
	bool m_first_mino;//�Q�[���I�[�o�[������s���K�v�����邩
	MINO_TYPE m_next[NEXT_AMOUNT];

	//�o�b�O�V�X�e��
	//���������~�m�̎�ނ��΂�Ȃ��悤�ɁA
	//7��ނ�1�Z�b�g�ɂ���B
	//7��ޑS�Đ������ꂽ��A���̃o�b�O�Ɉڂ�B
	//����̎d�l�́A7��ނ��ׂĂ�false�ɂȂ�����S��true�ɂ���
	bool m_bag[MINO_MAX_TYPE];
	bool m_bag_system;
	bool m_lines_complete;
	bool m_gameover;
	bool m_gameclear;
	int m_bag_round_count;

	MINO_TYPE m_hold_type;
	bool m_hold_flag;

	int m_ren;
	bool m_btb;

	//�����_���֘A
	int m_random_random_seed;
	std::mt19937 m_random_random_engine;

	//OPTION
	Tetris::ST_PRACTICE_OPTION m_practice_options;

	bool m_common_key_flag;
	bool m_pause_flag;
	int m_mino_count;
	std::list<Tetris::RisingGarbage::ST_FIELD_GARBAGE> m_list_garbage;

	ObjPracticeOrder m_practice_order;

	void InitField();//�u���b�N�z�u
	MINO_TYPE BagToType();//�o�b�O����~�m���󂯎��
	void Bag_All_Check();//�o�b�O���炷�ׂẴ~�m�����o�������`�F�b�N
	void DrawFontOption(float x, float y,float font_size);
	void Restart();


	void LineClear(int line_y);
	void BlockDraw(int screen_pos_x, int screen_pos_y, int type);
	void RisingLinesBlockDraw(/*int line*/);

	void AttackGarbage(int lines);
	int CalcScore(int lines,int ren,E_TSPIN_PATTERN tspin,bool btb,bool perfect);//���Z����X�R�A���v�Z
	void MinoCreate(MINO_TYPE type);//�~�m���쐬
	void MinoCreateFromNext();//�l�N�X�g���쐬
	void NextCreate();//�l�N�X�g���쐬
	void NextCreate(MINO_TYPE type);//�l�N�X�g���쐬(�Œ�p)
	void NextClear();
};

