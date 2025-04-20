#pragma once

//�I�u�W�F�N�g�l�[��------------------------------
enum OBJ_NAME
{
	OBJ_NO_NAME,	//�I�u�W�F�N�g������(�֎~)
	//�Q�[���Ŏg���I�u�W�F�N�g�̖��O
	//OBJ_�����ƕ\�L
	OBJ_BLOCK,
	OBJ_MINO,
	OBJ_SCORE,
	OBJ_GUIDE,
	OBJ_RISING,
	OBJ_FIXEDMINOSELECT,
	OBJ_PRACTICEOPTION,
	OBJ_PLAYERCONTROLL,
	OBJ_CLEARTEXT,
};
enum OBJ_PRIORITY
{
	PRIO_FIELD,
	PRIO_SCORE,
	PRIO_MINO,
	PRIO_RISING,
	PRIO_OPTION,
	PRIO_FIXED_MINO_SETTINGS,
	PRIO_CLEARTEXT,
	PRIO_CONTROLL,
};
//------------------------------------------------


//�e�N�X�`���l�[��----------------------------------
enum TEXTURE_NAME
{
	texBlock,
	texNext,
	texBase,
	texBlack32,
	texText,
	texNumber,
};
//�I�u�W�F�N�g�ԍ�
enum MapObjects 
{
	BlockEmpty,
	BlockGreen,
	BlockRed,
	BlockPurple,
	BlockYellow,
	BlockCyan,
	BlockBlue,
	BlockOrrange,
	BlockWall,		//��
	blockCOUNTER,	//�u���b�N�̎�ނ𐔂���p
};
enum AudioIds
{
	se_Mino_Move,
	se_Mino_Drop,
	se_Mino_Spin,
	se_Mino_TSpin,
	se_Mino_Hold,
	se_Line_Clear,
	se_Line_Tetris,
	se_Line_TSpin,
	se_Line_BTB,
	se_Perfect,
	se_Garbage,
	se_Rising,
	bgm_1,
};


//------------------------------------------------


//------------------------------------------------

//------------------------------------------------
//�Z�[�u�����[�h�ƃV�[���Ԃ̂���肷��f�[�^
struct UserData
{
	//���[�U�[�����ڕύX�ł���f�[�^
	int m_ARR_frame;
	int m_DAS_frame;
	int m_SDF_frame;
	int m_accidental_harddrop_frame;
	bool m_counter_clockwise;
};
//------------------------------------------------


//�Q�[�����Ŏg�p�����O���[�o���ϐ��E�萔�E��--


//------------------------------------------------
//�Q�[�����Ŏg�p����N���X�w�b�_------------------
//#include "CCounter.h"
//#include "CTimeCounter.h"
//#include "Function.h"
//#include "CAnime3.h"
//#include "CNumTexSource.h"

//----------------------------------------------

//�Q�[���V�[���I�u�W�F�N�g�w�b�_-----------------
#include "ObjBlock.h"
#include "ObjMino.h"
#include "ObjScore.h"
#include "ObjGuide.h"
#include "ObjPracticeOption.h"
#include "ObjRisingTimer.h"
#include "ObjFixedMinoSettings.h"
#include "ObjPlayerControll.h"
#include "ObjClearText.h"
//#include "Tetris.h"
//------------------------------------------------

//�Q�[���V�[���N���X�w�b�_------------------------
#include "SceneMain.h"

//-----------------------------------------------

//�V�[���X�^�[�g�N���X---------------------------
//�Q�[���J�n���̃V�[���N���X�o�^
#define SET_GAME_START SceneMain
//-----------------------------------------------