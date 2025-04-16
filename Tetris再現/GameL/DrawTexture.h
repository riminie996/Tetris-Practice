#pragma once

//STL�f�o�b�O�@�\��OFF�ɂ���
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  


//���������[�N���o�p
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <memory>
#include <vector>
#include <algorithm>
#include <D3D11.h> 
#include <d3dCompiler.h>
#include "../main.h"
#include <string>


//using namespace std;
using std::vector;
using std::shared_ptr;

namespace GameL
{
	//�e�N�X�`���C���[�W�T�C�Y�@�W����-�ݒ�
	enum TEX_SIZE
	{
		TEX_SIZE_64   =	  64,
		TEX_SIZE_128  =	 128,
		TEX_SIZE_256  =	 256,
		TEX_SIZE_512  =	 512,
		TEX_SIZE_1024 =	1024,
	};

	//�Q�c�`���pRECT��FLOAT�^
	struct RECT_N
	{
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	/*	RECT_F(float x, float y, float width, float height) : m_x(x), m_y(y), m_width(width), m_height(height) {}*/
	};
	struct RECT_F
	{
		float m_x;
		float m_y;
		float m_width;
		float m_height;
		/*	RECT_F(float x, float y, float width, float height) : m_x(x), m_y(y), m_width(width), m_height(height) {}*/
	};
	//�F
	struct COLOR
	{
		float r;
		float g;
		float b;
	};

	// �u�����h���[�h���
	enum E_BLEND_MODE
	{
		NONE,           // �u�����h�Ȃ�
		NORMAL,         // �ʏ�(�A���t�@�u�����h)
		ADDITION,       // ���Z
		ADDITIONALPHA,  // ���Z(���߂���)
		SUBTRACTION,    // ���Z
		SCREEN,         // �X�N���[��
		BLEND_NUMMAX,
	};

	enum E_REFERENCE
	{
		ref_Top,
		ref_Bottom,
		ref_Left,
		ref_Right,
		ref_Center,
	};

	//�e�N�X�`���ۑ��p�N���X
	class C_TEX_DATA
	{
		public :
			C_TEX_DATA();
			~C_TEX_DATA();

			ID3D11ShaderResourceView** GetTexData();
			float	GetTexSizeH();
			float	GetTexSizeW();
			void SetTexData(ID3D11ShaderResourceView* tex);
			void SetTexSize(float  size_w,float size_h);
		private:
			ID3D11ShaderResourceView* m_pSRV;
			float 	m_tex_size_w;
			float	m_tex_size_h;
	};
	namespace Color
	{
		const COLOR White = { 1.0f,	1.0f,	1.0f };
		const COLOR Black = { 0.0f,	0.0f,	0.0f };
		const COLOR Red =	{ 1.0f,	0.0f,	0.0f };
		const COLOR Green = { 0.0f,	1.0f,	0.0f };
		const COLOR Blue =	{ 0.0f,	0.0f,	1.0f };
		const COLOR NavyBlue =	{ 0.0f,	0.0f,	0.5f };
		const COLOR TabacoIro = { 0.525f,0.407f,0.294f };
		const COLOR Cyan =	{ 0.0f,	1.0f,	1.0f };
		const COLOR Magenta =	{ 1.0f,	0.0f,	1.0f };
		const COLOR Yellow =	{ 1.0f,	1.0f,	0.0f };
	}

	class CTexture
	{
	public:
		CTexture();
		~CTexture();
		void LoadTexture(std::string path);
		void Draw(int x,int y);
		void Draw(int x,int y, E_REFERENCE ref_x, E_REFERENCE ref_y);
		void Draw(int x,int y,RECT_N rectangle);
		void Draw(int x,int y,RECT_N rectangle, E_REFERENCE ref_x, E_REFERENCE ref_y);
		float m_opacity;
		float m_scale_x;
		float m_scale_y;
		float m_rotation_z;
		bool m_flip_x;
		bool m_flip_y;
		COLOR m_color;
		E_BLEND_MODE m_blend;

		ID3D11ShaderResourceView** GetTexData();
		int	GetTexSizeH() { return m_tex_size_h; }
		int	GetTexSizeW() { return m_tex_size_w; }
		void SetTexData(ID3D11ShaderResourceView* tex);
		void SetTexSize(float  size_w, float size_h);

