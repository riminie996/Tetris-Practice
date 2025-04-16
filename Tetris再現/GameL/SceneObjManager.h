#pragma once

//STLデバッグ機能をOFFにする
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  

#include <memory>
#include <list>
#include <algorithm>
#include <Windows.h>
//メモリリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define new DBG_NEW
#endif  // _DEBUG
using std::list;
using std::shared_ptr;

//using namespace std;

namespace GameL
{
	//継承抽象用　オブジェクトクラス
	class CObj
	{
		public:		
			CObj();
			virtual ~CObj(){};
			virtual void Init()=0;		//イニシャライズ
			virtual void Action()=0;	//アクション
			virtual void Draw()=0;		//ドロー
			virtual void Delete() {}
			void		 SetPrio(unsigned int prio);	//優先順位変更
			unsigned int GetPrio();
			void		 SetName(int name);				//オブジェクト名前ID変更
			int			 GetName();
			void		 SetStatus(bool exe);			//オブジェクトの状態変更
			bool		 GetStatus();
			int			GetActTime();
			int m_act_time;
			int m_draw_time;
		private:
			unsigned int m_prio;		//描画優先順位
			int			 m_name;		//オブジェクトの名前ID
			bool		 m_status;		//オブジェクトの状態
	};

	//オブジェクトマネージャー
	typedef class CSceneObjManager
	{
		public:
			static void Init();
			static void Delete();
			static void InsertObj(CObj* obj,int name,unsigned int prio);//オブジェクト登録
			static void ObjAction();			//アクション実行
			static void ObjDraw();				//ドロー実行
			static void ListDeleteSceneObj();	//オブジェクトリスト削除命令メソッド
			static CObj* GetObj(int name);		//OBJ_NEMEからオブジェクト情報取得
			static void ResetPrio();			//優先順位変更
			static void ObjSubThrActionDraw();
		private:
			static list<shared_ptr<CObj>>* m_ListData;	//ゲーム実行リスト
	}Objs;
};
