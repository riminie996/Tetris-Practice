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

	

	void SetGameOverFlag(bool status) { m_gameover = status; }
	void SetGameClearFlag(bool status) { m_gameclear = status; }
	bool GetGameOverFlag() { return m_gameover; }
	bool GetGameClearFlag() { return m_gameclear; }
	bool BlockPosCheck(int x, int y);//そこにすでにブロックがあるか返す
	void SetHoldType(MINO_TYPE type);//Hold
	void LinesCompleteCheck();//ラインがそろったかチェック
	Tetris::ST_PRACTICE_OPTION* GetOptions() {return &m_practice_options; }
	bool GetIsPause() { return m_pause_flag; }


	//両壁と床の分
	MapObjects m_field[FIELD_HEIGHT][FIELD_WIDTH];

	float field_x;
	float field_y;

	void FieldDraw(int offset_x,int offset_y,MapObjects field[FIELD_HEIGHT][FIELD_WIDTH]);


	void GarbageRising(int height);
	void GarbageRising(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage);
	void AddGarbageLines(int height);
	void AddGarbageLines(Tetris::RisingGarbage::ST_FIELD_GARBAGE garbage);

	void FieldUpdate();//ミノが置かれたときに入る処理
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
	bool m_gameclear;
	int m_bag_round_count;

	MINO_TYPE m_hold_type;
	bool m_hold_flag;

	int m_ren;
	bool m_btb;

	//ランダム関連
	int m_random_random_seed;
	std::mt19937 m_random_random_engine;

	//OPTION
	Tetris::ST_PRACTICE_OPTION m_practice_options;

	bool m_common_key_flag;
	bool m_pause_flag;
	int m_mino_count;
	int m_rising_lines;//のちのち消える
	int m_rising_remain;//のちのち消える
	std::list<Tetris::RisingGarbage::ST_FIELD_GARBAGE> m_list_garbage;


	void InitField();//ブロック配置
	MINO_TYPE BagToType();//バッグからミノを受け取る
	void Bag_All_Check();//バッグからすべてのミノを取り出したかチェック
	void DrawFontOption(float x, float y,float font_size);

	void Restart(); 
	void LineClear(int line_y);
	void BlockDraw(int screen_pos_x, int screen_pos_y, int type);
	void RisingLinesBlockDraw(/*int line*/);

	void AttackGarbage(int lines);
	int CalcScore(int lines,int ren,E_TSPIN_PATTERN tspin,bool btb,bool perfect);//加算するスコアを計算
	void MinoCreate(MINO_TYPE type);//ミノを作成
	void MinoCreateFromNext();//ネクストを作成
	void NextCreate();//ネクストを作成
	void NextCreate(MINO_TYPE type);//ネクストを作成(固定用)
};

