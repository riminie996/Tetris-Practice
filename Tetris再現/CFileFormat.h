#pragma once

#include "Structure.h"
#include "GameL\DrawTexture.h"
#include <list>
using namespace GameL;
struct ST_LAYOUT
{
	ST_LAYOUT()
	{
		loop_x = false;
		loop_y = false;
		pos.x = 0;
		pos.y = 0;
		rectangle.m_x = 0;
		rectangle.m_y = 0;
		rectangle.m_width = 1;
		rectangle.m_height = 1;
		reference_x = ref_Left;
		reference_y = ref_Top;
		blend_mode = GameL::E_BLEND_MODE::NORMAL;
		flip_x = false;
		flip_y = false;
		scale_x = 1.0f;
		scale_y = 1.0f;
		rotate_z = 0.0f;
	}

	std::string entry_name;
	Point pos;
	GameL::RECT_N rectangle;
	GameL::E_REFERENCE reference_x;
	GameL::E_REFERENCE reference_y;
	bool loop_x;
	bool loop_y;
	GameL::E_BLEND_MODE blend_mode;
	bool flip_x;
	bool flip_y;
	float scale_x;
	float scale_y;
	float rotate_z;
};

class CFileFormat
{
public:
	CFileFormat() {}
	~CFileFormat() {}

	std::list<std::string> LoadFile(std::string file_name);
protected:
private:


};

// Rimi - 私のニックネームの略称。
namespace Rimi
{
	//rmani = rimiのanimeファイル。
	const std::string FILE_FORMAT_ANIME = ".rmani";
	const std::string FILE_FORMAT_LAYOUT = ".rmlyt";
	const std::string FILE_FORMAT_IMAGE = ".png";

}
