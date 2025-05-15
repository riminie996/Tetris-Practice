#include "ObjPracticeOrder.h"
#include "GameL\SceneManager.h"
#include "ObjClearText.h"
#include "GameHead.h"
#include "Function.h"
#include <random>
#include <fstream>
ObjPracticeOrder::ObjPracticeOrder()
{
	m_vec_field_path = LoadDirectory("Field/Spin/",".txt");
}
MINO_TYPE ObjPracticeOrder::GetMinoListFront()
{
	if (!m_list_order_mino.empty())
	{
		MINO_TYPE mino = m_list_order_mino.front();
		m_list_order_mino.pop_front();
		return mino;
	}
	else
	{
		return Mino_Empty;
	}
}

void ObjPracticeOrder::NormaLinesSub(int sub_lines)
{
	m_norma_lines -= sub_lines;
	if (sub_lines == 0)
	{
		SetFailure();
	}
	if (m_norma_lines == 0)
	{
		SetClear();
	}
}

void ObjPracticeOrder::SetClear()
{
	ObjClearText* clear = (ObjClearText*)Objs::GetObj(OBJ_CLEARTEXT);
	clear->ClearAnimeStart();

	ObjBlock* block = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	//block->Restart();
}
void ObjPracticeOrder::SetFailure()
{
	ObjClearText* clear = (ObjClearText*)Objs::GetObj(OBJ_CLEARTEXT);
	clear->FailureAnimeStart();

	ObjBlock* block = (ObjBlock*)Objs::GetObj(OBJ_BLOCK);
	block->SetGameOverFlag(true);
	//block->Restart();
}

void ObjPracticeOrder::RandomFieldImport(MapObjects* field)
{
	std::random_device rand;
	std::string field_path = m_vec_field_path[rand() % m_vec_field_path.size()];
		std::string str_line;

	//ファイルを開く
	std::ifstream ifs(field_path);
	if (!ifs)
	{
		ifs.close();
	}

	while (getline(ifs, str_line))
	{
		if (str_line.find("MINO:") != std::string::npos)
		{
			str_line = str_line.substr(5);
			for (int i = 0; i < str_line.size(); i++)
			{
				MINO_TYPE mino = Tetris::CharToMino(str_line[i]);
				if (mino != Mino_Empty)
					m_list_order_mino.push_back(mino);
			}
		}
		else if(str_line.find("LINES:") != std::string::npos)
		{
			str_line = str_line.substr(6);
			m_norma_lines = std::stoi(str_line);
		}
	}
	FieldMapImport(field_path, field);
}