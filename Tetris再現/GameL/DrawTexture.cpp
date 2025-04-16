#include <wincodec.h>


#include <gdiplus.h>	//GDI+���g�p

#include "DrawTexture.h"

#include <memory>
#include <vector>



#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace GameL;

#define SAFE_RELEASE(p)      { if (p) { (p)->Release();    (p)=nullptr;  }	}
#define SAFE_RESET(p)		 { if (p) { (p).reset(nullptr);				 }  }

float CDrawTexture::m_opacity[SCENE_IMG_MAX];
float CDrawTexture::m_scale_x[SCENE_IMG_MAX];
float CDrawTexture::m_scale_y[SCENE_IMG_MAX];
float CDrawTexture::m_rotation_z[SCENE_IMG_MAX];
COLOR CDrawTexture::m_color[SCENE_IMG_MAX];
E_BLEND_MODE CDrawTexture::m_mode[SCENE_IMG_MAX];

//�|���S���\����
struct DRAW_POLYGON
{
	float		   pos[3];	//x-y-z���_���
	unsigned int   id[2];	//�t�u�̂h�c
};

//Draw2DTex.hlsl�O���[�o��
struct DRAW_2D_TEX
{
	float size[4];		//��ʃT�C�Y
	float color[4];		//�J���[���
	float rect_in[4];	//�؂���ʒu
	float rect_out[4];	//�\��t���ʒu
	float rotation[4];	//��]���
};


//�f�o�C�X
ID3D11DeviceContext*CDrawTexture::m_pDeviceContext;
ID3D11Device*		CDrawTexture::m_pDevice; 

//�e�N�X�`�����
vector< shared_ptr<C_TEX_DATA> > CDrawTexture::vec_tex_data;

//�E�B���h�E���
int CDrawTexture::m_width ;
int CDrawTexture::m_height; 

//�擾�C���[�W�ő吔
int CDrawTexture::m_img_max;

//�V�F�[�_�֌W
ID3D11VertexShader* CDrawTexture::m_pVertexShader;		//�o�[�e�b�N�X�V�F�[�_�[
ID3D11PixelShader*  CDrawTexture::m_pPixelShader;		//�s�N�Z���V�F�[�_�[
ID3D11SamplerState* CDrawTexture::m_pSampleLinear;		//�e�N�X�`���[�T���v���[
ID3D11InputLayout*  CDrawTexture::m_pVertexLayout;		//���_���̓��C�A�E�g
//�o�b�t�@
ID3D11Buffer*		CDrawTexture::m_pVertexBuffer;		//�Q�c�|���S���p�o�[�e�B�N�X�o�b�t�@
ID3D11Buffer*		CDrawTexture::m_pConstantBuffer;	//�A�v�������V�F�[�_�[�˂��� �R���X�^���g�o�b�t�@
ID3D11Buffer*		CDrawTexture::m_pIndexBuffer;		//�C���f�b�N�X�o�b�t�@
ID3D11BlendState*		CDrawTexture::m_pBlendState[BLEND_NUMMAX];		//�C���f�b�N�X�o�b�t�@

bool CreateTextureFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
	const wchar_t*fileName, ID3D11ShaderResourceView** tex, int* w, int*  h);

ULONG_PTR	g_diplusToken;	//GDI+�I�u�W�F�N�g

//�QD�`��pHLSL������
extern const char* g_hlsl_str;

//�e�N�X�`���ۑ��p�N���X���\�b�h�S----------------
C_TEX_DATA::C_TEX_DATA()
{	
	m_pSRV			= nullptr;
	m_tex_size_h	= 128;
	m_tex_size_w    = 128;
}


C_TEX_DATA::~C_TEX_DATA()
{
	SAFE_RELEASE(m_pSRV);
	m_tex_size_h	= 128;
	m_tex_size_w    = 128;
}
CTexture::CTexture()
{
	m_pSRV = nullptr;
	m_blend = E_BLEND_MODE::NORMAL;
	m_color = Color::White;
	m_opacity = 1.0f;
	m_rotation_z = 0.0f;
	m_scale_x = 1.0f;
	m_scale_y = 1.0f;
	m_tex_size_w = 1;
	m_tex_size_h = 1;
	m_load_success = false;
	m_flip_x = false;
	m_flip_y = false;
}
CTexture::~CTexture()
{
	SAFE_RELEASE(m_pSRV);
}
ID3D11ShaderResourceView** CTexture::GetTexData()
{
	return &m_pSRV;
}

