

//STLデバッグ機能をOFFにする
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  

//便利な関数置き場
#include "Function.h"
#include "main.h"
#include <math.h>
#include <sstream>
#include <fstream>


//LoadDirectory
#include <filesystem>

//渡された値の桁数を返す関数
int numOfDigit(int num)
{
	int digit = 1;
	//一桁からスタート。
	//numが10以上の場合カウントを増やすが、
	//9の場合はfor文を通らなく1桁なので。
	while ( num >= 10 )
	{
		//1, 値を10で割る 一の位は切り捨て 
		//2, 桁数を1つ増やす 

		num /= 10;
		digit++;
	}
	return digit;
}

//決められた幅から抜けてしまった場合、強制的にそこで止める
//主にメニューのカーソルなどで使うかな...
//minは最少数だが、一応maxとminを比べておく。

//返り値　調整された値
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

//画面外判定を行う
//画面内の場合、trueを返す。
bool IntheScreen(float x, float y, float sizeX, float sizeY)
{
	//0以下の場合だとうまくいかないので、念のため
	//テクスチャサイズ×2の値以下になったとき
	if (x > -(sizeX * 2) && x < WINDOW_SIZE_W + sizeX &&
		y > -(sizeY * 2) && y < WINDOW_SIZE_H + sizeY)
	{
		return true;
	}
	return false;
}

//二点間距離
//座標　X Y 距離X 距離Y
float DistanceF(float fx, float fy, float sx, float sy)
{
	return sqrtf((fx - sx) * (fx - sx) + (fy - sy) * (fy - sy));
}

//小数点第一位から四捨五入します。
int RoundingFirstDecimal(float num)
{
	int inte = (int)num;

	//1.57 - 1 = 0.57
	if (num - inte < 0.5f)
	{
		//切り捨て
		return inte;
	}
	else
	{
		//繰り上げ
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

	//ファイルを開く
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
	ofn.lStructSize = sizeof(OPENFILENAME);		//構造体のサイズ
	ofn.hwndOwner = NULL;						//オーナーウィンドウのハンドル
	ofn.lpstrFilter = L"全てのファイル (*.*)\0*.*\0"; //拡張子フィルター
	ofn.nFilterIndex = 0;			//フィルターの初期値
	ofn.lpstrFile = filePath;		//選択したファイルパスを受け取るバッファ
	ofn.nMaxFile = MAX_PATH;		//↑のバッファサイズ
	ofn.lpstrInitialDir = initial_path.c_str();
	if (GetOpenFileName(&ofn)) {
		//	MessageBox(NULL, filePath, L"情報", MB_OK);
		return wchar_to_string(filePath);
	}
	else {
		return "";
	}
}
	// 1024文字まで対応
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