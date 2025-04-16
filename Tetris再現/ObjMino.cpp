#include "ObjMino.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameHead.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "Function.h"
#include "GameL\Audio.h"

ObjMino::ObjMino(MINO_TYPE minoType)
{
	m_mino_type = minoType; 
};
//初期化
void ObjMino::Init()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	int count = 0;
	for (int i = 0; i < MINO_DATABASE_COL; i++)
	{
		for (int j = 0; j < MINO_DATABASE_LOW; j++)
		{
		 	if (count == MINO_BLOCK_AMOUNT)break;
			//0より大きい場合、座標の情報に代入する
			if (MINO_SHAPE[m_mino_type][i][j] == 1)
			{
				m_mino_x[count] = j;
				m_mino_y[count] = i;
				count++;
			}
		}
	}
	//ミノの初期位置
	m_px = MINO_INIT_POS_X;
	m_py = MINO_INIT_POS_Y;
	m_rotate_angle = RotateAngle0;

	m_key_flag[0] = false;
	m_key_flag[1] = false;
	m_key_flag[2] = false;

	//1秒
	m_ct_input_move = { CCounter(0.0f,0.0f, FRAME_MINO_INPUT_MOVE, STOP) };
	m_ct_landing = { CCounter(0.0f,0.0f, FRAME_MINO_LANDING, STOP) };
	m_ct_fall = { CCounter(0.0f,0.0f, FRAME_MINO_FALL, STOP) };

	//ロックダウン関連
	m_down_Block_line = 0;
	m_move_count = 0;

	m_mino_first_action = true;
	m_tspin_pattern = E_TSPIN_PATTERN::NoTSpin;

	if (oBlock != nullptr)
	{

		//生成できない場合、1ブロックあげてみる
		for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
		{
			int bx = m_px + m_mino_x[i];
			int by = m_py + m_mino_y[i];
			if (oBlock->BlockPosCheck(bx, by))
			{
				m_py -= 1;
				break;
			}
		}
	}
}

