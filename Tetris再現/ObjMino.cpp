#include "ObjMino.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "GameHead.h"
#include "GameL\WinInputs.h"
#include "CControllerInput.h"
#include "Function.h"
#include "GameL\Audio.h"
#include "GameL/UserData.h"

ObjMino::ObjMino(MINO_TYPE minoType)
{
	m_mino_type = minoType; 
};
//������
void ObjMino::Init()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	int count = 0;
	for (int i = 0; i < MINO_DATABASE_COL; i++)
	{
		for (int j = 0; j < MINO_DATABASE_LOW; j++)
		{
		 	if (count == MINO_BLOCK_AMOUNT)break;
			//0���傫���ꍇ�A���W�̏��ɑ������
			if (MINO_SHAPE[m_mino_type][i][j] == 1)
			{
				m_mino_x[count] = j;
				m_mino_y[count] = i;
				count++;
			}
		}
	}
	//�~�m�̏����ʒu
	m_px = MINO_INIT_POS_X;
	m_py = MINO_INIT_POS_Y;
	m_rotate_angle = RotateAngle0;

	//m_key_flag[0] = false;
	//m_key_flag[1] = false;
	//m_key_flag[2] = false;

	////1�b
	//m_ct_input_move = { CCounter(0.0f,0.0f, FRAME_MINO_INPUT_MOVE, STOP) };
	m_ct_landing = { CCounter(0.0f,0.0f, FRAME_MINO_LANDING, STOP) };
	m_ct_fall = { CCounter(0.0f,0.0f, FRAME_MINO_FALL, STOP) };

	//���b�N�_�E���֘A
	m_down_Block_line = 0;
	m_move_count = 0;

	m_mino_first_action = true;
	m_tspin_pattern = E_TSPIN_PATTERN::NoTSpin;

	m_ct_softdrop = { CCounter(0.0f,0.0f, USER_DATA->m_SDF_frame, STOP) };
	m_ct_arr = { CCounter(0.0f,0.0f, USER_DATA->m_frame_AutoRepeatRate, STOP) };
	m_ct_arr.NowValue = USER_DATA->m_frame_AutoRepeatRate;
	if (oBlock != nullptr)
	{
			//�����ł��Ȃ��ꍇ�A1�u���b�N�����Ă݂�
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

		//�z�[���h�Ń~�m�����ւ����ꍇ�A�x���͂Ȃ�
		if (oBlock->GetHoldFlag())
			m_ct_next_delay = { CCounter(0.0f,0.0f,USER_DATA->m_next_create_delay_frame,STOP) };
	}


}

