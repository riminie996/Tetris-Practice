

//STL�f�o�b�O�@�\��OFF�ɂ���
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  

//�֗��Ȋ֐��u����
#include "Function.h"
#include "main.h"
#include <math.h>
#include <sstream>
#include <fstream>


//LoadDirectory
#include <filesystem>

//�n���ꂽ�l�̌�����Ԃ��֐�
int numOfDigit(int num)
{
	int digit = 1;
	//�ꌅ����X�^�[�g�B
	//num��10�ȏ�̏ꍇ�J�E���g�𑝂₷���A
	//9�̏ꍇ��for����ʂ�Ȃ�1���Ȃ̂ŁB
	while ( num >= 10 )
	{
		//1, �l��10�Ŋ��� ��̈ʂ͐؂�̂� 
		//2, ������1���₷ 

		num /= 10;
		digit++;
	}
	return digit;
}

//���߂�ꂽ�����甲���Ă��܂����ꍇ�A�����I�ɂ����Ŏ~�߂�
//��Ƀ��j���[�̃J�[�\���ȂǂŎg������...
//min�͍ŏ��������A�ꉞmax��min���ׂĂ����B

//�Ԃ�l�@�������ꂽ�l
float Min_Max(float position, float min, float max)
{
	if (min > max)
	{
		int a = min;
		min = max;
		max = a;
	}
	
	if (position < min) return min;
	if (position > max) return max;

	return position;
}
int Min_Max(int position, int min, int max)
{
	if (min > max)
	{
		int a = min;
		min = max;
		max = a;
	}

	if (position < min) return min;
	if (position > max) return max;

	return position;
}

//��ʊO������s��
//��ʓ��̏ꍇ�Atrue��Ԃ��B
bool IntheScreen(float x, float y, float sizeX, float sizeY)
{
	//0�ȉ��̏ꍇ���Ƃ��܂������Ȃ��̂ŁA�O�̂���
	//�e�N�X�`���T�C�Y�~2�̒l�ȉ��ɂȂ����Ƃ�
	if (x > -(sizeX * 2) && x < WINDOW_SIZE_W + sizeX &&
		y > -(sizeY * 2) && y < WINDOW_SIZE_H + sizeY)
	{
		return true;
	}
	return false;
}

//��_�ԋ���
//���W�@X Y ����X ����Y
float DistanceF(float fx, float fy, float sx, float sy)
{
	return sqrtf((fx - sx) * (fx - sx) + (fy - sy) * (fy - sy));
}

//�����_���ʂ���l�̌ܓ����܂��B
int RoundingFirstDecimal(float num)
{
	int inte = (int)num;

	//1.57 - 1 = 0.57
	if (num - inte < 0.5f)
	{
		//�؂�̂�
		return inte;
	}
	else
	{
		//�J��グ
		return inte + 1;
	}


}
std::vector<std::string> split(std::string& input, char delimiter)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

void DrawFill(int tex, GameL::RECT_F rectangle)
{
	for (float y = 0.0f; y < WINDOW_SIZE_H; y += rectangle.m_height)
	{
		for (float x = 0.0f; x < WINDOW_SIZE_W; x += rectangle.m_width)
		{
			GameL::Draw::Draw(tex, x, y, rectangle);
		}
	}
}

bool stob(std::string str)
{
	if (str == "true")return true;
	if (str == "1")return true;
	return false;
}

bool PathExist(std::string path)
{
	struct _stat s;
	int result = _stat(path.c_str(), &s);

	return result == 0 ? true : false;
}
void FieldMapExport(MapObjects field[Tetris::FIELD_HEIGHT][Tetris::FIELD_WIDTH])
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
void FieldMapImport(std::string file_name, MapObjects* field)
{
	std::string str_line;
	vector<std::string> vec_str;

	//�t�@�C�����J��
	std::ifstream ifs(file_name);
	if (!ifs)
	{
		ifs.close();
	}

	for (int y = 0;getline(ifs, str_line) && y < Tetris::FIELD_HEIGHT;y++)
	{
		vec_str = split(str_line, ',');

		for (int x = 0; x < Tetris::FIELD_WIDTH; x++)
		{
			field[y * Tetris::FIELD_WIDTH + x] = (MapObjects)stoi(vec_str[x]);
		}
	}

}

std::string OpenFileDialog(std::wstring initial_path)
{
	WCHAR filePath[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);		//�\���̂̃T�C�Y
	ofn.hwndOwner = NULL;						//�I�[�i�[�E�B���h�E�̃n���h��
	ofn.lpstrFilter = L"�S�Ẵt�@�C�� (*.*)\0*.*\0"; //�g���q�t�B���^�[
	ofn.nFilterIndex = 0;			//�t�B���^�[�̏����l
	ofn.lpstrFile = filePath;		//�I�������t�@�C���p�X���󂯎��o�b�t�@
	ofn.nMaxFile = MAX_PATH;		//���̃o�b�t�@�T�C�Y
	ofn.lpstrInitialDir = initial_path.c_str();
	if (GetOpenFileName(&ofn)) {
		//	MessageBox(NULL, filePath, L"���", MB_OK);
		return wchar_to_string(filePath);
	}
	else {
		return "";
	}
}
	// 1024�����܂őΉ�
std::string wchar_to_string(const wchar_t* in_wstr)
{
	size_t convertedChars = 0;
	char str_c[1024];
	wcstombs_s(&convertedChars, str_c, sizeof(str_c), in_wstr, _TRUNCATE);
	std::string out_str = str_c;
	return out_str;
}

std::vector<std::string> LoadDirectory(std::string path)
{
	return LoadDirectory(path, "");
}
std::vector<std::string> LoadDirectory(std::string path, std::string find_str)
{
	std::vector<std::string> vec_path;
	using std::filesystem::recursive_directory_iterator;



	for (const auto& file : recursive_directory_iterator(path))
	{
		if (file.path().string().find(find_str) != -1 || find_str == "")
		{
			vec_path.push_back(file.path().string());
		}

	}
	return vec_path;
}