ID3D11ShaderResourceView** C_TEX_DATA::GetTexData()
{
	return &m_pSRV;
}

float C_TEX_DATA::GetTexSizeH()
{
	return m_tex_size_h;
}

float C_TEX_DATA::GetTexSizeW()
{
	return m_tex_size_w;
}

void C_TEX_DATA::SetTexData(ID3D11ShaderResourceView* tex)
{
	m_pSRV=tex;
}
void CTexture::SetTexData(ID3D11ShaderResourceView* tex)
{
	m_pSRV = tex;
}
void CTexture::SetTexSize(float  size_w, float size_h)
{
	m_tex_size_h = size_h;
	m_tex_size_w = size_w;
}
void C_TEX_DATA::SetTexSize(float  size_w, float size_h)
{
	m_tex_size_h = size_h;
	m_tex_size_w = size_w;
}
	
//�e�N�X�`���ǂݍ��݁E�\���N���X���\�b�h�S-------
//�QD�g�p�ݒ�
void  CDrawTexture::Set2DDraw()
{
	//���_���C�A�E�g
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );

	//�A���o�b�t�@�Œ�
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer );

	//�g�p����V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP  );

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0,1,&m_pSampleLinear);

	//�o�[�e�b�N�X�o�b�t�@�o�^
	UINT stride = sizeof( DRAW_POLYGON ),offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	//�C���f�b�N�X�o�b�t�@�o�^
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

//�����蔻��`��
void CDrawTexture::DrawHitBox(float x,float y,float h,float w,float col[4])
{
	//�QD�g�p�ݒ�
	Set2DDraw();
	//�V�F�[�_�f�[�^�A��
	D3D11_MAPPED_SUBRESOURCE pData;

	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		DRAW_2D_TEX  data;
		data.color[0]=col[0];	data.color[1]=col[1];	data.color[2]=col[2];	data.color[3]=col[3];

		data.size[0]=0.0f;				data.size[1]=0.0f;
		data.size[2]=(float)m_width;	data.size[3]=(float)m_height;

		data.rect_out[0]=(float)x;		data.rect_out[1]=(float)y;
		data.rect_out[2]=(float)x+w;	data.rect_out[3]=(float)y+h;

		data.rect_in[0]=0.0f;			data.rect_in[1]=0.0f;
		data.rect_in[2]=0.0f;			data.rect_in[3]=0.0f;
		

		data.rotation[0] = 0.0f;		data.rotation[1] = 0.0f;
		data.rotation[2] = 0.0f;		data.rotation[3] = 0.0f;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&data, sizeof(DRAW_2D_TEX));
		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
	}

	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(4, 0, 0);
}

//�����`��
void CDrawTexture::DrawStr(ID3D11ShaderResourceView* ptex_res_view,float x,float y,float size,float col[4])
{
	//�QD�g�p�ݒ�
	Set2DDraw();
	//�V�F�[�_�f�[�^�A��
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		DRAW_2D_TEX  data;
		data.color[0]=col[0];	data.color[1]=col[1];	data.color[2]=col[2];	data.color[3]=col[3];

		data.size[0]=1.0f;				data.size[1]=1.0f;
		data.size[2]=(float)m_width;	data.size[3]=(float)m_height;

		data.rect_out[0]=(float)x;		data.rect_out[1]=(float)y;
		data.rect_out[2]=(float)x+size;	data.rect_out[3]=(float)y+size;

		data.rect_in[0]=0.0f;			data.rect_in[1]=0.0f;
		data.rect_in[2]=1.0f;			data.rect_in[3]=1.0f;

		data.rotation[0] = 0.0f;		data.rotation[1] = 0.0f;
		data.rotation[2] = 0.0f;		data.rotation[3] = 0.0f;

		memcpy_s( pData.pData, pData.RowPitch, (void*)&data, sizeof( DRAW_2D_TEX  ) );

		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
	}

	//�e�N�X�`���ݒ�
	m_pDeviceContext->PSSetShaderResources(0,1, &ptex_res_view);

	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(4, 0, 0);
}

