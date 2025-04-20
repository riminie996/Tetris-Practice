#pragma once 

//使用するヘッダーファイル
#include "GameL\SceneManager.h"
#include "CCounter.h"

//使用するネームスペース
using namespace GameL;

//シーンメイン
class SceneMain :public CScene
{
public:
	SceneMain() ;
	~SceneMain() ;
	void InitScene() ;//初期化
	void Scene();//実行中

private:
	
};