//�i�s
void ObjMino::Action()
{ 

	//�u���b�N�̏����擾
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);

	if (!m_ct_next_delay.GetMaxReached() && m_ct_next_delay.GetUse())
	{
		m_ct_next_delay.Add(1);
		return;
	}


	ObjPlayerControll* ctrl = (ObjPlayerControll*)Objs::GetObj(OBJ_PLAYERCONTROLL);
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

	//CCounter����
	m_ct_landing.Add(1.0f);
	//m_ct_input_move.Add(1.0f);
	m_ct_fall.Add(1.0f);

	m_ct_arr.Add(1.0f);
		//�ړ�
	if (ctrl->GetButtonInput(E_PLAYER_CONTROLL::Button_LEFT))
	{
		if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_LEFT) || 
			(ctrl->GetButtonLongPressFrame(E_PLAYER_CONTROLL::Button_LEFT) >= USER_DATA->m_frame_DelayerAutoShift && m_ct_arr.GetMaxReached()))
		{
			MinoMove(Left);
			m_ct_arr.Reset();
		}
		
	}
	else if (ctrl->GetButtonInput(E_PLAYER_CONTROLL::Button_RIGHT))
	{
		if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_RIGHT) ||
			(ctrl->GetButtonLongPressFrame(E_PLAYER_CONTROLL::Button_RIGHT) >= USER_DATA->m_frame_DelayerAutoShift && m_ct_arr.GetMaxReached()))
		{
			MinoMove(Right);
			m_ct_arr.Reset();
		}
	}
	
	//�����邲�ƂɈړ��񐔂����Z�b�g
	if (ctrl->GetButtonInput(E_PLAYER_CONTROLL::Button_DOWN))
	{
		if (GetMinoBlockFixed() == false && m_ct_softdrop.GetMinReached())
		{
			MinoMove(Down);
			m_move_count = 0;
			oScore->AddScore(FALL_ADD_SCORE);
			m_ct_fall.Reset();
		}
		m_ct_softdrop.Add(1);
		if (m_ct_softdrop.GetMaxReached())
		{
			m_ct_softdrop.Reset();
		}
	}
	else
	{
		m_ct_softdrop.Reset();
	}
	//�ʏ�~�����̓X�R�A��^�������Ȃ����߁A������
	if (m_ct_fall.GetMaxReached() && !oBlock->GetOptions()->option_flag[E_PRACTICE_OPTION::NoNaturalDrop])
	{
		if (GetMinoBlockFixed() == false)
		{
			MinoMove(Down);
			m_move_count = 0;
			m_ct_fall.Reset();
		}
	}

	if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_UP))
	{
		//���Ƃ��Ĕz�u
		MinoHardDrop();
		SetFieldMino();
		return;
	}
	//����]
	if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_B))
	{
			RotateMino(GetRotateKey(E_PLAYER_CONTROLL::Button_B));
			m_move_count++;
	}
	//�E��]
	if (ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_A))
	{

		RotateMino(GetRotateKey(E_PLAYER_CONTROLL::Button_A));
		m_move_count++;
	}


	if ((ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_L) || ctrl->GetButtonInputOnce(E_PLAYER_CONTROLL::Button_R)) && 
		oBlock->GetHoldFlag() == true)
	{
			Audio::Start(se_Mino_Hold);
			oBlock->SetHoldFlag(false);
			oBlock->SetHoldType(m_mino_type);
			this->SetStatus(false);
			return;
	}
	
	if(GetMinoBlockFixed() == false)
	{
		//���n���Ă��Ȃ��ꍇ�A���0�ɂ���
		m_ct_landing.Reset();
	}
	else
	{

		m_ct_fall.Reset();
	}



	//�I���l�ɒB����
	if ((m_ct_landing.GetMaxReached()||
		m_move_count >= LOCKDOWN_MAX_MOVECOUNT)&& GetMinoBlockFixed() == true &&
		!oBlock->GetOptions()->option_flag[E_PRACTICE_OPTION::NoNaturalDrop])
	{
		SetFieldMino();
		return;
	}
}

//�`��
void ObjMino::Draw()
{
	if (!m_ct_next_delay.GetMaxReached() && m_ct_next_delay.GetUse())
	{
		m_ct_next_delay.Add(1);
		return;
	}

	GhostDraw();
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		float bx = (m_px + m_mino_x[i]) * BLOCK_PIXELS + FIELD_1P_POS_X + FIELD_OFFSET_X;
		float by = (m_py + m_mino_y[i]) * BLOCK_PIXELS + FIELD_1P_POS_Y + FIELD_OFFSET_Y;
		RECT_F Color_Block = { 0,m_mino_type * BLOCK_PIXELS,BLOCK_PIXELS,BLOCK_PIXELS };
		Draw::Draw(texBlock, bx,by , Color_Block);
		Draw::Draw(texBlock, bx, by, BLOCK_LIGHT_SRC_POS);
	}
	
	
}