//�`��-----------------------------------------------------------------------
void CDrawTexture:: Draw(int id,RECT_F* src,RECT_F* dst,float col[4],float r)
{
	if(m_img_max < id ) return ;
	if(vec_tex_data[id]->GetTexData()==nullptr) return ; 

	//�QD�g�p�ݒ�
	Set2DDraw();

	//�V�F�[�_�f�[�^�A��
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		DRAW_2D_TEX  data;
		data.color[0]=col[0];	data.color[1]=col[1];	
		data.color[2]=col[2];	data.color[3]=col[3];

		data.size[0]=(float)vec_tex_data[id]->GetTexSizeW();	data.size[1]= (float)vec_tex_data[id]->GetTexSizeH();
		data.size[2]=(float)m_width;							data.size[3]=(float)m_height;

		data.rect_out[0]=dst->m_x;		data.rect_out[1]=dst->m_y;
		data.rect_out[2]=dst->m_width;		data.rect_out[3]=dst->m_height;

		data.rect_in[0]=src->m_x;		data.rect_in[1]=src->m_y;
		data.rect_in[2]=src->m_width;		data.rect_in[3]=src->m_height;

		data.rotation[0] = r;		data.rotation[1] = r;
		data.rotation[2] = r;		data.rotation[3] = r;
		memcpy_s( pData.pData, pData.RowPitch, (void*)&data, sizeof( DRAW_2D_TEX  ) );

		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
	}

	//�e�N�X�`���ݒ�
	m_pDeviceContext->PSSetShaderResources(0,1,vec_tex_data[id].get()->GetTexData());

	// �u�����h�Ȃ����Z�b�g
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	m_pDeviceContext->OMSetBlendState(m_pBlendState[m_mode[id]], blendFactor, 0xffffffff);

	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(4, 0, 0);

}
//�M���@�Ǝ��ɒǉ������֐�------------------------------------------


void CDrawTexture::SetOpacity(int id, float alpha)
{

	m_opacity[id] = alpha;
}
void CDrawTexture::SetScaleX(int id, float scale)
{
	m_scale_x[id] = scale;
}
void CDrawTexture::SetScaleY(int id, float scale)
{
	m_scale_y[id] = scale;
}
void CDrawTexture::SetRotationZ(int id, float rotate)
{
	m_rotation_z[id] = rotate;
}
void CDrawTexture::SetColor(int id, COLOR color)
{
	m_color[id] = color;
}
void CDrawTexture::SetBlend(int id, E_BLEND_MODE mode)
{
	m_mode[id] = mode;
}
void CDrawTexture::InfoReset()
{
	for (int i = 0; i < SCENE_IMG_MAX; i++)
	{
		m_opacity[i] = 1.0f;
		m_scale_x[i] = 1.0f;
		m_scale_y[i] = 1.0f;
		m_rotation_z[i] = 0.0f;
		m_color[i] = { 1.0f,1.0f,1.0f };
		m_mode[i] = E_BLEND_MODE::NORMAL;
	}
}
int CDrawTexture::GetTexSizeW(int id)
{
	return vec_tex_data[id]->GetTexSizeW();
}
int CDrawTexture::GetTexSizeH(int id)
{
	return vec_tex_data[id]->GetTexSizeH();
}
//���̍��W�Ƀe�N�X�`���T�C�Y�̂܂ܕ`�悷��
void CDrawTexture::Draw(int id, float x, float y, bool flipW,bool flipH)
{
	float texSizeX = (float)vec_tex_data[id]->GetTexSizeW();
	float texSizeY = (float)vec_tex_data[id]->GetTexSizeH();

	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//�؂���ʒu�̐ݒ�
	src.m_x = 0.0f;		
	src.m_y = 0.0f;
	src.m_width = texSizeX;		
	src.m_height = texSizeY;
	//�\���ʒu�̐ݒ�
	if (flipW == false)
	{
		dst.m_x = x;
		dst.m_width = x + (texSizeX * m_scale_x[id]);
	}
	else
	{
		dst.m_x = x + (texSizeX * m_scale_x[id]);
		dst.m_width = x;

	}
	
	
	dst.m_y = y;
	dst.m_height = y + (texSizeY * m_scale_y[id]);

	Draw(id, &src, &dst, color, m_rotation_z[id]);
}


