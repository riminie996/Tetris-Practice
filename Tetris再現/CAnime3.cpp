
#include "CAnime3.h"
#include <list>

#include <fstream>
#include <sstream>
#include <math.h>
 

namespace Rimi
{
	const float PI = 3.14f;
}

using namespace std;
CAnime3::CAnime3()
{
	m_flag_start = false;
//	m_vec_anime.resize(AnimeTypeCounter);
	Init();
}

CAnime3::CAnime3(string file_path)
{
	m_flag_start = false;
//	m_vec_anime.resize(AnimeTypeCounter);


	LoadData(file_path);

	Init();
}

void CAnime3::LoadData(string file_path)
{
	list<string> list_str = LoadFile(file_path + Rimi::FILE_FORMAT_ANIME);

	if (list_str.size() == 0)return;
	
	ST_ANIME anime;
	int now_anime_type;
	float max_time = 0.0f;

	for (auto itr = list_str.begin(); itr != list_str.end(); itr++)
	{
		string str_split;

		if (itr->find("ENTRY_NAME:") != -1)
		{
			str_split = itr->substr(11);
			ST_ITEM item;
			item.entry_name = str_split;
			m_anime.push_back(item);
		}
		else if (itr->find("MAX_TIME:") != -1)
		{
			str_split = itr->substr(9);
			max_time = stof(str_split) * 1000;//小数点からmsに変換
		}
		else if (itr->find("MAX_FRAME:") != -1)
		{
			str_split = itr->substr(10);
			max_time = stof(str_split);
		}
		else if (itr->find("TIME:") != -1)
		{
			str_split = itr->substr(5);
			anime.frame = stof(str_split) * 1000;//小数点からmsに変換
		}
		else if (itr->find("FRAME:") != -1)
		{
			str_split = itr->substr(6);
			anime.frame = stof(str_split);
		}
		else if (itr->find("PARAM:") != -1)
		{
			str_split = itr->substr(6);
			anime.value = stof(str_split);
		}
		else if (itr->find("TYPE:") != -1)
		{
			str_split = itr->substr(5);

			for (int i = 0; i < AnimeTypeCounter; i++)
			{
				if (str_split == STR_ANIME_TYPE[i])
				{
					anime.type = (E_ANIME_TYPE)i;
					now_anime_type = i;
				}
			}

		}
		else if (itr->find("MODE:") != -1)
		{
			str_split = itr->substr(5);

			for (int i = 0; i < MoveTypeCounter; i++)
			{
				if (str_split == STR_MOVE_TYPE[i])
					anime.mode = (E_ANIME_MODE)i;
			}
		}
		//アニメ情報をエントリに追加する
		else if (itr->find("#PUSH") != -1)
		{
			m_anime.back().vec_anime[now_anime_type].push_back(anime);
		}
	}
	m_ct_time = { CTimeCounter(0,max_time,1) };
	m_ct_frame = { CCounter(0.0f,0.0f,max_time,FREE) };

	Init();
}

void CAnime3::Init()
{
	for (int i = 0; i < m_anime.size(); i++)
	{
		for (int j = 0; j < AnimeTypeCounter; j++)
		{

			if (m_anime[i].vec_anime[j].size() == 0)continue;

			float* p_value;
			switch ((E_ANIME_TYPE)j)
			{
			case POS_X:
			default:
				p_value = &m_anime[i].value.x;
				break;
			case POS_Y:
				p_value = &m_anime[i].value.y;
				break;
			case SCALE_X:
				p_value = &m_anime[i].value.scale_x;
				break;
			case SCALE_Y:
				p_value = &m_anime[i].value.scale_y;
				break;
			case COLOR_R:
				p_value = &m_anime[i].value.r;
				break;
			case COLOR_G:
				p_value = &m_anime[i].value.g;
				break;
			case COLOR_B:
				p_value = &m_anime[i].value.b;
				break;
			case ROTATE_Z:
				p_value = &m_anime[i].value.rotate_z;
				break;
			case OPACITY:
				p_value = &m_anime[i].value.opacity;
				break;
			case PATTERN:
				p_value = &m_anime[i].value.pattern;
				break;
			}

			*p_value = m_anime[i].vec_anime[j].front().value;


		}
	}
}

CAnime3::~CAnime3()
{
	m_anime.clear();
	m_anime.shrink_to_fit();

}

void CAnime3::Action()
{
	if (m_flag_start == false)return;

	m_ct_time.Action();//1フレーム文増加
	Update(m_ct_time.NowValue);

}

void CAnime3::ActionLoop()
{
	if (m_flag_start == false)return;

	m_ct_time.ActionLoop();//1フレーム文増加
	Update(m_ct_time.NowValue);

}