	private:
		ID3D11ShaderResourceView* m_pSRV;
		int 	m_tex_size_w;
		int	m_tex_size_h;
		bool m_load_success;
	};

	//�e�N�X�`���ǂݍ��݁E�\���E�폜�N���X
	typedef class CDrawTexture
	{
		public :
			//�`��p�I�u�W�F�N�g�j��
			static void DeleteDrawTexture();
			static void InitDraw(ID3D11Device* p_device,ID3D11DeviceContext* p_device_context,int w,int h,int img_max);	//������
			static void LoadImage(const wchar_t* name,int id,TEX_SIZE hw);			//�O���t�B�b�N�ǂݍ��݁@�T�C�Y�͒萔�Ŏw��
			static void DeleteImage();											//�O���t�B�b�N�j��
			static void Draw(int id,RECT_F* src,RECT_F* dst,float col[4],float r); //�o�^�e�N�X�`���`��
			static void Draw(int x, int y);
			static void Draw(int x, int y, RECT rectangle);
			static void Draw(int id, float x, float y, bool flipW = false, bool flipH = false);
			static void Draw(int id, float x, float y, RECT_F rect_f, bool flipW=false, bool flipH = false);
			static void RightDraw(int id, float x, float y, RECT_F rect_f, bool flipW=false, bool flipH = false);
			static void CenterDraw(int id, float x, float y,  bool flipW = false, bool flipH = false);
			static void CenterDraw(int id, float x, float y, RECT_F rect_f, bool flipW = false, bool flipH = false);
			static void LeftCenterDraw(int id, float x, float y, RECT_F rect_f, bool flipH = false);
			static void TopCenterDraw(int id, float x, float y, RECT_F rect_f, bool flipH = false);
			static void BottomCenterDraw(int id, float x, float y, RECT_F rect_f, bool flipH = false);
			static void DrawStr(ID3D11ShaderResourceView* ptex_res_view,float x,float y,float size,float col[4]);//�����`��
			static void DrawHitBox(float x,float y,float h,float w,float col[4]);//�����蔻��`��
			//�`��̏���ύX�ł���֐�--------------
			static void SetOpacity(int id, float alpha);
			static void SetScaleX(int id, float scale);
			static void SetScaleY(int id, float scale);
			static void SetRotationZ(int id, float rotate);
			static void SetColor(int id, COLOR color);
			static void SetBlend(int id, E_BLEND_MODE mode);
			static void InfoReset();
			static int GetTexSizeW(int id);
			static int GetTexSizeH(int id);

			static void Set2DDraw();	//�QD�g�p�ݒ�

			//�f�o�C�X
			static ID3D11DeviceContext* m_pDeviceContext;
			static ID3D11Device*		m_pDevice; 

			//�V�F�[�_�֌W
			static ID3D11VertexShader* m_pVertexShader;		//�o�[�e�b�N�X�V�F�[�_�[
			static ID3D11PixelShader*  m_pPixelShader;		//�s�N�Z���V�F�[�_�[
			static ID3D11SamplerState* m_pSampleLinear;		//�e�N�X�`���[�T���v���[
			static ID3D11InputLayout*  m_pVertexLayout;		//���_���̓��C�A�E�g
			//�o�b�t�@
			static ID3D11Buffer* m_pVertexBuffer;			//�Q�c�|���S���p�o�[�e�B�N�X�o�b�t�@
			static ID3D11Buffer* m_pConstantBuffer;			//�A�v�������V�F�[�_�[�˂��� �R���X�^���g�o�b�t�@
			static ID3D11Buffer* m_pIndexBuffer;			//�C���f�b�N�X�o�b�t�@
			static ID3D11BlendState* m_pBlendState[BLEND_NUMMAX];

			//�E�B���h�E���
			static int m_width ;
			static int m_height; 
		private:
			//�`����
			static float m_opacity[SCENE_IMG_MAX];
			static float m_scale_x[SCENE_IMG_MAX];
			static float m_scale_y[SCENE_IMG_MAX];
			static float m_rotation_z[SCENE_IMG_MAX];
			static COLOR m_color[SCENE_IMG_MAX];
			static E_BLEND_MODE m_mode[SCENE_IMG_MAX];


			//�e�N�X�`�����
			static vector<shared_ptr<C_TEX_DATA>> vec_tex_data;

			//�擾�C���[�W�ő吔
			static int m_img_max;

	}Draw;

};