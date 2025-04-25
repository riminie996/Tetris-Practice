#pragma once
#include "CCounter.h"
#include "GameHead.h"
#include "ObjBlock.h"
#include "Tetris.h"
#include "ObjPlayerControll.h"
//GameLのヘッダー
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

	//ミノの座標
	int m_px;
	int m_py;
	//ミノの各ブロックの座標
	int m_mino_x[MINO_BLOCK_AMOUNT];
	int m_mino_y[MINO_BLOCK_AMOUNT];
	//スーパーローテーションシステム用
	//角度を記憶する
	ROTATE_ANGLE m_rotate_angle;

	CCounter m_ct_landing;//着地してから置かれるまでの時間


	//ロックダウン関連
	int m_move_count;
	int m_down_Block_line;
	CCounter m_ct_fall;//落下までの時間
	CCounter m_ct_softdrop;//落下までの時間
	CCounter m_ct_arr;//横移動の速度(おしっぱ)
	CCounter m_ct_next_delay;//横移動の速度(おしっぱ)

	//種類
	MINO_TYPE m_mino_type;
	E_TSPIN_PATTERN m_tspin_pattern;

	void MinoMove(int direction);//ミノ動かす
	void MinoHardDrop();//クイックドロップ

	void RotateMino(int direction);//ミノ回転
	void RotationMino(int* minoPosX, int* minoPosY, float rotation,float axis);//ミノ回転処理
	bool SuperRotationSystem(int* minoPosX, int* minoPosY, int rotate_dir, int mino_type);//
	E_TSPIN_PATTERN TSpinCheck(int blockPosX, int blockPosY, ROTATE_ANGLE angle);

	//ミノが着地しているかどうか判断する
	bool GetMinoBlockFixed();
	bool MinoOverlapCheck(int x, int y);//そこにブロックがすでにあるかチェック
	void SetFieldMino();//ブロックにミノを配置
	void GhostDraw();//ゴーストを描画

	void SpinSoundPlay(bool spin_result);

	DIRECTION GetRotateKey(E_PLAYER_CONTROLL ctrl);
	
};