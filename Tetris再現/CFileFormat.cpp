#include "CFileFormat.h"

#include <fstream>
#include <sstream>


using namespace std;

list<string> CFileFormat::LoadFile(string file_name)
{
	list<string> str_split_command;
	//パスが存在しない
//	if (!PathExist(file_name))return str_split_command;


	//ファイルを開く
	ifstream ifs(file_name);
	if (!ifs)
	{
		//失敗
		ifs.close();
		return str_split_command;
	}

	string str_command;
	while (getline(ifs, str_command))
	{
		str_split_command.push_back(str_command);
	}

	return str_split_command;
}