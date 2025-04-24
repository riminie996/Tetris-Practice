#include "Tetris.h"
#include "GameL\DrawTexture.h"
#include "GameHead.h"
int Tetris::CalcSendGarbageLines(int clear_line, int ren, bool btb, E_TSPIN_PATTERN tspin, bool perfect)
{
	int garbage_lines = 0;

	//
	// 
	//	Clear Type									Rows Cleared	Garbage Rows Sent
	//	Single										1				0
	//	Double										2				1
	//	Triple										3				2
	//	Tetris										4				4
	//	Mini T-Spin Single(wherever available)		1				0
	//	Mini T-Spin Double(wherever available)		2				1
	//	T-Spin Single								1				2
	//	T-Spin Double								2				4
	//	T-Spin Triple								3				6
	//	Back-to-Back Bonus							0				+1 (MTSS, MTSD; TSS), +2 (TSD; Tetris); +3 (TST)
	//	Perfect clear								1 - 4			10 (+last line clear type if applicable)
	//

	//
	//	0REN +0
	//	1REN +0 
	//	2REN +1 
	//	3REN +1 
	//	4REN +2 
	//	5REN +2 
	//	6REN +3 
	//	7REN +3 
	//	8REN +4 
	//	9REN +4 
	//	10REN +4
	//	11REN +5
	//	12REN +5
	//	13REN +5
	//	14REN +5

	//Tスピン
	if (tspin == E_TSPIN_PATTERN::TSpin)
	{
		garbage_lines += clear_line * 2;
	}
	else if (clear_line >= 4)
	{
		garbage_lines += 4;
	}
	else
	{
		garbage_lines += clear_line - 1;
	}
	//REN
	if (ren >= 11)
		garbage_lines += 5;
	else if (ren >= 8)
		garbage_lines += 4;
	else if (ren >= 6)
		garbage_lines += 3;
	else if (ren >= 4)
		garbage_lines += 2;
	else if (ren >= 2)
		garbage_lines += 1;
	else
		;


	if (btb)
	{
		garbage_lines += 1;
	}
	if (perfect)
	{
		garbage_lines += 10;
	}

	return garbage_lines;
}

void Tetris::Mino_Shape_Draw(int x, int y, MINO_TYPE type)
{
	float offset_x = 0.0f;
	float offset_y = 0.0f;
	//真ん中に表示させたいため、ずらす
	if (type == Mino_I || type == Mino_O)
	{
		offset_y = BLOCK_PIXELS;
	}
	else//3×3ブロックのやつ
	{
		offset_x = BLOCK_PIXELS / 2.0f;
		offset_y = BLOCK_PIXELS;
	}

	//初期位置
	float bx = x + ((float)MINO_DATABASE_LOW / 2.0f) * -BLOCK_PIXELS + offset_x;
	float by = y + ((float)MINO_DATABASE_COL / 2.0f) * -BLOCK_PIXELS + offset_y;
	GameL::RECT_F Color_Block = { 0,BLOCK_PIXELS * type,BLOCK_PIXELS,BLOCK_PIXELS };

	for (int i = 0; i < MINO_DATABASE_COL; i++)
	{
		for (int j = 0; j < MINO_DATABASE_LOW; j++)
		{
			if (MINO_SHAPE[type][i][j] == 1)
			{
				GameL::Draw::Draw(texBlock, bx + (j * BLOCK_PIXELS), by + (i * BLOCK_PIXELS), Color_Block);
				GameL::Draw::Draw(texBlock, bx + (j * BLOCK_PIXELS), by + (i * BLOCK_PIXELS), BLOCK_LIGHT_SRC_POS);
			}
		}
	}
}
std::wstring Tetris::GetOptionName(E_PRACTICE_OPTION option)
{
	int type = (int)option;

	if (type >= TetriminoOrderFixed_1 && type <= TetriminoOrderFixed_End)
		return L"テトリミノ" + std::to_wstring(type + 1) + L"巡目固定";
	else if (option == NoNaturalDrop)
		return L"自然落下なし";
	else if (option == DPCGuide)
		return L"未使用";
	else if (option == InfiniteHold)
		return L"無限ホールド";
	else if (option == RisingTimer)
		return L"せりあがりタイマー:";
	else if (option == User_ARR)
		return L"ARR";
	else if (option == User_DAS)
		return L"DAS";
	else if (option == User_SDF)
		return L"SDF";
	else if (option == User_ReverseRotate)
		return L"回転操作反転";
	else if (option == User_AccidentalHardDrop)
		return L"上キー暴発対策";
	else if (option == User_NextCreateDelayFrame)
		return L"次のミノ生成されるまでのフレーム";

	return L"error";
}