void ObjMino::MinoHardDrop()
{

	ObjScore* oScore = (ObjScore*)Objs::GetObj(OBJ_SCORE);
	while (1)
	{
		for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
		{
			//���W�ɕϊ�
			int bx = m_px + (m_mino_x[i]);
			int by = m_py + (m_mino_y[i]);
			//�~�m���W��1�}�X���Ƀu���b�N�����邩�m�F����B
			if (MinoOverlapCheck(bx, by + 1) == true)
			{
				return;
			}
		}
		m_py += 1;


		oScore->AddScore(FALL_ADD_SCORE);
	}
}

//�����̗���
// �~�m�̍��W�����̍��W�ɑ��
// ���̍��W��������]��SRS������
// ��]�ł���悤�������炻�̊֐����ō��W�̒u������������
//
void ObjMino::RotateMino(int direction)
{
	bool result = false;
	//O�~�m�̏ꍇ�A��]�͕s�K�v
	if (m_mino_type == Mino_O)
	{
		SpinSoundPlay(false);
		return;
	}

	//���ڍ��W��n���ƁA���̂܂܏����������Ă��܂��̂ŁA
	//��]�ł��邩�m���߂Ă����]���������̂ō쐬
	//���W�͂��̊֐����ŏ�����������B
	int new_pos_x[MINO_BLOCK_AMOUNT];
	int new_pos_y[MINO_BLOCK_AMOUNT];
	float r = direction == Left ? -ROTATION_ANGLE : ROTATION_ANGLE;
	if (m_mino_type == Mino_I)
	{
		//�V���W�̃|�C���^��n��
		RotationMino(new_pos_x, new_pos_y, r, MINO_ROTATION_AXIS_I);
		result = SuperRotationSystem(new_pos_x, new_pos_y, direction, m_mino_type);
	}
	else
	{
		//�V���W�̃|�C���^��n��
		RotationMino(new_pos_x, new_pos_y, r, MINO_ROTATION_AXIS);
		result = SuperRotationSystem(new_pos_x, new_pos_y, direction, m_mino_type);
	}
	SpinSoundPlay(result);

	//��]�ɐ��������璅�n���Ԃ����Z�b�g����
	if (result)
	{
		m_ct_landing.Reset();
	}
}

//�߂�l:��]����
bool ObjMino::SuperRotationSystem(int* minoPosX, int* minoPosY, int rotate_dir, int mino_type)
{
	int* shift_x;
	int* shift_y;

	//�E��]�A����]�Ɠ������Ⴄ�B
	//�܂��AI�~�m�͂�����Ɠ���ł��B
	//�ڂ����͂��������Ă��炦��΂Ǝv���܂��B
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
			//���W�ɕϊ�
			//�S�̓I�ȍ��W + ��]��̍��W + ���炵�����W �Ōv�Z����B
			int bx = m_px + minoPosX[j] + shift_x[i];
			int by = m_py + minoPosY[j] + shift_y[i];

			//4�񂸂��ׁAtrue���Ԃ��Ă������_�ł����ɂ���Ƃ������ƁB
			//4��false�̏ꍇ�͂����ɂȂ��Ƃ�������
			if (MinoOverlapCheck(bx, by) == true)
			{
				isBlock = true;
				break;
			}
		}

		if (isBlock == false)
		{
			//�~�m�̍��W�ɑ������ƁA��]���������\�������邽��
			//�S�̓I�ȍ��W�ɉ��Z����
			m_px += shift_x[i];
			m_py += shift_y[i];
			for (int j = 0; j < MINO_BLOCK_AMOUNT; j++)
			{
				m_mino_x[j] = minoPosX[j];
				m_mino_y[j] = minoPosY[j];
			}


			//�V������]�p���L��������
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

//���̈ʒu�ɂ��łɃu���b�N�����邩�ǂ����m�F����B
//���݂����ꍇ�Atrue��Ԃ��B
bool ObjMino::MinoOverlapCheck(int x, int y)
{
	//�u���b�N�̏����擾
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	return oBlock->BlockPosCheck(x,y);
}

//�~�m�����n���Ă��邩�ǂ����Ԃ�
bool ObjMino::GetMinoBlockFixed()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		//���W�ɕϊ�
		int bx = m_px + (m_mino_x[i]);
		int by = m_py + (m_mino_y[i]);
		//�~�m���W��1�}�X���Ƀu���b�N�����邩�m�F����B
		if (oBlock->m_field[by + 1][bx] != BlockEmpty || by + 1 >= FIELD_HEIGHT)
		{
			//������
			return true;
		}
	}
	return false;
}