//進行
void ObjMino::Action()
{ 

	//ブロックの情報を取得
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);
	if (m_mino_first_action)
	{

		for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
		{
			if (MinoOverlapCheck(m_px + m_mino_x[i], m_py + m_mino_y[i]) == true)
			{

				oBlock->SetGameOverFlag(true);
				SetFieldMino();
				break;
			}
		}

		m_mino_first_action = false;
	}

	if (oBlock->GetIsPause() == true)return;

	//CCounter処理
	m_ct_landing.Add(1.0f);
	m_ct_input_move.Add(1.0f);
	m_ct_fall.Add(1.0f);

		//移動
	if (Input::GetVKey('A') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_LEFT))
	{
		if (m_key_flag[0] == true)
		{
			MinoMove(Left);
			m_key_flag[0] = false;
		}
	}
	else if (Input::GetVKey('D') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_RIGHT))
	{
		if (m_key_flag[0] == true)
		{
			MinoMove(Right);
			m_key_flag[0] = false;
		}

	}
	else
	{
		m_key_flag[0] = true;
		//左右どちらも押されていない時、Startが実行され続けるため、
		//常に0になる
		m_ct_input_move.Reset();
	}
	//終了値に達した
	if (m_ct_input_move.GetMaxReached() == true)
	{
		m_key_flag[0] = true;
	}
	
	//下がるごとに移動回数がリセット
	if (Input::GetVKey('S') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_DOWN))
	{
		if (GetMinoBlockFixed() == false)
		{
			MinoMove(Down);
			m_move_count = 0;
			oScore->AddScore(FALL_ADD_SCORE);
			m_ct_fall.Reset();
		}
	}
	//通常降下時はスコアを与えさせないため、分ける
	if (m_ct_fall.GetMaxReached() && !oBlock->GetOptions(E_PRACTICE_OPTION::NoNaturalDrop))
	{
		if (GetMinoBlockFixed() == false)
		{
			MinoMove(Down);
			m_move_count = 0;
			m_ct_fall.Reset();
		}
	}

	if (Input::GetVKey('W') || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_UP))
	{
		if (m_key_flag[2] == true)
		{
			//落として配置
			MinoHardDrop();
			SetFieldMino();
			return;
		}
		m_key_flag[2] = false;
	}
	else
	{
		m_key_flag[2] = true;
	}
	
	//左回転
	if (Input::GetVKey(VK_LEFT) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_B))
	{
		if (m_key_flag[1] == true)
		{
			RotateMino(Left);
			m_move_count++;
		}
		m_key_flag[1] = false;
	}
	//右回転
	else if (Input::GetVKey(VK_RIGHT) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_A))
	{
		
		if (m_key_flag[1] == true)
		{
			RotateMino(Right);
			m_move_count++;
		}
		m_key_flag[1] = false;
	}

	else
	{
		m_key_flag[1] = true;
	}

	if ((Input::GetVKey(VK_UP) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_L1) || CConInput::GetConInput(Controller_Input_DualShock4::INPUT_R1)) && oBlock->GetHoldFlag() == true)
	{
		if (m_key_flag[3] == true)
		{
			Audio::Start(se_Mino_Hold);
			oBlock->SetHoldType(m_mino_type);
			oBlock->SetHoldFlag(false);
			this->SetStatus(false);
			return;
		}
		m_key_flag[3] = false;
	}
	else
	{
		m_key_flag[3] = true;
	}
	
	if(GetMinoBlockFixed() == false)
	{
		//着地していない場合、常に0にする
		m_ct_landing.Reset();
	}
	else
	{

		m_ct_fall.Reset();
	}



	//終了値に達した
	if ((m_ct_landing.GetMaxReached()||
		m_move_count >=15 )&& GetMinoBlockFixed() == true && 
		!oBlock->GetOptions(E_PRACTICE_OPTION::NoNaturalDrop))
	{
		SetFieldMino();
		oBlock->SetHoldFlag(true);
		return;
	}
}

//描画
void ObjMino::Draw()
{
	GhostDraw();
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		float bx = (m_px + m_mino_x[i]) * BLOCK_PIXELS + FIELD_1P_POS_X + FIELD_OFFSET_X;
		float by = (m_py + m_mino_y[i]) * BLOCK_PIXELS + FIELD_1P_POS_Y + FIELD_OFFSET_Y;
		RECT_F Color_Block = { 0,m_mino_type * BLOCK_PIXELS,BLOCK_PIXELS,BLOCK_PIXELS };
		Draw::Draw(texBlock, bx,by , Color_Block);
		Draw::Draw(texBlock, bx, by, BLOCK_LIGHT_SRC_POS);
	}
	
	//デバッグ用　T-Spinチェッカー描画
	
	//for (int i = 0; i < MINO_DATABASE_COL; i++)
	//{
	//	for (int j = 0; j < MINO_DATABASE_LOW; j++)
	//	{
	//		float bx = (m_px + j) * BLOCK_PIXELS + FIELD_1P_POS_X + FIELD_OFFSET_X;
	//		float by = (m_py +i) * BLOCK_PIXELS + FIELD_1P_POS_Y + FIELD_OFFSET_Y;
	//		if (Tetris::TSPIN_CHECKER[(int)m_rotate_angle][i][j] == 1 && m_mino_type == Mino_T)
	//		{


	//			RECT_F Color_Block_Frame = { BLOCK_PIXELS,BLOCK_PIXELS * m_mino_type,BLOCK_PIXELS,BLOCK_PIXELS };
	//			Draw::Draw(texBlock, bx, by, Color_Block_Frame);
	//		}

	//	}
	//}
	//	
	
}

void ObjMino::MinoHardDrop()
{

	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);
	while (1)
	{
		for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
		{
			//座標に変換
			int bx = m_px + (m_mino_x[i]);
			int by = m_py + (m_mino_y[i]);
			//ミノ座標の1マス左にブロックがあるか確認する。
			if (MinoOverlapCheck(bx, by + 1) == true)
			{
				return;
			}
		}
		m_py += 1;


		oScore->AddScore(FALL_ADD_SCORE);
	}
}