//rect[4] = {x, y, width, height}; �e�N�X�`���؂���ʒu
//flipH   true = ���E���], false = �ʏ�(�f�t�H���g=false)
void CDrawTexture::Draw(int id, float x, float y, RECT_F source, bool flipW,bool flipH )
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//�\���ʒu�̐ݒ�
	if (flipW == false)
	{
		dst.m_x = x;
		dst.m_width = x + source.m_width * m_scale_x[id];
	}
	else
	{
		dst.m_x = x + source.m_width * m_scale_x[id];
		dst.m_width = x;
	}
	if (flipH == false)
	{
		dst.m_y = y;
		dst.m_height = y + source.m_height * m_scale_y[id];
	}
	else
	{
		dst.m_y = y + source.m_height * m_scale_y[id];
		dst.m_height = y;
	}
	
	Draw(id, &src, &dst, color, m_rotation_z[id]);
}

void CDrawTexture::RightDraw(int id, float x, float y, RECT_F source, bool flipW, bool flipH)
{
	float adjustX = (source.m_width * m_scale_x[id]);
	float adjustY = (source.m_height * m_scale_y[id]);
	x -= adjustX;
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//�\���ʒu�̐ݒ�
	if (flipW == false)
	{

		dst.m_x = x ;
		dst.m_width = x + adjustX;
	}
	else
	{
		dst.m_x = x + adjustX;
		dst.m_width = x;
	}
	if (flipH == false)
	{
		dst.m_y = y;
		dst.m_height = y + adjustY;
	}
	else
	{
		dst.m_y = y + adjustY;
		dst.m_height = y;
	}

	Draw(id, &src, &dst, color, m_rotation_z[id]);
}

//���̍��W�Ƀe�N�X�`���T�C�Y�̂܂ܕ`�悷��
void CDrawTexture::CenterDraw(int id, float x, float y,bool flipW, bool flipH)
{
	float texSizeX = (float)vec_tex_data[id]->GetTexSizeW();
	float texSizeY = (float)vec_tex_data[id]->GetTexSizeH();
	float adjustX = (texSizeX * m_scale_x[id]) / 2.0f;
	float adjustY = (texSizeY * m_scale_y[id]) / 2.0f;

	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//�؂���ʒu�̐ݒ�
	src.m_x = 0.0f;
	src.m_y = 0.0f;
	src.m_width = texSizeX;
	src.m_height = texSizeY;
	//�\���ʒu�̐ݒ�
	if(flipW == false)
	{ 
		dst.m_x = x - adjustX;
		dst.m_width = x + adjustX;
	}
	else
	{
		dst.m_width= x - adjustX;
		dst.m_x = x +  adjustX;
	}
	if (flipH == false)
	{
		dst.m_y = y - adjustY;
		dst.m_height = y + adjustY;
	}
	else
	{
		dst.m_height = y - adjustY;
		dst.m_y = y + adjustY;
	}

	Draw(id, &src, &dst, color, m_rotation_z[id]);
}


//rct[4] = {x, y, width, height}; �e�N�X�`���؂���ʒu
//flipH   true = ���E���], false = �ʏ�(�f�t�H���g=false)
void CDrawTexture::CenterDraw(int id, float x, float y, RECT_F source, bool flipW, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	float adjustX = (source.m_width * m_scale_x[id]) / 2;
	float adjustY = (source.m_height * m_scale_y[id]) / 2;

	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//�\���ʒu�̐ݒ�
	if (flipW == false)
	{
		dst.m_x = x - adjustX;
		dst.m_width = x + adjustX;
	}
	else
	{
		dst.m_x = x + adjustX;
		dst.m_width = x - adjustX;
	}
	//�\���ʒu�̐ݒ�
	if (flipH == false)
	{
		dst.m_y = y - adjustY;
		dst.m_height = y + adjustY;
	}
	else
	{
		dst.m_y = y + adjustY;
		dst.m_height = y - adjustY;
	}
	Draw(id, &src, &dst, color, m_rotation_z[id]);
}


//rect[4] = {x, y, width, height}; �e�N�X�`���؂���ʒu
//flipH   true = ���E���], false = �ʏ�(�f�t�H���g=false)
void CDrawTexture::LeftCenterDraw(int id, float x, float y, RECT_F source, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };
	float adjustY = (source.m_height * m_scale_y[id]) / 2;
	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//�\���ʒu�̐ݒ�
	if (flipH == false)
	{
		dst.m_x = x;
		dst.m_width = x + source.m_width * m_scale_x[id];
	}
	else
	{
		dst.m_x = x + source.m_width * m_scale_x[id];
		dst.m_width = x;
	}
	dst.m_y = y - adjustY;
	dst.m_height = y + adjustY;
	Draw(id, &src, &dst, color, m_rotation_z[id]);
}

