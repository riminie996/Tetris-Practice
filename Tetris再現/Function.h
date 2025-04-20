#pragma once
#include <vector>
#include <string>
#include <list>
#include "Tetris.h"
#include "GameHead.h"
#include "Structure.h"
//�֗��Ȋ֐��u����
#define USER_DATA ((UserData*)Save::GetData())


//�^����ꂽ���l�̌����𒲂ׂ�֐�
int numOfDigit(int num);

//���߂�ꂽ�����甲���Ă��܂����ꍇ�A�����I�ɂ����Ŏ~�߂�
int Min_Max(int position, int min, int max);
float Min_Max(float position, float min, float max);

//��ʊO���������B
//��ɕ`�揈�����X�L�b�v����Ƃ��Ɏg���B ��{�e�N�X�`���T�C�Y:64
bool IntheScreen(float x, float y, float sizeX = 64, float sizeY = 64);

//��_�ԋ��������߂�
float DistanceF(float fx, float fy, float sx, float sy);


//�����_���ʂ���l�̌ܓ����܂��B
int RoundingFirstDecimal(float num);

int RoundingFirstDigit(float num);

std::vector<std::string> split(std::string& input, char delimiter);

void DrawFill(int tex, GameL::RECT_F rectangle);

const float PI = 3.141592f;

bool stob(std::string str);

bool PathExist(std::string path);

void FieldMapExport(MapObjects field[Tetris::FIELD_HEIGHT][Tetris::FIELD_WIDTH]);
void FieldMapImport(std::string file_name, MapObjects* field);
std::string OpenFileDialog(std::wstring initial_path);
// 1024�����܂őΉ�
std::string wchar_to_string(const wchar_t* in_wstr);

std::vector<std::string> LoadDirectory(std::string path);
std::vector<std::string> LoadDirectory(std::string path, std::string find_str);
void ColorScreenDraw(GameL::COLOR c,float opacity);