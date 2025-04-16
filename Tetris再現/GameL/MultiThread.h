#pragma once
//STLデバッグ機能をOFFにする
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0) 

#include <list>
#include <thread>
#include <vector>
#include <atomic>
#include <Windows.h>

using std::thread;
//using namespace std;

//メモリリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define new DBG_NEW
#endif  // _DEBUG
namespace GameL
{
	typedef class CMultiThread
	{
		public:
			~CMultiThread();
			static void Init(void(*game_sub)());
			static void AddThread(thread* thr);
			static void DeleteThread(thread* thr);
			static void Delete();
			static void End();
			static bool GetEnd();
		private:

			static unsigned __stdcall GameThread(void *p);	//ゲームスレッド用メソッド
			static unsigned __stdcall GameSubThread(void *p);	//ゲームスレッド用メソッド
			
			static thread*		 m_handoru_game;			//ゲームスレッド
		//	static std::vector<std::thread*> m_vec_handle;
	}GameLoop;
};