//�V���W�̃|�C���^���󂯎��A�l��ҏW���ĕԂ�
void ObjMino::RotationMino(int* minoPosX, int* minoPosY, float rotation, float axis)
{

	//axis = ��]��
	for (int i = 0; i < MINO_BLOCK_AMOUNT; i++)
	{
		//������x�N�g�������߂�
		float rvx = axis - m_mino_x[i];
		float rvy = axis - m_mino_y[i];
		//2�_�ԋ��������߂�
		float l = DistanceF(m_mino_x[i], m_mino_y[i], axis, axis);
		//�x�N�g������p�x�����߂�
		float r = atan2(rvy, rvx);
		r = r * 180.0f / 3.14f;

		//���̈ʒu�̕ϐ���錾���A�������Ă�����W�ɑ������
		float pos_x = axis;
		float pos_y = axis;

		r -= rotation;


		if (r < 0.0f)
			r = abs(r);
		else
			r = 360.0f - abs(r);

		//�p�x��ϊ������Ă�����Ƃ��Ƃ̋����̕��߂�
		pos_x += cos(3.14 / 180 * r) * l;
		pos_y += -sin(3.14 / 180 * r) * l;

		//���̂܂ܕϊ�����Ə����_�ȉ����؂�̂Ă��邽�߁A
		//�������Ă���������
		minoPosX[i] = RoundingFirstDecimal(pos_x);
		minoPosY[i] = RoundingFirstDecimal(pos_y);

	}

}

void ObjMino::SetFieldMino()
{
	//�u���b�N��z�u
	//�u���b�N�̏����擾
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
	oBlock->FieldUpdate();

	if (gameover)
		oBlock->SetGameOverFlag(true);
	//HoldFlag��߂��ANext���쐬
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
			//���W�ɕϊ�
			int bx = gx + m_mino_x[i]  ;
			int by = gy + m_mino_y[i] ;
			//�~�m���W��1�}�X���Ƀu���b�N�����邩�m�F����B
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
		//���W�ɕϊ�
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
		//���W�ɕϊ�
		int bx = m_px + m_mino_x[i] + plus_x;
		int by = m_py + m_mino_y[i] + plus_y;
		//�~�m���W��1�}�X���Ƀu���b�N�����邩�m�F����B
		if (MinoOverlapCheck(bx, by) == true)
		{
			return;
		}
	}
	//�u���b�N���Ȃ��ꍇ�A�ړ�����
	m_px += plus_x;
	m_py += plus_y;
	Audio::Start(AudioIds::se_Mino_Move);

	m_ct_landing.Reset();
	//TSpin�������
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
				//1�̕����Ƀu���b�N���Ȃ��΂����ATSpinMini�ɂȂ�
				else if (Tetris::TSPIN_CHECKER[(int)angle][i][j] == 1)
				{
					tspin_flag = false;
				}
			}
				
		}
	}

	//T-Spin�ɕK�v�ȃ`�F�b�J�[�̐���3
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

DIRECTION ObjMino::GetRotateKey(E_PLAYER_CONTROLL ctrl)
{
	DIRECTION dir = Left;
	if (USER_DATA->m_reverse_rotate)
		dir = ctrl == E_PLAYER_CONTROLL::Button_B ? Right : Left;
	else
		dir = ctrl == E_PLAYER_CONTROLL::Button_B ? Left : Right;
	return dir;
}