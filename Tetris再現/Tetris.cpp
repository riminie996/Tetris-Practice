#include "Tetris.h"
#include "GameL\DrawTexture.h"
#include "GameHead.h"

#include <fstream>
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

	//0以下なら0
	if (garbage_lines <= 0)
		return 0;

	return garbage_lines;
}

void Tetris::Mino_Shape_Draw(int x, int y, MINO_TYPE type)
{
	if (type == Mino_Empty)return;

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
	else if (option == ShowController)
		return L"コントローラー表示:";
	else if (option == InfiniteHold)
		return L"無限ホールド";
	else if (option == RisingTimer)
		return L"せりあがりタイマー:";
	else if (option == GameMode)
		return L"ゲームモード:";
	else if (option == NextDisplayedCount)
		return L"ネクスト表示数:";
	else if (option == User_AutoRepeatRate)
		return L"AutoRepeatRate:";
	else if (option == User_DelayerAutoShift)
		return L"DelayerAutoShift:";
	else if (option == User_SDF)
		return L"SDF:";
	else if (option == User_ReverseRotate)
		return L"回転操作反転";
	else if (option == User_AccidentalHardDrop)
		return L"未使用";
	else if (option == User_NextCreateDelayFrame)
		return L"次のミノ生成されるまでのフレーム:";

	return L"error";
}

Tetris::E_GAME_MODE& Tetris::operator++(E_GAME_MODE& mode)
{
	int nMode = (int)mode;
	nMode++;

	if (nMode >= (int)E_GAME_MODE::mode_count)
	{
		nMode = (int)E_GAME_MODE::mode_count - 1;
	}
	mode = (E_GAME_MODE)nMode;
	return mode;
};
Tetris::E_GAME_MODE& Tetris::operator--(E_GAME_MODE& mode)
{
	int nMode = (int)mode;
	nMode--;

	if (nMode  <0)
	{
		nMode = 0;
	}
	mode = (E_GAME_MODE)nMode;
	return mode;
};

std::wstring Tetris::PracticeOption::GetStrGameMode(E_GAME_MODE mode)
{
	if (mode == E_GAME_MODE::mode_Standard)
		return L"スタンダード";
	else if (mode == E_GAME_MODE::mode_40Line)
		return L"40ライン";
	else if (mode == E_GAME_MODE::mode_TSD20)
		return L"TSD20";
	else if (mode == E_GAME_MODE::mode_ULTRA)
		return L"ウルトラ";
	else if (mode == E_GAME_MODE::mode_100LineCheez)
		return L"100ラインチーズ";
	else if (mode == E_GAME_MODE::mode_4WRen)
		return L"4列REN";
	else if (mode == E_GAME_MODE::mode_Spin)
		return L"回転入れ練習モード";

	return L"error";
}

MINO_TYPE Tetris::CharToMino(char c)
{
	switch (c)
	{
	case 'S':
		return Mino_S;
	case 'Z':
		return Mino_Z;
	case 'T':
		return Mino_T;
	case 'J':
		return Mino_J;
	case 'L':
		return Mino_L;
	case 'O':
		return Mino_O;
	case 'I':
		return Mino_I;
	default:
		return Mino_Empty;
	}
}

void Tetris::FieldMapExport(MapObjects field[Tetris::FIELD_HEIGHT][Tetris::FIELD_WIDTH])
{
	std::ofstream writing_file;
	std::string filename = "field.txt";
	writing_file.open(filename, std::ios::out);
	std::string writing_text = "";

	for (int i = 0; i < Tetris::FIELD_HEIGHT; i++)
	{
		for (int j = 0; j < Tetris::FIELD_WIDTH; j++)
		{
			writing_text += std::to_string((int)field[i][j]) + ",";
		}
		writing_text += "\n";
	}

	writing_file.write(writing_text.c_str(), writing_text.size());
	writing_file.close();

}
void Tetris::FieldMapImport(std::string file_name, MapObjects* field)
{
	std::string str_line;
	vector<std::string> vec_str;

	//ファイルを開く
	std::ifstream ifs(file_name);
	if (!ifs)
	{
		ifs.close();
	}
	while (getline(ifs, str_line))
	{
		if (str_line.find("#FIELD") != std::string::npos)
		{
			for (int y = 0; getline(ifs, str_line) && y < Tetris::FIELD_HEIGHT; y++)
			{
				vec_str = split(str_line, ',');

				for (int x = 0; x < Tetris::FIELD_WIDTH; x++)
				{
					field[y * Tetris::FIELD_WIDTH + x] = (MapObjects)stoi(vec_str[x]);
				}
			}
			break;
		}
	}


}
//void Tetris::FieldMapImport(std::string file_name, MapObjects* field, ObjPracticeOrder* order)
//{
//	std::string str_line;
//	vector<std::string> vec_str;
//
//	//ファイルを開く
//	std::ifstream ifs(file_name);
//	if (!ifs)
//	{
//		ifs.close();
//	}
//
//	for (int y = 0; getline(ifs, str_line) && y < Tetris::FIELD_HEIGHT; y++)
//	{
//		vec_str = split(str_line, ',');
//
//		for (int x = 0; x < Tetris::FIELD_WIDTH; x++)
//		{
//			field[y * Tetris::FIELD_WIDTH + x] = (MapObjects)stoi(vec_str[x]);
//		}
//	}
//
//}