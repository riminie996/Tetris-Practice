#pragma once

//オブジェクトネーム------------------------------
enum OBJ_NAME
{
	OBJ_NO_NAME,	//オブジェクト名無し(禁止)
	//ゲームで使うオブジェクトの名前
	//OBJ_○○と表記
	OBJ_BLOCK,
	OBJ_MINO,
	OBJ_SCORE,
	OBJ_GUIDE,
	OBJ_RISING,
	OBJ_FIXEDMINOSELECT,
	OBJ_PRACTICEOPTION,
};
enum OBJ_PRIORITY
{
	PRIO_FIELD,
	PRIO_MINO,
	PRIO_RISING,
	PRIO_OPTION,
	PRIO_FIXED_MINO_SETTINGS,
};
//------------------------------------------------


//テクスチャネーム----------------------------------
enum TEXTURE_NAME
{
	texBlock,
	texNext,
	texBase,
	texBlack32,
};
//オブジェクト番号
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
	BlockWall,		//壁
	blockCOUNTER,	//ブロックの種類を数える用
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
//セーブ＆ロードとシーン間のやり取りするデータ
struct UserData
{
	//ユーザーが直接変更できるデータ
	
};
//------------------------------------------------


//ゲーム内で使用されるグローバル変数・定数・列挙--


//------------------------------------------------
//ゲーム内で使用するクラスヘッダ------------------
#include "CCounter.h"

//----------------------------------------------

//ゲームシーンオブジェクトヘッダ-----------------
#include "ObjBlock.h"
#include "ObjMino.h"
#include "ObjScore.h"
#include "ObjGuide.h"
#include "ObjRisingTimer.h"
#include "ObjFixedMinoSettings.h"
#include "ObjPracticeOption.h"
#include "Tetris.h"
//------------------------------------------------

//ゲームシーンクラスヘッダ------------------------
#include "SceneMain.h"

//-----------------------------------------------

//シーンスタートクラス---------------------------
//ゲーム開始時のシーンクラス登録
#define SET_GAME_START SceneMain
//-----------------------------------------------