//減算も使えます。
void CAnime3::Add(float add)
{
	m_ct_frame.Add(add);
	if (m_ct_frame.NowValue <= m_ct_frame.MinValue)
	{
		m_ct_frame.NowValue = m_ct_frame.MinValue;
	}
	if (m_ct_frame.NowValue >= m_ct_frame.MaxValue)
	{
		m_ct_frame.NowValue = m_ct_frame.MaxValue;
	}
	Update(m_ct_frame.NowValue);
}
void CAnime3::AddLoop(float add)
{
	m_ct_frame.Add(add);

	if (m_ct_frame.NowValue < m_ct_frame.MinValue)
	{
		m_ct_frame.NowValue = m_ct_frame.MinValue - m_ct_frame.NowValue + m_ct_frame.MaxValue;
	}
	if (m_ct_frame.NowValue >= m_ct_frame.MaxValue)
	{
		m_ct_frame.NowValue = m_ct_frame.NowValue - m_ct_frame.MaxValue + m_ct_frame.MinValue;
	}


	Update(m_ct_frame.NowValue);
}

void CAnime3::Start()
{
	m_ct_time.Start(0, m_ct_time.EndValue, 1);
	m_flag_start = true;
	//全ての値をリセットする
	Init();
}

void CAnime3::End()
{
	m_flag_start = false;
}

void CAnime3::Update(float frame)
{
	for (int i = 0; i < m_anime.size(); i++)
	{
		for (int j = 0; j < AnimeTypeCounter; j++)
		{
			if (m_anime[i].vec_anime[j].size() == 0)continue;

			float* p_value;
			switch ((E_ANIME_TYPE)j)
			{
			case POS_X:
			default:
				p_value = &m_anime[i].value.x;
				break;
			case POS_Y:
				p_value = &m_anime[i].value.y;
				break;
			case SCALE_X:
				p_value = &m_anime[i].value.scale_x;
				break;
			case SCALE_Y:
				p_value = &m_anime[i].value.scale_y;
				break;
			case COLOR_R:
				p_value = &m_anime[i].value.r;
				break;
			case COLOR_G:
				p_value = &m_anime[i].value.g;
				break;
			case COLOR_B:
				p_value = &m_anime[i].value.b;
				break;
			case ROTATE_Z:
				p_value = &m_anime[i].value.rotate_z;
				break;
			case OPACITY:
				p_value = &m_anime[i].value.opacity;
				break;
			case PATTERN:
				p_value = &m_anime[i].value.pattern;
				break;
			}

			if (m_ct_frame.GetMinReached() && m_ct_frame.GetUse() && !m_ct_time.GetProgress())
			{
				*p_value = m_anime[i].vec_anime[j].front().value;
				continue;
			}
			else if ((m_ct_frame.GetMaxReached() && m_ct_frame.GetUse()) ||
				m_ct_time.GetProgress() && m_ct_time.GetEndValueReached())
			{
				*p_value = m_anime[i].vec_anime[j].back().value;
				continue;
			}

			//Actionが一度実行された場合、
			// 一度最終の値にしてから調べることで
			//最終フレームに到達している場合最後の値に設定する
			*p_value = m_anime[i].vec_anime[j].front().value;

			for (int k = 0; k < m_anime[i].vec_anime[j].size(); k++)
			{
				//次のフレームがない場合、最終値にして終了
				if (k + 1 == m_anime[i].vec_anime[j].size())
				{
					*p_value = m_anime[i].vec_anime[j].back().value;
					break;
				}

				if (frame < m_anime[i].vec_anime[j][k + 1].frame)
				{


					float next_time = m_anime[i].vec_anime[j][k + 1].frame;
					float next_value = m_anime[i].vec_anime[j][k + 1].value;


					float percent;
					//アニメーションを行う最大のフレーム数
					float now_time = frame - m_anime[i].vec_anime[j][k].frame;
					//指定フレームと次の指定フレームの差
					float time = next_time - m_anime[i].vec_anime[j][k].frame;

					//ゼロ除算を避けるため、frame数が0の時は瞬間移動する。
					if (time == 0.0f || m_anime[i].vec_anime[j][k].value == next_value)
					{
						percent = 1.0f;
					}
					else
					{
						percent = (float)now_time / (float)time;
					}

					//5 + (8 - 5)
					float value = 0.0f;

					switch (m_anime[i].vec_anime[j][k].mode)
					{
					case MOVE_NORMAL:
					case MOVE_ACC_DEC:
						value = m_anime[i].vec_anime[j][k].value +
							(next_value - m_anime[i].vec_anime[j][k].value) * percent;
						break;
					case MOVE_ACCELE:
						value = m_anime[i].vec_anime[j][k].value +
							(next_value - m_anime[i].vec_anime[j][k].value) * (1.0f - cos(percent * Rimi::PI / 2.0f));
						break;
					case MOVE_DECELE:
						value = m_anime[i].vec_anime[j][k].value +
							(next_value - m_anime[i].vec_anime[j][k].value) * sin(percent * Rimi::PI / 2.0f);
						break;
					case MOVE_TELEPORT:
						value = m_anime[i].vec_anime[j][k].value;
						break;
					}
					*p_value = value;
					//移動してから
					break;
				}


			}
		}
	}
}