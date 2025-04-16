#pragma once

//STL�f�o�b�O�@�\��OFF�ɂ���
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  

#include <memory>
#include <list>
#include <string>
#include <algorithm>
#include <D3D11.h> 
#include <d3dCompiler.h>

using std::list;
using std::unique_ptr;
using std::string;

//using namespace std;

namespace GameL
{

	//�t�H���g�ꕶ���̃e�N�X�`�����
	class CFontCharTex
	{
		friend class CDrawFont;
		public:
			CFontCharTex();
			~CFontCharTex();
		private:
			wchar_t					  m_wc;				//�g�p����Ă��镶��(�����p)
			string					m_str;
			ID3D11Texture2D*		  m_pTexture;		//�����̃e�N�X�`�����
			ID3D11ShaderResourceView* m_pTexResView;	//�e�N�X�`�����V�F�[�_�ɑ�������
	};
	struct COLOR_A
	{
		float r;
		float g;
		float b;
		float a;
	};
	namespace ColorA
	{
		const COLOR_A White =	  { 1.0f,	1.0f,	1.0f,	1.0f};
		const COLOR_A Black =	  { 0.0f,	0.0f,	0.0f,	1.0f};
		const COLOR_A Red =		  { 1.0f,	0.0f,	0.0f,	1.0f};
		const COLOR_A Yellow =	  { 1.0f,	1.0f,	0.0f,	1.0f};
		const COLOR_A Green =	  { 0.0f,	1.0f,	0.0f,	1.0f};
		const COLOR_A Blue =	  { 0.0f,	0.0f,	1.0f,	1.0f};
		const COLOR_A NavyBlue =  { 0.0f,	0.0f,	0.5f,	1.0f};
		const COLOR_A TabacoIro = { 0.525f,	0.407f,	0.294f,	1.0f};
	}
	enum FONT_SIZE
	{
		FONT_SIZE_32 = 32,
		FONT_SIZE_64 = 64,
		FONT_SIZE_128 = 128,
		FONT_SIZE_256 = 256,
		FONT_SIZE_512 = 512,
		FONT_SIZE_1024 = 1024,
	};

	//�t�H���g�\���N���X
	typedef class CDrawFont
	{
		public:
			static void Init(ID3D11Device* p_device,ID3D11DeviceContext* p_device_context);	//������
			static void Delete();														//�N���X�폜
			static void SetStrTex(const wchar_t* init_str);									//���O�\�񕶎����X�g�쐬
			static void ListDelete();													//�������X�g�폜
			static void StrDraw(const wchar_t* str,float x,float y,float size,float color[4]);	//������`��
			static void StrCenterDraw(const wchar_t* str, float x, float y, float size, float color[4]);//�����񒆉���`��
			static void StrDraw(const wchar_t* str, float x, float y, float size, COLOR_A color);	//������`��
			static void StrRightDraw(const wchar_t* str, float x, float y, float size, COLOR_A color);	//������`��
			static void StrCenterDraw(const wchar_t* str, float x, float y, float size, COLOR_A color);//�����񒆉���`��
			//static void StrDraw(string str, float x, float y, float size, COLOR_A color);	//������`��

		private:
			static list< unique_ptr<CFontCharTex>>::iterator SetChar(wchar_t c);		//�t�H���g���X�g�ǉ����\�b�h
		
			static HFONT		m_hFont;
			static HDC			m_hdc;
			static HFONT		m_oldFont;
			static TEXTMETRIC	m_TM;
 		
			static ID3D11Device*		m_pDevice;
			static ID3D11DeviceContext* m_pDeviceContext;
			static D3D11_TEXTURE2D_DESC m_desc;

			//�t�H���g�����i�[���X�g
			static list< unique_ptr<CFontCharTex>>*	m_ListData;		//�������X�g
	}Font;

};