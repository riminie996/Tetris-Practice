#pragma once
//STL�f�o�b�O�@�\��OFF�ɂ���
#define _HAS_ITERATOR_DEBUGGING	(0)

#include "CFileFormat.h"
#include "CTimeCounter.h"
#include "CCounter.h"
//#include "GameHead.h"
class CAnime3 : public CFileFormat
{ 
public:
	CAnime3();
	CAnime3(std::string file_path);
	~CAnime3();
	void Init();
	void LoadData(std::string file_path);

	//ST_ANIME_INFO Get() { return m_now_info; }
	void Action();
	void ActionLoop();
	void Add(float add);
	void AddLoop(float add);

	void Start();
	void End();
	bool GetStart() { return m_flag_start; }

	struct ST_VALUE_TYPE
	{
		float x = 0.0f, y = 0.0f, scale_x = 1.0f, scale_y = 1.0f;
		float r = 1.0f, g = 1.0f, b = 1.0f, opacity = 1.0f;
		float rotate_z = 0.0f;
		float pattern = 0;
	};

//	float GetNowFrame() { return m_now_frame; }
	ST_VALUE_TYPE GetValue(int number) 
	{
		if (number >= m_anime.size())return ST_VALUE_TYPE();

		return m_anime[number].value; 
	}
	//������Ȃ������ꍇ0��Ԃ�
	ST_VALUE_TYPE GetValue(std::string name)
	{
		int num = 0;
		for (int i = 0; i < m_anime.size(); i++)
		{
			if (m_anime[i].entry_name == name)
			{
				num = i;
				break;
			}
		}

		return m_anime[num].value;
	}
	bool GetEnd() { return m_ct_time.GetEndValueReached(); }
	bool GetMaxReached() { return m_ct_frame.GetMaxReached(); }

private:
	//�A�j���[�V�����^�C�v
	enum E_ANIME_TYPE
	{
		POS_X,
		POS_Y,
		SCALE_X,
		SCALE_Y,
		COLOR_R,
		COLOR_G,
		COLOR_B,
		ROTATE_Z,
		OPACITY,
		PATTERN,

		AnimeTypeCounter,//�^�C�v�J�E���g�p
	};

	const std::string STR_ANIME_TYPE[AnimeTypeCounter] =
	{
		"PosX",			//POS_X,
		"PosY",			//POS_Y,
		"ScaleX",		//SCALE_X,
		"ScaleY",		//SCALE_Y,
		"ColorR",		//COLOR_R,
		"ColorG",		//COLOR_G,
		"ColorB",		//COLOR_B,
		"RotateZ",		//ROTATE_Z,
		"Opacity",
		"Pattern",
	};

	enum E_ANIME_MODE
	{
		MOVE_NORMAL,//����
		MOVE_ACCELE,//����
		MOVE_DECELE,//����
		MOVE_ACC_DEC,//������
		MOVE_TELEPORT,

		MoveTypeCounter,//������ރJ�E���g�p
	};

	const std::string STR_MOVE_TYPE[MoveTypeCounter] =
	{
		"Normal",	//����
		"Accele",	//����
		"Decele",	//����
		"Acc_Dec",	//������
		"Teleport",	//�u�Ԉړ�
	};


	//vector�ŊǗ��������B
	struct ST_ANIME
	{
		ST_ANIME()
		{
			frame = 0.0f;
			mode = MOVE_NORMAL;
			value = 0.0f;
			type = POS_X;
		}
		E_ANIME_MODE mode;

		float value;
		float frame;
		E_ANIME_TYPE type;
	};

	CTimeCounter m_ct_time;
	CCounter m_ct_frame;
	bool m_flag_start;

	//ST_VALUE_TYPE m_value;//���݂̒l���L�����邽�߂̕ϐ�
	//std::vector< std::vector<ST_ANIME>> m_vec_anime;//�t���[�����Ƃ̏������邽�߂̕ϐ�

	struct ST_ITEM
	{
		std::string entry_name;
		std::vector< std::vector<ST_ANIME>> vec_anime;
		ST_VALUE_TYPE value;//���݂̒l���L�����邽�߂̕ϐ�

		ST_ITEM()
		{
			vec_anime.resize(AnimeTypeCounter);
		}
	};
	std::vector<ST_ITEM> m_anime;

	void Update(float frame);

};