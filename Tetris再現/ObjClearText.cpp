#include "ObjClearText.h"
#include "Function.h"
#include "GameL\DrawTexture.h"

using namespace Tetris::ClearText;
void ObjClearText::Init()
{
	m_ren = 0;
	m_lines = 0;
	m_tspin_type = E_TSPIN_PATTERN::NoTSpin;

	m_ani_ren.LoadData("Animation/ClearText/ren");
	m_ani_lines.LoadData("Animation/ClearText/lines");
	m_ani_tspin.LoadData("Animation/ClearText/tspin");
	m_ani_btb.LoadData("Animation/ClearText/btb");
	m_ani_perfect.LoadData("Animation/ClearText/perfect");
	m_num_ren.SetTex(texNumber, RECT_NUMBER.x, RECT_NUMBER.y);
}
void ObjClearText::Action()
{
	m_ani_ren.Action();
	m_ani_lines.Action();
	m_ani_tspin.Action();
	m_ani_btb.Action();
	m_ani_perfect.Action();
}
void ObjClearText::Draw()
{
	if (m_ani_ren.GetStart())
	{
		Draw::SetOpacity(texText, m_ani_ren.GetValue("Ren").opacity);
		Draw::SetOpacity(texNumber, m_ani_ren.GetValue("Ren").opacity);
		Draw::SetRotationZ(texText, m_ani_ren.GetValue("Ren").rotate_z);
		Draw::SetRotationZ(texNumber, m_ani_ren.GetValue("Ren").rotate_z);
		Draw::SetScaleX (texText, m_ani_ren.GetValue("Ren").scale_x * 0.5f);
		Draw::SetScaleX (texNumber, m_ani_ren.GetValue("Ren").scale_x);
		Draw::SetScaleY (texText, m_ani_ren.GetValue("Ren").scale_y * 0.5f);
		Draw::SetScaleY (texNumber, m_ani_ren.GetValue("Ren").scale_y);
		Draw::Draw(texText, DISPLAYED_REN_POS.x + m_ani_ren.GetValue("Ren").x, DISPLAYED_REN_POS.y + m_ani_ren.GetValue("Ren").y, RECT_REN, false, false);
		m_num_ren.Draw(m_ren, DISPLAYED_REN_NUMBER_POS.x, DISPLAYED_REN_NUMBER_POS.y, 35);
	}
	if (m_ani_lines.GetStart())
	{
		Draw::SetOpacity(texText, m_ani_lines.GetValue("Lines").opacity);
		Draw::SetRotationZ(texText, m_ani_lines.GetValue("Lines").rotate_z);
		Draw::SetScaleX(texText, m_ani_lines.GetValue("Lines").scale_x);
		Draw::SetScaleY(texText, m_ani_lines.GetValue("Lines").scale_y);
		Draw::CenterDraw(texText, DISPLAYED_LINES_POS.x + m_ani_lines.GetValue("Lines").x, DISPLAYED_LINES_POS.y + m_ani_lines.GetValue("Lines").y, RECT_LINES[m_lines - 1], false, false);

	}
	
	if (m_ani_tspin.GetStart())
	{
		Draw::SetOpacity(texText, m_ani_tspin.GetValue("TSpin").opacity);
		Draw::SetRotationZ(texText, m_ani_tspin.GetValue("TSpin").rotate_z);
		Draw::SetScaleX(texText, m_ani_tspin.GetValue("TSpin").scale_x);
		Draw::SetScaleY(texText, m_ani_tspin.GetValue("TSpin").scale_y);

		if(m_tspin_type == E_TSPIN_PATTERN::TSpin)
		Draw::CenterDraw(texText, DISPLAYED_TSPIN_POS.x + m_ani_tspin.GetValue("TSpin").x, DISPLAYED_TSPIN_POS.y + m_ani_tspin.GetValue("TSpin").y, RECT_TSPIN, false, false);
		if (m_tspin_type == E_TSPIN_PATTERN::TSpinMini)
			Draw::CenterDraw(texText, DISPLAYED_TSPIN_POS.x + m_ani_tspin.GetValue("TSpin").x, DISPLAYED_TSPIN_POS.y + m_ani_tspin.GetValue("TSpin").y, RECT_TSPINMINI, false, false);

	}
	if (m_ani_btb.GetStart())
	{
		Draw::SetOpacity(texText, m_ani_btb.GetValue("BTB").opacity);
		Draw::SetRotationZ(texText, m_ani_btb.GetValue("BTB").rotate_z);
		Draw::SetScaleX(texText, m_ani_btb.GetValue("BTB").scale_x);
		Draw::SetScaleY(texText, m_ani_btb.GetValue("BTB").scale_y);
		Draw::CenterDraw(texText, DISPLAYED_BTB_POS.x + m_ani_btb.GetValue("BTB").x, DISPLAYED_BTB_POS.y + m_ani_btb.GetValue("BTB").y, RECT_BTB, false, false);

	}
	if (m_ani_perfect.GetStart())
	{
		Draw::SetOpacity(texText, m_ani_perfect.GetValue("Perfect").opacity);
		Draw::SetRotationZ(texText, m_ani_perfect.GetValue("Perfect").rotate_z);
		Draw::SetScaleX(texText, m_ani_perfect.GetValue("Perfect").scale_x);
		Draw::SetScaleY(texText, m_ani_perfect.GetValue("Perfect").scale_y);
		Draw::CenterDraw(texText, Tetris::FIELD_1P_POS_X + DISPLAYED_PERFECT_POS.x + m_ani_perfect.GetValue("Perfect").x, Tetris::FIELD_1P_POS_Y + DISPLAYED_PERFECT_POS.y + m_ani_perfect.GetValue("Perfect").y, RECT_PERFECT, false, false);

	}
}

void ObjClearText::AnimeStart(int ren, int lines, bool btb, E_TSPIN_PATTERN tspin, bool perfect)
{
	if (ren >= 1)
	{
		m_ren = ren;
		m_ani_ren.Start();
	}
	if (lines >= 1)
	{
		m_lines = lines;
		m_ani_lines.Start();
	}
	if (btb)
	{

		m_ani_btb.Start();
	}
	if (tspin != E_TSPIN_PATTERN::NoTSpin)
	{
		m_tspin_type = tspin;
		m_ani_tspin.Start();
	}
	if (perfect)
	{
		m_ani_perfect.Start();
	}
}

void ObjClearText::ClearAnimeStart()
{

}
void ObjClearText::FailureAnimeStart()
{

}