void CDrawTexture::TopCenterDraw(int id, float x, float y, RECT_F source, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	float adjustX = (source.m_width * m_scale_x[id]) / 2;

	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//�\���ʒu�̐ݒ�
	if (flipH == false)
	{
		dst.m_x = x - adjustX;
		dst.m_width = x + adjustX;
	}
	else
	{
		dst.m_x = x + adjustX;
		dst.m_width = x - adjustX;
	}
	dst.m_y = y;
	dst.m_height = y + source.m_height * m_scale_y[id];
	Draw(id, &src, &dst, color, m_rotation_z[id]);
}
void CDrawTexture::BottomCenterDraw(int id, float x, float y, RECT_F source, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	float adjustX = (source.m_width * m_scale_x[id]) / 2;

	//�؂���ʒu�̐ݒ�
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//�\���ʒu�̐ݒ�
	if (flipH == false)
	{
		dst.m_x = x - adjustX;
		dst.m_width = x + adjustX;
	}
	else
	{
		dst.m_x = x + adjustX;
		dst.m_width = x - adjustX;
	}

	y -= source.m_height * m_scale_y[id];
	dst.m_y = y;
	dst.m_height = y + source.m_height * m_scale_y[id];
	Draw(id, &src, &dst, color, m_rotation_z[id]);
}
//------------------------------------------------------------------------------

//�`����\�z����
void CDrawTexture::InitDraw(ID3D11Device* p_device,ID3D11DeviceContext* p_device_context,int w,int h,int img_max)
{
	InfoReset();

	m_pDevice		= p_device;
	m_pDeviceContext= p_device_context;

	//GDI+�̏����ݒ�
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	if (Gdiplus::GdiplusStartup(&g_diplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
	{
		return ;
	}

	//�E�B���h�E���擾
	m_width =w;
	m_height=h; 

	//�擾�C���[�W�ő吔�����ɋ��e�ʂƗv�f����ݒ�
	m_img_max=img_max;
	vec_tex_data.reserve(m_img_max);//�z��̋��e�ʐݒ�
	vec_tex_data.resize(m_img_max);	//�z��̗v�f���ݒ�
	

	//�擾�e�N�X�`���������m��
	for(int i=0; i<m_img_max;i++)
	{
		vec_tex_data[i].reset(new C_TEX_DATA());
	}

	//hlsl�t�@�C����
	const wchar_t* hlsl_name=L"GameL\\Draw2DTex.hlsl";
	
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́B
	//XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ƂȂ�
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors		 =NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DCompile(g_hlsl_str, strlen(g_hlsl_str), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs", "vs_4_0", 0,0, &pCompiledShader, &pErrors)))
	{
		char* c = (char*)pErrors->GetBufferPointer();
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(pErrors);
	
	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s",NULL,MB_OK);
		return ;
	}
	
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_UINT	, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬�E���C�A�E�g���Z�b�g
	if( FAILED(m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
		return ;
	SAFE_RELEASE(pCompiledShader);

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DCompile(g_hlsl_str, strlen(g_hlsl_str), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps", "ps_4_0", 0, 0, &pCompiledShader, &pErrors)))
	{
		char* c = (char*)pErrors->GetBufferPointer();
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"�s�N�Z���V�F�[�_�[�쐬���s",NULL,MB_OK);
		return ;
	}
	SAFE_RELEASE(pCompiledShader);


	//�o�[�e�b�N�X�o�b�t�@�쐬
	DRAW_POLYGON vertices[] =
	{
		{ 1.0f, 1.0f, 0.0f,		1,3},	//���_1
		{ 1.0f, 0.0f, 0.0f,		1,2},	//���_2
		{ 0.0f, 1.0f, 0.0f,		0,3},	//���_3
		{ 0.0f, 0.0f, 0.0f,		0,2},	//���_4	
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( DRAW_POLYGON ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) ) )
		return ;

	//�C���f�b�N�X�o�b�t�@�쐬
	unsigned short hIndexData[4] = 
	{
		0,1,2,3,
	};

	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(hIndexData);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = hIndexData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	//���̃C���f�b�N�X�o�b�t�@���쐬
	m_pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &m_pIndexBuffer);

	//�R���X�^���g�o�b�t�@�[�쐬 �V�F�[�_�Ƀf�[�^�󂯓n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags			=D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			=sizeof( DRAW_2D_TEX );
	cb.CPUAccessFlags		=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			=0;
	cb.StructureByteStride	=0;
	cb.Usage				=D3D11_USAGE_DYNAMIC;
	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer)))
	{
		return ;
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc,sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter   = D3D11_FILTER_ANISOTROPIC;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.BorderColor[0] = 0.0f;
	SamDesc.BorderColor[1] = 0.0f;
	SamDesc.BorderColor[2] = 0.0f;
	SamDesc.BorderColor[3] = 0.0f;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 2;
	SamDesc.MinLOD = 0.0f;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    m_pDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear);

	//�u�����h-------------------------------------------------------------

	// �u�����h�̐ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	for (int i = 0; i < E_BLEND_MODE::BLEND_NUMMAX; i++)
	{
		E_BLEND_MODE mode = (E_BLEND_MODE)i;

		// Dest=��{�F(�����C���[), Src=�����F(�ヌ�C���[)
		switch (mode)
		{
		case NORMAL:        // �ʏ�(�A���t�@�u�����h)
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			// Src * SrcA + Dest * (1 - SrcA)
			break;

		case ADDITION:      // ���Z
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * 1 + Dest * 1
			break;

		case ADDITIONALPHA: // ���Z(���߂���)
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * SrcA + Dest * 1
			break;

		case SUBTRACTION:   // ���Z
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			// Src * 0 + Dest * (1 - Src)
			break;

		case SCREEN:        // �X�N���[��
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * (1 - Dest) + Dest * 1
			break;

		case NONE:          // �Ȃ�
		default:            // �ی�����
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			// Src * 1 + Dest * 0
		}

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		// �u�����h�X�e�[�g���쐬
		if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState[i])))
		{
			MessageBox(NULL, L"�u�����h�X�e�[�g���쐬�ł��܂���ł����B", NULL, MB_OK | MB_ICONERROR);
			return;
		}
	}
}

