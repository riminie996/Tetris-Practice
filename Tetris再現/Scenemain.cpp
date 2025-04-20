//STL�f�o�b�O�@�\��OFF�ɂ���
#define _SECURE_SCL (O)
#define _HAS_ITERATOR_DEBUGGING (O)

//GameL�Ŏg�p����w�b�_�[
#include "GameL\SceneObjManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\UserData.h"
#include "GameL/WinInputs.h"
#include "GameL\Audio.h"


//�g�p����l�[���X�y�[�X
using namespace GameL;

//�g�p�w�b�_�[
#include "SceneMain.h"
#include "GameHead.h"
#include "Function.h"

//�R���X�g���N�^
SceneMain::SceneMain()
{

}

//�f�X�g���N�^
SceneMain::~SceneMain()
{

}
//������
void SceneMain::InitScene()
{
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
	//������---------------------------------------

	//�摜�̓ǂݍ���---------------------------
	Draw::LoadImageW(L"BlockTexture.png",	texBlock, TEX_SIZE_64);
	Draw::LoadImageW(L"Next.png",			texNext, TEX_SIZE_128);
	Draw::LoadImageW(L"Base.png",			texBase, TEX_SIZE_128);
	Draw::LoadImageW(L"Block32.png",		texBlack32, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/Text.png",		texText, TEX_SIZE_128);
	Draw::LoadImageW(L"Texture/ScoreNum.png",		texNumber, TEX_SIZE_128);

	//�I�u�W�F�N�g�̍쐬--------------------------
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

	Save::Open();

}
//���s��
void SceneMain::Scene()
{
	
}