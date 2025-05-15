#pragma once
#include "Tetris.h"

#include <list>
#include <vector>
#include <string>

class ObjPracticeOrder/* :public CObj*/
{
public:
	ObjPracticeOrder();
	~ObjPracticeOrder(){}
	void NormaLinesSub(int sub_lines);
	MINO_TYPE GetMinoListFront();
	void RandomFieldImport(MapObjects* field);
	//void Init(){}
	//void Action(){}
	//void Draw(){}
private:
	std::list<MINO_TYPE> m_list_order_mino;
	std::vector<std::string> m_vec_field_path;
	int m_norma_lines;

	void SetFailure();
	void SetClear();
};