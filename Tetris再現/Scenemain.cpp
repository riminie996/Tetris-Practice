//STLデバッグ機能をOFFにする
#define _SECURE_SCL (O)
#define _HAS_ITERATOR_DEBUGGING (O)

//GameLで使用するヘッダー
#include "GameL\SceneObjManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\UserData.h"
#include "GameL/WinInputs.h"
#include "GameL\Audio.h"


//使用するネームスペース
using namespace GameL;

//使用ヘッダー
#include "SceneMain.h"
#include "GameHead.h"
#include "Function.h"

//コンストラクタ
SceneMain::SceneMain()
{

}

//デストラクタ
SceneMain::~SceneMain()
{

}
//初期化
void SceneMain::InitScene()
{
	if (!Save::Open())
	{
		USER_DATA->m_frame_AutoRepeatRate = FRAME_MINO_MOVE_SPEED;//横移動の速度(お
		USER_DATA->m_frame_DelayerAutoShift = FRAME_MINO_INPUT_MOVE;//横ため時間
		USER_DATA->m_SDF_frame = FRAME_MINO_SOFTDROP;//Softdrop speed
		USER_DATA->m_accidental_harddrop_frame = 0;
		USER_DATA->m_reverse_rotate = false;
		USER_DATA->m_next_create_delay_frame = 6;
	}

	Audio::LoadAudio2(AudioIds::se_Mino_Move,	"Sound/Mino_Move.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Mino_Drop,	"Sound/Mino_Drop.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Mino_Spin,	"Sound/Mino_Spin.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Mino_TSpin,	"Sound/Mino_TSpin.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Mino_Hold,	"Sound/Mino_Hold.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Line_Clear,	"Sound/Line_Clear.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Line_Tetris, "Sound/Line_Clear_Tetris.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Line_TSpin,	"Sound/Line_Clear_TSpin.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Line_BTB,	"Sound/BTB.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Perfect,		"Sound/Perfect.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Rising,		"Sound/Rising.ogg", GameL::SOUND_TYPE::EFFECT);
	Audio::LoadAudio2(AudioIds::se_Garbage,		"Sound/Garbage.ogg", GameL::SOUND_TYPE::EFFECT);

	std::vector<std::string> vec_bgm_path = LoadDirectory("BGM\\", ".ogg");
	std::random_device rand;

	Audio::LoadAudio2(AudioIds::bgm_1, vec_bgm_path[rand() % vec_bgm_path.size()].c_str(), GameL::SOUND_TYPE::BACK_MUSIC);

	Audio::VolumeMaster(-0.9f);
	//初期化---------------------------------------

	//画像の読み込み---------------------------
	Draw::LoadImageW(L"BlockTexture.png",	texBlock, TEX_SIZE_64);
	Draw::LoadImageW(L"Next.png",			texNext, TEX_SIZE_128);
	Draw::LoadImageW(L"Base.png",			texBase, TEX_SIZE_128);
	Draw::LoadImageW(L"Block32.png",		texBlack32, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/Text.png",		texText, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/ScoreNum.png",		texNumber, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/Controller/Base.png", texControllerBase, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/Controller/Input.png", texControllerInput, TEX_SIZE_128);

	//オブジェクトの作成--------------------------
	ObjBlock* oBlock = new ObjBlock();
	Objs::InsertObj(oBlock, OBJ_BLOCK, PRIO_FIELD);

	ObjScore* oScore = new ObjScore();
	Objs::InsertObj(oScore, OBJ_SCORE, PRIO_SCORE);

	ObjPlayerControll* oCtrl = new ObjPlayerControll();
	Objs::InsertObj(oCtrl, OBJ_PLAYERCONTROLL, PRIO_CONTROLL);
	ObjClearText* oText = new ObjClearText();
	Objs::InsertObj(oText, OBJ_CLEARTEXT, PRIO_CLEARTEXT);
	//ObjGuide* oGuide = new ObjGuide();
	//Objs::InsertObj(oGuide, OBJ_GUIDE, 1);


}
//実行中
void SceneMain::Scene()
{
	
}