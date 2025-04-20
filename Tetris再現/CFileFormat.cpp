#include "CFileFormat.h"

#include <fstream>
#include <sstream>


using namespace std;

list<string> CFileFormat::LoadFile(string file_name)
{
	list<string> str_split_command;
	//�p�X�����݂��Ȃ�
//	if (!PathExist(file_name))return str_split_command;


	//�t�@�C�����J��
	ifstream ifs(file_name);
	if (!ifs)
	{
		//���s
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