#include "WinInputs.h"
using namespace GameL;

int   CWinInputs::m_x,CWinInputs::m_y;		//マウス座標
bool  CWinInputs::m_ButtonL;				//マウスクリック
bool  CWinInputs::m_ButtonR;
BYTE  CWinInputs::m_KeyBoard[256];			//キーボード配列
bool  CWinInputs::m_key_flag;


void CWinInputs::Init()
{
	m_x=0;
	m_y=0;
	m_ButtonL=false;
	m_ButtonR=false;
	m_key_flag=false;
	memset(m_KeyBoard,0x00,sizeof(m_KeyBoard));
}

bool CWinInputs::WmInput(HWND hWnd,UINT* uMsg, LPARAM* lParam)
{
	if(*uMsg== WM_MOUSEMOVE )
	{
		POINT point={ LOWORD(*lParam), HIWORD((*lParam))};
		
		m_x = point.x;   // カーソルの x 座標
		m_y = point.y;   // カーソルの y 座標
		return true;
	}
	else if(*uMsg==WM_LBUTTONDOWN)
	{
		m_ButtonL=true;
	}
	else if(*uMsg==WM_LBUTTONUP)
	{
		m_ButtonL=false;
	}
	else if(*uMsg==WM_RBUTTONDOWN)
	{
		m_ButtonR=true;
	}
	else if(*uMsg==WM_RBUTTONUP)
	{
		m_ButtonR=false;
	}
	return false;
}

bool CWinInputs:: GetVKey(int v_key)
{	
	if (GetAsyncKeyState(v_key) & 0x8000)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}
bool CWinInputs::GetVKeyOnce(int v_key)
{
	if (GetAsyncKeyState(v_key) & 0x8000)
	{
		if (m_key_flag == true)
		{
			return true;
		}
		m_key_flag = false;
	}
	else
	{
		m_key_flag = true;
		return false;
	}
	return false;
}

//キー入力で1度しか実行しない関数を模索中。
//関数が実行されるたびにコッチに来ているため、
//変数の記憶はできない。

//仕様

bool CWinInputs::GetMouButtonL()
{
	return m_ButtonL;
}
bool CWinInputs::GetMouButtonR()
{
	return m_ButtonR;
}

int CWinInputs:: GetPosX()
{
	return m_x;
}

int CWinInputs::GetPosY()
{
	return m_y;
}
