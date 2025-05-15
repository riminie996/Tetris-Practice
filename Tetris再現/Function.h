#pragma once
#include <vector>
#include <string>
#include <list>
#include "Tetris.h"
#include "GameHead.h"
#include "Structure.h"
//便利な関数置き場
#define USER_DATA ((UserData*)Save::GetData())


//与えられた数値の桁数を調べる関数
int numOfDigit(int num);

//決められた幅から抜けてしまった場合、強制的にそこで止める
int Min_Max(int position, int min, int max);
float Min_Max(float position, float min, float max);

//画面外判定をする。
//主に描画処理をスキップするときに使う。 基本テクスチャサイズ:64
bool IntheScreen(float x, float y, float sizeX = 64, float sizeY = 64);

//二点間距離を求める
float DistanceF(float fx, float fy, float sx, float sy);


//小数点第一位から四捨五入します。
int RoundingFirstDecimal(float num);

int RoundingFirstDigit(float num);

std::vector<std::string> split(std::string& input, char delimiter);

void DrawFill(int tex, GameL::RECT_F rectangle);

const float PI = 3.141592f;

bool stob(std::string str);

bool PathExist(std::string path);

std::string OpenFileDialog(std::wstring initial_path);
// 1024文字まで対応
std::string wchar_to_string(const wchar_t* in_wstr);

std::vector<std::string> LoadDirectory(std::string path);
std::vector<std::string> LoadDirectory(std::string path, std::string find_str);
void ColorScreenDraw(GameL::COLOR c,float opacity);