//処理の流れ
// ミノの座標を仮の座標に代入
// 仮の座標だけを回転とSRSかける
// 回転できるようだったらその関数内で座標の置き換えをする
//
void ObjMino::RotateMino(int direction)
{
	bool result = false;
	//Oミノの場合、回転は不必要
	if (m_mino_type == Mino_O)
	{
		SpinSoundPlay(false);
		return;
	}

	//直接座標を渡すと、そのまま書き換えられてしまうので、
	//回転できるか確かめてから回転させたいので作成
	//座標はその関数内で書き換えられる。
	int new_pos_x[MINO_BLOCK_AMOUNT];
	int new_pos_y[MINO_BLOCK_AMOUNT];
	float r = direction == Left ? -ROTATION_ANGLE : ROTATION_ANGLE;
	if (m_mino_type == Mino_I)
	{
		//新座標のポインタを渡す
		RotationMino(new_pos_x, new_pos_y, r, MINO_ROTATION_AXIS_I);
		result = SuperRotationSystem(new_pos_x, new_pos_y, direction, m_mino_type);
	}
	else
	{
		//新座標のポインタを渡す
		RotationMino(new_pos_x, new_pos_y, r, MINO_ROTATION_AXIS);
		result = SuperRotationSystem(new_pos_x, new_pos_y, direction, m_mino_type);
	}
	SpinSoundPlay(result);

	//回転に成功したら着地時間をリセットする
	if (result)
	{
		m_ct_landing.Reset();
	}
}

//戻り値:回転成功
bool ObjMino::SuperRotationSystem(int* minoPosX, int* minoPosY, int rotate_dir, int mino_type)
{
	int* shift_x;
	int* shift_y;

	//右回転、左回転と動きが違う。
	//また、Iミノはちょっと特殊です。
	//詳しくはここを見てもらえればと思います。
	//
	// https://tetrisch.github.io/main/srs.html
	//

	if (rotate_dir == Left)
	{
		if (m_mino_type == Mino_I)
		{
			shift_x = (int*)SRS_LEFT_I_SHIFT_X[m_rotate_angle];
			shift_y = (int*)SRS_LEFT_I_SHIFT_Y[m_rotate_angle];
		}
		else
		{
			shift_x = (int*)SRS_LEFT_SHIFT_X[m_rotate_angle];
			shift_y = (int*)SRS_LEFT_SHIFT_Y[m_rotate_angle];
		}
	}
	else// if (rotate_dir == Right)
	{
		if (m_mino_type == Mino_I)
		{
			shift_x = (int*)SRS_RIGHT_I_SHIFT_X[m_rotate_angle];
			shift_y = (int*)SRS_RIGHT_I_SHIFT_Y[m_rotate_angle];
		}
		else
		{
			shift_x = (int*)SRS_RIGHT_SHIFT_X[m_rotate_angle];
			shift_y = (int*)SRS_RIGHT_SHIFT_Y[m_rotate_angle];
		}
	}


	for (int i = 0; i < SRS_PATTERN; i++)
	{
		bool isBlock = false;
		for (int j = 0; j < MINO_BLOCK_AMOUNT; j++)
		{
			//座標に変換
			//全体的な座標 + 回転後の座標 + ずらした座標 で計算する。
			int bx = m_px + minoPosX[j] + shift_x[i];
			int by = m_py + minoPosY[j] + shift_y[i];

			//4回ずつ調べ、trueが返ってきた時点でそこにあるということ。
			//4回falseの場合はそこにないということ
			if (MinoOverlapCheck(bx, by) == true)
			{
				isBlock = true;
				break;
			}
		}

		if (isBlock == false)
		{
			//ミノの座標に代入すると、回転軸がずれる可能性があるため
			//全体的な座標に加算する
			m_px += shift_x[i];
			m_py += shift_y[i];
			for (int j = 0; j < MINO_BLOCK_AMOUNT; j++)
			{
				m_mino_x[j] = minoPosX[j];
				m_mino_y[j] = minoPosY[j];
			}


			//新しい回転角を記憶させる
			if (rotate_dir == Left)
			{
				if (m_rotate_angle == RotateAngle270)
					m_rotate_angle = RotateAngle180;
				else if (m_rotate_angle == RotateAngle180)
					m_rotate_angle = RotateAngle90;
				else if (m_rotate_angle == RotateAngle90)
					m_rotate_angle = RotateAngle0;
				else //if (m_rotate_angle == RotateAngle0)
					m_rotate_angle = RotateAngle270;
			}
			else if (rotate_dir == Right)
			{
				if (m_rotate_angle == RotateAngle0)
					m_rotate_angle = RotateAngle90;
				else if (m_rotate_angle == RotateAngle90)
					m_rotate_angle = RotateAngle180;
				else if (m_rotate_angle == RotateAngle180)
					m_rotate_angle = RotateAngle270;
				else //if (m_rotate_angle == RotateAngle270)
					m_rotate_angle = RotateAngle0;
			}

			if (m_mino_type == Mino_T)
				m_tspin_pattern = TSpinCheck(m_px, m_py, m_rotate_angle);

			return true;
		}
	}
	return false;
}

