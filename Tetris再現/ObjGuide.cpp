#include "ObjGuide.h"
#include "GameL\SceneManager.h"
#include "GameL\DrawTexture.h"
#include "GameL\DrawFont.h"
#include "Function.h"
//初期化
void ObjGuide::Init()
{
	m_is_show_dpc = false;

	std::string guide_path = OpenFileDialog(L"Guide/");
	FieldMapImport(guide_path, *m_field);


	
}

//進行
void ObjGuide::Action()
{

}

//描画
void ObjGuide::Draw()
{
	ObjBlock* oBlock = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	oBlock->FieldDraw(768 + FIELD_OFFSET_X, FIELD_1P_POS_Y + FIELD_OFFSET_Y, m_field);
}

void ObjGuide::Reset()
{
	Init();
}