//�e�N�X�`�����[�h
void  CDrawTexture::LoadImage(const wchar_t* name,int id,TEX_SIZE hw)
{
	if( 0  > id) 
		return ; 
	
	if(m_img_max < id) 
		return ; 

	OutputDebugString(name);
	OutputDebugString(L"\n");
	
	if(vec_tex_data[id]->GetTexData()!=nullptr)
	{
		vec_tex_data[id].reset(new C_TEX_DATA());
	}

	//�e�N�X�`���[�쐬
	int h, w;
	ID3D11ShaderResourceView* tex=nullptr;
	CreateTextureFromFile(m_pDevice, m_pDeviceContext, name, &tex,&w,&h);
	
	//�傫���擾
	vec_tex_data[id]->SetTexData(tex);
	vec_tex_data[id]->SetTexSize((float)w,(float) h);

	return ;
}

//�C���[�W���j��
void  CDrawTexture::DeleteImage()
{
	//�e�N�X�`���������̃��Z�b�g
	for(int i=0; i<m_img_max;i++)
	{
		vec_tex_data[i].reset(new C_TEX_DATA());
	}
}

//�`��p�I�u�W�F�N�g�j��
void  CDrawTexture::DeleteDrawTexture()
{
	DeleteImage();
	//�x�N�^�[���j��
	vec_tex_data.clear();
	vec_tex_data.shrink_to_fit();

	//GDI+�̉��
	Gdiplus::GdiplusShutdown(g_diplusToken);

	//�\���f�[�^�j��
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);		
	SAFE_RELEASE(m_pPixelShader);	
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}