//その位置にすでにブロックがあるかどうか確認する。
//存在した場合、trueを返す。
bool ObjMino::MinoOverlapCheck(int x, int y)
{
	//ブロックの情報を取得
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	return oBlock->BlockPosCheck(x,y);
}

//ミノが着地しているかどうか返す
bool ObjMino::GetMinoBlockFixed()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		//座標に変換
		int bx = m_px + (m_mino_x[i]);
		int by = m_py + (m_mino_y[i]);
		//ミノ座標の1マス下にブロックがあるか確認する。
		if (oBlock->m_field[by + 1][bx] != BlockEmpty || by + 1 >= FIELD_HEIGHT)
		{
			//あった
			return true;
		}
	}
	return false;
}

//新座標のポインタを受け取り、値を編集して返す
void ObjMino::RotationMino(int* minoPosX, int* minoPosY, float rotation, float axis)
{

	//axis = 回転軸
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		//差からベクトルを求める
		float rvx = axis - m_mino_x[i];
		float rvy = axis - m_mino_y[i];
		//2点間距離を求める
		float l = DistanceF(m_mino_x[i], m_mino_y[i], axis, axis);
		//ベクトルから角度を求める
		float r = atan2(rvy, rvx);
		r = r * 180.0f / 3.14f;

		//仮の位置の変数を宣言し、調整してから座標に代入する
		float pos_x = axis;
		float pos_y = axis;

		r -= rotation;


		if (r < 0.0f)
			r = abs(r);
		else
			r = 360.0f - abs(r);

		//角度を変換させてからもともとの距離の分戻す
		pos_x += cos(3.14 / 180 * r) * l;
		pos_y += -sin(3.14 / 180 * r) * l;

		//そのまま変換すると小数点以下が切り捨てられるため、
		//調整してから代入する
		minoPosX[i] = RoundingFirstDecimal(pos_x);
		minoPosY[i] = RoundingFirstDecimal(pos_y);

	}

}

void ObjMino::SetFieldMino()
{
	//ブロックを配置
	//ブロックの情報を取得
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	bool gameover = true;
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		oBlock->SetBlock(m_px + m_mino_x[i], m_py + m_mino_y[i], (MapObjects)((int)m_mino_type + 1));
		if (m_py + m_mino_y[i] >= BLOCK_GAMEOVER_LINE_BOTTOM)
		{
			gameover = false;
		}
	}



	oBlock->AddMinoCount();

	Audio::Start(AudioIds::se_Mino_Drop);
