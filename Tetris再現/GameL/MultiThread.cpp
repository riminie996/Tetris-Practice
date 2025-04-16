#include <process.h>
#include "MultiThread.h"
#include <algorithm>

using namespace GameL;
using std::atomic;
atomic<bool>  execution;							//スレッド実行フラグ
thread*		  CMultiThread::m_handoru_game;			//ゲームスレッド
//std::vector<thread*>	  CMultiThread::m_vec_handle;			//ゲームスレッド



CMultiThread::~CMultiThread()
{
	////スレッド終了確認
//	m_handoru_game->join();


}
bool CMultiThread::GetEnd()
{
	return  execution.load(std::memory_order_acquire);
}

//初期化
void CMultiThread::Init(void(*game_sub)())
{
	m_handoru_game = new thread(GameThread, (void*)game_sub);
//	m_list_handle = new std::list<std::shared_ptr<thread>>;
//	AddThread(m_handoru_game);
	execution.store(true);
}
void CMultiThread::AddThread(thread* thr)
{
////	m_vec_handle.push_back(thr);
}
void CMultiThread::DeleteThread(thread* thr)
{
//	auto itr = std::find(m_vec_handle.begin(), m_vec_handle.end(), thr);
	//if ((*itr)->joinable())
	//(*itr)->join();
//	delete (*itr);
//	m_vec_handle.erase(itr);
}

//削除
void CMultiThread::Delete()
{
	//スレッド終了フラグを立てる
	End();


	//for (auto itr = m_vec_handle.begin(); itr != m_vec_handle.end(); itr++)
	//{
	//	(*itr)->join();

	//	if (!(*itr)->joinable())
	//	{
	//		delete (*itr);
	//	}
	//}
	m_handoru_game->join();
	delete m_handoru_game;
	m_handoru_game = nullptr;

	//m_vec_handle.clear();
	//m_vec_handle.shrink_to_fit();
}

//ゲームスレッド停止メソッド
void CMultiThread::End()
{	
	execution.store(false);	
}

//ゲームスレッド用メソッド
unsigned __stdcall CMultiThread::GameThread(void *p)
{
	execution.store(true);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	

	void (*sub)() = (void(*)()) p;
	

	//ゲームループ
	while(execution.load(std::memory_order_acquire))
	{
		sub();
	}

	CoUninitialize();//ＣＯＭ解除
    return 0;
}

//ゲームスレッド用メソッド
unsigned __stdcall CMultiThread::GameSubThread(void* p)
{
	//execution.store(true);
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);


	//void (*sub)() = (void(*)()) p;


	////ゲームループ
	//while (execution.load(std::memory_order_acquire))
	//{
	//	sub();
	//}

	//CoUninitialize();//ＣＯＭ解除
	return 0;
}