bool CreateTextureFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
	const wchar_t*fileName, ID3D11ShaderResourceView** tex,int* w,int*  h)
{
	// �摜�̓ǂݍ���
	int width = 0;
	int height = 0;

	//�摜�t�@�C�����J���F�Ή��摜�`�� BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(fileName);
	if (pBitmap->GetLastStatus() == Gdiplus::Ok)
	{
		width  = pBitmap->GetWidth();
		height = pBitmap->GetHeight();
	}
	else
	{
		return false;
	}

	//��e�N�X�`���쐬
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;						// �T���v�����O��1�s�N�Z���̂�
	desc.Usage = D3D11_USAGE_DYNAMIC;				// CPU�������݉\
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_���\�[�X
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU���珑�����݃A�N�Z�X��
	desc.Height= height;							// �c�̃T�C�Y
	desc.Width = width;								// ���̃T�C�Y

	//�e�N�X�`���N���G�C�g
	ID3D11Texture2D* resource_tex;
	pDevice->CreateTexture2D(&desc, 0, &resource_tex);

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	resource_tex->GetDesc(&texDesc);
	
	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	pDevice->CreateShaderResourceView(resource_tex, &srvDesc, tex);

	//�e�N�X�`�����b�N
	D3D11_MAPPED_SUBRESOURCE mapped;
	DWORD* pBits;
	Gdiplus::Color  srcColor;
	void*  ptr = nullptr;

	pDeviceContext->Map(resource_tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	pBits = (DWORD*)mapped.pData;

	memset(pBits, 0xFF, sizeof(DWORD)*width*height);
	//�쐬�����e�N�X�`���̑傫����32�ׂ̂���ɂȂ邽�߃T�C�Y�𑪂蒼��
	int i;
	for (i = 0; i < width; i += 32)
		;
	//�f�[�^�o�^
	int width_size = i;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pBitmap->GetPixel(j, i, &srcColor);
			pBits[i*width_size + j] = (srcColor.GetR()) | (srcColor.GetG() << 8) | (srcColor.GetB() << 16) | (srcColor.GetA() << 24);
		}
	}

	//�A�����b�N
	pDeviceContext->Unmap(resource_tex, D3D11CalcSubresource(0, 0, 1));

	//�r�b�g�}�b�v���j��
	delete pBitmap;

	resource_tex->Release();

	*h = height;
	*w = width;
	return true;
}


