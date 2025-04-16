#pragma once

//GameL�̃w�b�_�[
#include "GameL\SceneObjManager.h"
#include "GameHead.h"
#include "Tetris.h"

#include <random>
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

	
	void MinoCreate(MINO_TYPE type);//�~�m���쐬
	void NextCreate();//�l�N�X�g���쐬
	void SetGameOverFlag(bool status) { m_gameover = status; }
	bool GetGameOverFlag() { return m_gameover; }
	bool BlockPosCheck(int x, int y);//�����ɂ��łɃu���b�N�����邩�Ԃ�
	void SetHoldType(MINO_TYPE type);//Hold
	void LinesCompleteCheck();//���C��������������`�F�b�N
	bool GetOptions(E_PRACTICE_OPTION option_num) {return m_practice_options[option_num]; }
	bool GetIsPause() { return m_pause_flag; }


	//���ǂƏ��̕�
	MapObjects m_field[FIELD_HEIGHT][FIELD_WIDTH];

	float field_x;
	float field_y;

	void FieldDraw(int offset_x,int offset_y,MapObjects field[FIELD_HEIGHT][FIELD_WIDTH]);


	void GarbageRising(int height);
	void AddGarbageLines(int height);

	void FieldUpdate();
private:
	bool m_first_mino;//�Q�[���I�[�o�[������s���K�v�����邩
	MINO_TYPE m_next[NEXT_AMOUNT];

	//�o�b�O�V�X�e��
	//���������~�m�̎�ނ��΂�Ȃ��悤�ɁA
	//7��ނ�1�Z�b�g�ɂ���B
	//7��ޑS�Đ������ꂽ��A���̃o�b�O�Ɉڂ�B
	//����̎d�l�́A7��ނ��ׂĂ�false�ɂȂ�����S��true�ɂ���
	bool m_bag[MINO_MAX_TYPE];
	bool m_lines_complete;
	bool m_gameover;
	int m_bag_round_count;

	MINO_TYPE m_hold_type;
	bool m_hold_flag;

	int m_ren;
	bool m_btb;

	//�����_���֘A
	int m_random_seed;
	int m_random_random_seed;
	std::mt19937 m_random_engine;
	std::mt19937 m_random_engine_copy;
	std::mt19937 m_random_random_engine;

	//OPTION
	bool m_practice_options[E_PRACTICE_OPTION::Option_Count];
	int m_rising_timer_sec;

	bool m_common_key_flag;
	bool m_pause_flag;
	int m_now_selected_option;
	int m_mino_count;
	MINO_TYPE m_fixed_mino[MINO_MAX_TYPE];
	int m_rising_lines;
	int m_rising_remain;

	void InitField();//�u���b�N�z�u
	MINO_TYPE BagToType();//�o�b�O����~�m���󂯎��
	void Bag_All_Check();//�o�b�O���炷�ׂẴ~�m�����o�������`�F�b�N
	void Next_Mino_Draw(float x,float y,int type);
	void DrawFontOption(float x, float y,float font_size);

	void Restart(); 
	void DrawPauseModeScreen();
	void LineClear(int line_y);
	void BlockDraw(int screen_pos_x, int screen_pos_y, int type);
	void RisingLinesBlockDraw(int line);
	int CalcSendAttackLines(int clear_line,int ren,bool btb,E_TSPIN_PATTERN tspin,bool perfect);
	void AttackGarbage(int lines);
};