//	oBlock->LinesCompleteCheck();
	oBlock->FieldUpdate();

	if (gameover)
		oBlock->SetGameOverFlag(true);
	//HoldFlagを戻し、Nextを作成
	oBlock->SetHoldFlag(true);
	oBlock->NextCreate();
	this->SetStatus(false);
}


void ObjMino::GhostDraw()
{
	bool isBlock = false;
	int gx = m_px;
	int gy = m_py;

	while (isBlock == false)
	{

		for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
		{
			//座標に変換
			int bx = gx + m_mino_x[i]  ;
			int by = gy + m_mino_y[i] ;
			//ミノ座標の1マス下にブロックがあるか確認する。
			if (MinoOverlapCheck(bx, by + 1) == true)
			{
				isBlock = true;
				break;
			}
		}
		if (isBlock == false)
			gy += 1;
	}
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		RECT_F Color_Block_Frame = { BLOCK_PIXELS,BLOCK_PIXELS * m_mino_type,BLOCK_PIXELS,BLOCK_PIXELS };
		//座標に変換
		int bx = (gx + m_mino_x[i]) * BLOCK_PIXELS + FIELD_1P_POS_X + FIELD_OFFSET_X;
		int by = (gy + m_mino_y[i]) * BLOCK_PIXELS + FIELD_1P_POS_Y + FIELD_OFFSET_Y;
		Draw::Draw(texBlock, bx, by, Color_Block_Frame);
	}
}

void ObjMino::MinoMove(int direction)
{

	int plus_x = 0;
	int plus_y = 0;
	
	if (direction == Left)plus_x = -MINO_MOVE_DISTANCE;
	else if (direction == Right)plus_x = MINO_MOVE_DISTANCE;
	else if (direction == Down)plus_y = MINO_MOVE_DISTANCE;

	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		//座標に変換
		int bx = m_px + m_mino_x[i] + plus_x;
		int by = m_py + m_mino_y[i] + plus_y;
		//ミノ座標の1マス左にブロックがあるか確認する。
		if (MinoOverlapCheck(bx, by) == true)
		{
			return;
		}
	}
	//ブロックがない場合、移動する
	m_px += plus_x;
	m_py += plus_y;
	Audio::Start(AudioIds::se_Mino_Move);

	m_ct_landing.Reset();
	//TSpin判定消去
	m_tspin_pattern = E_TSPIN_PATTERN::NoTSpin;
	if (direction != Down)
	m_move_count++;
}

E_TSPIN_PATTERN ObjMino::TSpinCheck(int blockPosX, int blockPosY,ROTATE_ANGLE angle)
{
	int checker_count = 0;
	bool tspin_flag = true;
	for (int i = 0; i < MINO_DATABASE_COL; i++)
	{
		for (int j = 0; j < MINO_DATABASE_LOW; j++)
		{
			int bx = blockPosX + j;
			int by = blockPosY + i;

			if (Tetris::TSPIN_CHECKER[(int)angle][i][j] >= 1)
			{
				if (MinoOverlapCheck(bx, by) == true)
				{
					checker_count++;
				}
				//1の部分にブロックがないばあい、TSpinMiniになる
				else if (Tetris::TSPIN_CHECKER[(int)angle][i][j] == 1)
				{
					tspin_flag = false;
				}
			}
				
		}
	}

	//T-Spinに必要なチェッカーの数は3
	if (checker_count <3)
	{
		return E_TSPIN_PATTERN::NoTSpin;
	}
	else if(tspin_flag)
	{
		return E_TSPIN_PATTERN::TSpin;
	}
	else
	{
		return E_TSPIN_PATTERN::TSpinMini;
	}
}

void ObjMino::SpinSoundPlay(bool spin_result)
{
	if (m_tspin_pattern != E_TSPIN_PATTERN::NoTSpin && spin_result )
	{
		Audio::Start(AudioIds::se_Mino_TSpin);
	}
	else
	{
		Audio::Start(AudioIds::se_Mino_Spin);
	}
}