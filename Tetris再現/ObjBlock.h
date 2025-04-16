#pragma once

//GameLのヘッダー
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

	
	void MinoCreate(MINO_TYPE type);//ミノを作成
	void NextCreate();//ネクストを作成
	void SetGameOverFlag(bool status) { m_gameover = status; }
	bool GetGameOverFlag() { return m_gameover; }
	bool BlockPosCheck(int x, int y);//そこにすでにブロックがあるか返す
	void SetHoldType(MINO_TYPE type);//Hold
	void LinesCompleteCheck();//ラインがそろったかチェック
	bool GetOptions(E_PRACTICE_OPTION option_num) {return m_practice_options[option_num]; }
	bool GetIsPause() { return m_pause_flag; }


	//両壁と床の分
	MapObjects m_field[FIELD_HEIGHT][FIELD_WIDTH];

	float field_x;
	float field_y;

	void FieldDraw(int offset_x,int offset_y,MapObjects field[FIELD_HEIGHT][FIELD_WIDTH]);


	void GarbageRising(int height);
	void AddGarbageLines(int height);

	void FieldUpdate();
private:
	bool m_first_mino;//ゲームオーバー判定を行う必要があるか
	MINO_TYPE m_next[NEXT_AMOUNT];

	//バッグシステム
	//生成されるミノの種類が偏らないように、
	//7種類で1セットにする。
	//7種類全て生成されたら、次のバッグに移る。
	//今回の仕様は、7種類すべてがfalseになったら全てtrueにする
	bool m_bag[MINO_MAX_TYPE];
	bool m_lines_complete;
	bool m_gameover;
	int m_bag_round_count;

	MINO_TYPE m_hold_type;
	bool m_hold_flag;

	int m_ren;
	bool m_btb;

	//ランダム関連
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

	void InitField();//ブロック配置
	MINO_TYPE BagToType();//バッグからミノを受け取る
	void Bag_All_Check();//バッグからすべてのミノを取り出したかチェック
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