//bool CreateTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
//	const wchar_t* fileName, ID3D11ShaderResourceView** tex, int* w, int* h)
//{
//	{
//		//// �����R�[�h�����C�h������ɕϊ�
//		//// �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̑��ɕ�����I�[�R�[�h������������Z�L���A�ȑΉ����D�܂����ł��B
//		//wchar_t     path[MAX_PATH];
//		//size_t      pathLength = 0;
//
//		//if (mbstowcs_s(&amp; pathLength,            // [out]    �ϊ����ꂽ������
//		//	&amp; path[0],               // [out]    �ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
//		//	MAX_PATH,               // [in]     �o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
//		//	fileName,               // [in]     �}���`�o�C�g������̃A�h���X(�ϊ���)
//		//	_TRUNCATE) != 0) {    // [in]     �o�͐�Ɋi�[���郏�C�h�����̍ő吔
//		//	return false;
//		//}
//
//		//  GDI+�I�u�W�F�N�g�i�摜�W�J�ɕK�v�j
//		Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
//		ULONG_PTR                       gdiplusToken;
//
//		//---- GDI+�̏����ݒ�
//		if (Gdiplus::GdiplusStartup(&gdiplusToken,&gdiplusStartupInput, NULL) != Gdiplus::Ok) {
//			return false;
//		}
//
//		//-------------------------------------------------------------
//		// �摜�̓ǂݍ���
//		//-------------------------------------------------------------
//		bool    result = false;
//		int     width = 0;         // �摜�̕�
//		int     height = 0;         //  �V  ����
//
//		//--- �摜�t�@�C�����J��
//		//  �y�Ή��摜�`���z  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
//		Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(fileName);
//		if (pBitmap->GetLastStatus() == Gdiplus::Ok) {
//			//---- �摜�T�C�Y���̗̈�m��
//			width = pBitmap->GetWidth();
//			height = pBitmap->GetHeight();
//
//			// �����[�h����摜�̃������̉𑜓x��ύX/�ݒ�i���̈ʒu�ɔC�ӂɋL�q���ĉ������j
//			if ( /* createBuffer(width, height)*/) {
//				result = true;
//			}
//		}
//
//		//---- �摜�C���[�W�̓ǂݍ���
//		if (result == true) {
//			for (int y = 0; y < height; y++) {
//				for (int x = 0; x < width; x++) {
//					Gdiplus::Color  srcColor;
//					pBitmap->GetPixel(x, y, &srcColor);
//
//					unsigned char r = srcColor.GetR();
//					unsigned char g = srcColor.GetG();
//					unsigned char b = srcColor.GetB();
//					unsigned char a = srcColor.GetA();
//
//					// ���s�N�Z���ւ̕`��i���̈ʒu�ɔC�ӂɋL�q���ĉ������j
//					// setPixel(x, y,  r, g, b, a);
//				}
//			}
//		}
//
//		delete pBitmap;
//
//		//---- GDI+�̉��
//		Gdiplus::GdiplusShutdown(gdiplusToken);
//
//		return result;
//	}
//}
//CTexture
//#include "../MusicGame.h"
//void CTexture::LoadTexture(std::string path)
//{
//	OutputDebugString(MusicGame::StringToWString("Loading" + path + "\n").c_str());
//
//	wstring wsr = MusicGame::StringToWString(path);
//	m_load_success = false;
//	if (GetTexData() != nullptr)
//	{
//		m_pSRV = nullptr;
//	}
//
//	//�e�N�X�`���[�쐬
//	int h, w;
//	ID3D11ShaderResourceView* tex = nullptr;
//	bool result = CreateTextureFromFile(CDrawTexture::m_pDevice, CDrawTexture::m_pDeviceContext, wsr.c_str(), &tex, &w, &h);
//
//	if (result == false)return;
//
//	//�傫���擾
//	SetTexData(tex);
//	SetTexSize((float)w, (float)h);
//	m_load_success = true;
//	return;
//}
void CTexture::Draw(int x, int y)
{
	RECT_N rectangle = { 0,0,GetTexSizeW(),GetTexSizeH() };
	Draw(x, y, rectangle);
}
void CTexture::Draw(int x, int y, E_REFERENCE ref_x, E_REFERENCE ref_y)
{
	RECT_N rectangle = { 0,0,m_tex_size_w,m_tex_size_h };
	Draw(x, y, rectangle, ref_x, ref_y);
}
void CTexture::Draw(int x, int y, RECT_N rectangle, E_REFERENCE ref_x, E_REFERENCE ref_y)
{
	float width = (float)rectangle.m_width * m_scale_x;
	float height = (float)rectangle.m_height * m_scale_y;
	float offset_x = 0;
	float offset_y = 0;

	if (ref_x == ref_Center)
	{
		offset_x = -width * 0.5f;
	}
	else if (ref_x == ref_Right)
	{
		offset_x = -width;
	}
	if (ref_y == ref_Center)
	{
		offset_y = -height * 0.5f;
	}
	else if (ref_y == ref_Bottom)
	{
		offset_y = -height;
	}
	x += offset_x;
	y += offset_y;
	Draw(x, y, rectangle);
}
void CTexture::Draw(int x, int y, RECT_N rectangle)
{
	CDrawTexture::Set2DDraw();


	//�V�F�[�_�f�[�^�A��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(CDrawTexture::m_pDeviceContext->Map(CDrawTexture::m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		DRAW_2D_TEX  data;
		data.color[0] = m_color.r;	data.color[1] = m_color.g;
		data.color[2] = m_color.b;	data.color[3] = m_opacity;

		data.size[0] = m_tex_size_w;	
		data.size[1] = m_tex_size_h;
		data.size[2] = CDrawTexture::m_width;
		data.size[3] = CDrawTexture::m_height;

		data.rect_out[0] = m_flip_x == false ? x : x + rectangle.m_width * m_scale_x;
		data.rect_out[1] = m_flip_y == false ? y : y + rectangle.m_height * m_scale_y;
		data.rect_out[2] = m_flip_x == false ? x+rectangle.m_width * m_scale_x : x;
		data.rect_out[3] = m_flip_y == false ? y + rectangle.m_height * m_scale_y : y;

		data.rect_in[0] = rectangle.m_x;
		data.rect_in[1] = rectangle.m_y;
		data.rect_in[2] = rectangle.m_x+rectangle.m_width;
		data.rect_in[3] = rectangle.m_y+rectangle.m_height;

		data.rotation[0] = m_rotation_z;		
		data.rotation[1] = m_rotation_z;
		data.rotation[2] = m_rotation_z;
		data.rotation[3] = m_rotation_z;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&data, sizeof(DRAW_2D_TEX));

		CDrawTexture::m_pDeviceContext->Unmap(CDrawTexture::m_pConstantBuffer, 0);
	}


	//�e�N�X�`���ݒ�
	CDrawTexture::m_pDeviceContext->PSSetShaderResources(0, 1, GetTexData());

	// �u�����h�Ȃ����Z�b�g
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	CDrawTexture::m_pDeviceContext->OMSetBlendState(CDrawTexture::m_pBlendState[m_blend], blendFactor, 0xffffffff);

	//�v���~�e�B�u�������_�����O
	CDrawTexture::m_pDeviceContext->DrawIndexed(4, 0, 0);

}