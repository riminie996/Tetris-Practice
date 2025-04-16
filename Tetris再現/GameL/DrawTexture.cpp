#include <wincodec.h>


#include <gdiplus.h>	//GDI+を使用

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

//ポリゴン構造体
struct DRAW_POLYGON
{
	float		   pos[3];	//x-y-z頂点情報
	unsigned int   id[2];	//ＵＶのＩＤ
};

//Draw2DTex.hlslグローバル
struct DRAW_2D_TEX
{
	float size[4];		//画面サイズ
	float color[4];		//カラー情報
	float rect_in[4];	//切り取り位置
	float rect_out[4];	//貼り付け位置
	float rotation[4];	//回転情報
};


//デバイス
ID3D11DeviceContext*CDrawTexture::m_pDeviceContext;
ID3D11Device*		CDrawTexture::m_pDevice; 

//テクスチャ情報
vector< shared_ptr<C_TEX_DATA> > CDrawTexture::vec_tex_data;

//ウィンドウ情報
int CDrawTexture::m_width ;
int CDrawTexture::m_height; 

//取得イメージ最大数
int CDrawTexture::m_img_max;

//シェーダ関係
ID3D11VertexShader* CDrawTexture::m_pVertexShader;		//バーテックスシェーダー
ID3D11PixelShader*  CDrawTexture::m_pPixelShader;		//ピクセルシェーダー
ID3D11SamplerState* CDrawTexture::m_pSampleLinear;		//テクスチャーサンプラー
ID3D11InputLayout*  CDrawTexture::m_pVertexLayout;		//頂点入力レイアウト
//バッファ
ID3D11Buffer*		CDrawTexture::m_pVertexBuffer;		//２Ｄポリゴン用バーティクスバッファ
ID3D11Buffer*		CDrawTexture::m_pConstantBuffer;	//アプリ←→シェーダー架け橋 コンスタントバッファ
ID3D11Buffer*		CDrawTexture::m_pIndexBuffer;		//インデックスバッファ
ID3D11BlendState*		CDrawTexture::m_pBlendState[BLEND_NUMMAX];		//インデックスバッファ

bool CreateTextureFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,
	const wchar_t*fileName, ID3D11ShaderResourceView** tex, int* w, int*  h);

ULONG_PTR	g_diplusToken;	//GDI+オブジェクト

//２D描画用HLSL文字列
extern const char* g_hlsl_str;

//テクスチャ保存用クラスメソッド郡----------------
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
	
//テクスチャ読み込み・表示クラスメソッド郡-------
//２D使用設定
void  CDrawTexture::Set2DDraw()
{
	//頂点レイアウト
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );

	//輸送バッファ固定
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer );

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);

	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP  );

	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0,1,&m_pSampleLinear);

	//バーテックスバッファ登録
	UINT stride = sizeof( DRAW_POLYGON ),offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	//インデックスバッファ登録
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

//当たり判定描画
void CDrawTexture::DrawHitBox(float x,float y,float h,float w,float col[4])
{
	//２D使用設定
	Set2DDraw();
	//シェーダデータ輸送
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

	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(4, 0, 0);
}

//文字描画
void CDrawTexture::DrawStr(ID3D11ShaderResourceView* ptex_res_view,float x,float y,float size,float col[4])
{
	//２D使用設定
	Set2DDraw();
	//シェーダデータ輸送
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

	//テクスチャ設定
	m_pDeviceContext->PSSetShaderResources(0,1, &ptex_res_view);

	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(4, 0, 0);
}

//描画-----------------------------------------------------------------------
void CDrawTexture:: Draw(int id,RECT_F* src,RECT_F* dst,float col[4],float r)
{
	if(m_img_max < id ) return ;
	if(vec_tex_data[id]->GetTexData()==nullptr) return ; 

	//２D使用設定
	Set2DDraw();

	//シェーダデータ輸送
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

	//テクスチャ設定
	m_pDeviceContext->PSSetShaderResources(0,1,vec_tex_data[id].get()->GetTexData());

	// ブレンドなしをセット
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	m_pDeviceContext->OMSetBlendState(m_pBlendState[m_mode[id]], blendFactor, 0xffffffff);

	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(4, 0, 0);

}
//舟瀬　独自に追加した関数------------------------------------------


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
//その座標にテクスチャサイズのまま描画する
void CDrawTexture::Draw(int id, float x, float y, bool flipW,bool flipH)
{
	float texSizeX = (float)vec_tex_data[id]->GetTexSizeW();
	float texSizeY = (float)vec_tex_data[id]->GetTexSizeH();

	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//切り取り位置の設定
	src.m_x = 0.0f;		
	src.m_y = 0.0f;
	src.m_width = texSizeX;		
	src.m_height = texSizeY;
	//表示位置の設定
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


//rect[4] = {x, y, width, height}; テクスチャ切り取り位置
//flipH   true = 左右反転, false = 通常(デフォルト=false)
void CDrawTexture::Draw(int id, float x, float y, RECT_F source, bool flipW,bool flipH )
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//表示位置の設定
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

	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//表示位置の設定
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

//その座標にテクスチャサイズのまま描画する
void CDrawTexture::CenterDraw(int id, float x, float y,bool flipW, bool flipH)
{
	float texSizeX = (float)vec_tex_data[id]->GetTexSizeW();
	float texSizeY = (float)vec_tex_data[id]->GetTexSizeH();
	float adjustX = (texSizeX * m_scale_x[id]) / 2.0f;
	float adjustY = (texSizeY * m_scale_y[id]) / 2.0f;

	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	//切り取り位置の設定
	src.m_x = 0.0f;
	src.m_y = 0.0f;
	src.m_width = texSizeX;
	src.m_height = texSizeY;
	//表示位置の設定
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


//rct[4] = {x, y, width, height}; テクスチャ切り取り位置
//flipH   true = 左右反転, false = 通常(デフォルト=false)
void CDrawTexture::CenterDraw(int id, float x, float y, RECT_F source, bool flipW, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };

	float adjustX = (source.m_width * m_scale_x[id]) / 2;
	float adjustY = (source.m_height * m_scale_y[id]) / 2;

	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//表示位置の設定
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
	//表示位置の設定
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


//rect[4] = {x, y, width, height}; テクスチャ切り取り位置
//flipH   true = 左右反転, false = 通常(デフォルト=false)
void CDrawTexture::LeftCenterDraw(int id, float x, float y, RECT_F source, bool flipH)
{
	RECT_F src;
	RECT_F dst;
	float color[4] = { m_color[id].r, m_color[id].g, m_color[id].b,m_opacity[id] };
	float adjustY = (source.m_height * m_scale_y[id]) / 2;
	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;
	//表示位置の設定
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

	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//表示位置の設定
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

	//切り取り位置の設定
	src.m_x = source.m_x;
	src.m_width = source.m_x + source.m_width;
	src.m_y = source.m_y;
	src.m_height = source.m_y + source.m_height;

	//表示位置の設定
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

//描画環境構築する
void CDrawTexture::InitDraw(ID3D11Device* p_device,ID3D11DeviceContext* p_device_context,int w,int h,int img_max)
{
	InfoReset();

	m_pDevice		= p_device;
	m_pDeviceContext= p_device_context;

	//GDI+の初期設定
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	if (Gdiplus::GdiplusStartup(&g_diplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
	{
		return ;
	}

	//ウィンドウ情報取得
	m_width =w;
	m_height=h; 

	//取得イメージ最大数を元に許容量と要素数を設定
	m_img_max=img_max;
	vec_tex_data.reserve(m_img_max);//配列の許容量設定
	vec_tex_data.resize(m_img_max);	//配列の要素数設定
	

	//取得テクスチャメモリ確保
	for(int i=0; i<m_img_max;i++)
	{
		vec_tex_data[i].reset(new C_TEX_DATA());
	}

	//hlslファイル名
	const wchar_t* hlsl_name=L"GameL\\Draw2DTex.hlsl";
	
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。
	//XXシェーダーとして特徴を持たない。後で各種シェーダーとなる
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors		 =NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DCompile(g_hlsl_str, strlen(g_hlsl_str), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs", "vs_4_0", 0,0, &pCompiledShader, &pErrors)))
	{
		char* c = (char*)pErrors->GetBufferPointer();
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(pErrors);
	
	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"バーテックスシェーダー作成失敗",NULL,MB_OK);
		return ;
	}
	
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_UINT	, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	//頂点インプットレイアウトを作成・レイアウトをセット
	if( FAILED(m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
		return ;
	SAFE_RELEASE(pCompiledShader);

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DCompile(g_hlsl_str, strlen(g_hlsl_str), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps", "ps_4_0", 0, 0, &pCompiledShader, &pErrors)))
	{
		char* c = (char*)pErrors->GetBufferPointer();
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"ピクセルシェーダー作成失敗",NULL,MB_OK);
		return ;
	}
	SAFE_RELEASE(pCompiledShader);


	//バーテックスバッファ作成
	DRAW_POLYGON vertices[] =
	{
		{ 1.0f, 1.0f, 0.0f,		1,3},	//頂点1
		{ 1.0f, 0.0f, 0.0f,		1,2},	//頂点2
		{ 0.0f, 1.0f, 0.0f,		0,3},	//頂点3
		{ 0.0f, 0.0f, 0.0f,		0,2},	//頂点4	
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

	//インデックスバッファ作成
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

	//そのインデックスバッファを作成
	m_pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &m_pIndexBuffer);

	//コンスタントバッファー作成 シェーダにデータ受け渡し用
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

	//テクスチャー用サンプラー作成
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

	//ブレンド-------------------------------------------------------------

	// ブレンドの設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	for (int i = 0; i < E_BLEND_MODE::BLEND_NUMMAX; i++)
	{
		E_BLEND_MODE mode = (E_BLEND_MODE)i;

		// Dest=基本色(下レイヤー), Src=合成色(上レイヤー)
		switch (mode)
		{
		case NORMAL:        // 通常(アルファブレンド)
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			// Src * SrcA + Dest * (1 - SrcA)
			break;

		case ADDITION:      // 加算
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * 1 + Dest * 1
			break;

		case ADDITIONALPHA: // 加算(透過あり)
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * SrcA + Dest * 1
			break;

		case SUBTRACTION:   // 減算
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			// Src * 0 + Dest * (1 - Src)
			break;

		case SCREEN:        // スクリーン
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			// Src * (1 - Dest) + Dest * 1
			break;

		case NONE:          // なし
		default:            // 保険処理
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			// Src * 1 + Dest * 0
		}

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		// ブレンドステートを作成
		if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState[i])))
		{
			MessageBox(NULL, L"ブレンドステートを作成できませんでした。", NULL, MB_OK | MB_ICONERROR);
			return;
		}
	}
}

//テクスチャロード
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

	//テクスチャー作成
	int h, w;
	ID3D11ShaderResourceView* tex=nullptr;
	CreateTextureFromFile(m_pDevice, m_pDeviceContext, name, &tex,&w,&h);
	
	//大きさ取得
	vec_tex_data[id]->SetTexData(tex);
	vec_tex_data[id]->SetTexSize((float)w,(float) h);

	return ;
}

//イメージ情報破棄
void  CDrawTexture::DeleteImage()
{
	//テクスチャメモリのリセット
	for(int i=0; i<m_img_max;i++)
	{
		vec_tex_data[i].reset(new C_TEX_DATA());
	}
}

//描画用オブジェクト破棄
void  CDrawTexture::DeleteDrawTexture()
{
	DeleteImage();
	//ベクター情報破棄
	vec_tex_data.clear();
	vec_tex_data.shrink_to_fit();

	//GDI+の解放
	Gdiplus::GdiplusShutdown(g_diplusToken);

	//表示データ破棄
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
	// 画像の読み込み
	int width = 0;
	int height = 0;

	//画像ファイルを開く：対応画像形式 BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
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

	//空テクスチャ作成
	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;						// サンプリングは1ピクセルのみ
	desc.Usage = D3D11_USAGE_DYNAMIC;				// CPU書き込み可能
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// シェーダリソース
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUから書き込みアクセス可
	desc.Height= height;							// 縦のサイズ
	desc.Width = width;								// 横のサイズ

	//テクスチャクリエイト
	ID3D11Texture2D* resource_tex;
	pDevice->CreateTexture2D(&desc, 0, &resource_tex);

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	resource_tex->GetDesc(&texDesc);
	
	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	pDevice->CreateShaderResourceView(resource_tex, &srvDesc, tex);

	//テクスチャロック
	D3D11_MAPPED_SUBRESOURCE mapped;
	DWORD* pBits;
	Gdiplus::Color  srcColor;
	void*  ptr = nullptr;

	pDeviceContext->Map(resource_tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	pBits = (DWORD*)mapped.pData;

	memset(pBits, 0xFF, sizeof(DWORD)*width*height);
	//作成したテクスチャの大きさは32のべき乗になるためサイズを測り直し
	int i;
	for (i = 0; i < width; i += 32)
		;
	//データ登録
	int width_size = i;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pBitmap->GetPixel(j, i, &srcColor);
			pBits[i*width_size + j] = (srcColor.GetR()) | (srcColor.GetG() << 8) | (srcColor.GetB() << 16) | (srcColor.GetA() << 24);
		}
	}

	//アンロック
	pDeviceContext->Unmap(resource_tex, D3D11CalcSubresource(0, 0, 1));

	//ビットマップ情報破棄
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
//		//// 文字コードをワイド文字列に変換
//		//// 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
//		//wchar_t     path[MAX_PATH];
//		//size_t      pathLength = 0;
//
//		//if (mbstowcs_s(&amp; pathLength,            // [out]    変換された文字数
//		//	&amp; path[0],               // [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
//		//	MAX_PATH,               // [in]     出力側のバッファのサイズ(単位:文字数)
//		//	fileName,               // [in]     マルチバイト文字列のアドレス(変換元)
//		//	_TRUNCATE) != 0) {    // [in]     出力先に格納するワイド文字の最大数
//		//	return false;
//		//}
//
//		//  GDI+オブジェクト（画像展開に必要）
//		Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
//		ULONG_PTR                       gdiplusToken;
//
//		//---- GDI+の初期設定
//		if (Gdiplus::GdiplusStartup(&gdiplusToken,&gdiplusStartupInput, NULL) != Gdiplus::Ok) {
//			return false;
//		}
//
//		//-------------------------------------------------------------
//		// 画像の読み込み
//		//-------------------------------------------------------------
//		bool    result = false;
//		int     width = 0;         // 画像の幅
//		int     height = 0;         //  〃  高さ
//
//		//--- 画像ファイルを開く
//		//  【対応画像形式】  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
//		Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(fileName);
//		if (pBitmap->GetLastStatus() == Gdiplus::Ok) {
//			//---- 画像サイズ分の領域確保
//			width = pBitmap->GetWidth();
//			height = pBitmap->GetHeight();
//
//			// ■ロードする画像のメモリの解像度を変更/設定（この位置に任意に記述して下さい）
//			if ( /* createBuffer(width, height)*/) {
//				result = true;
//			}
//		}
//
//		//---- 画像イメージの読み込み
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
//					// ■ピクセルへの描画（この位置に任意に記述して下さい）
//					// setPixel(x, y,  r, g, b, a);
//				}
//			}
//		}
//
//		delete pBitmap;
//
//		//---- GDI+の解放
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
//	//テクスチャー作成
//	int h, w;
//	ID3D11ShaderResourceView* tex = nullptr;
//	bool result = CreateTextureFromFile(CDrawTexture::m_pDevice, CDrawTexture::m_pDeviceContext, wsr.c_str(), &tex, &w, &h);
//
//	if (result == false)return;
//
//	//大きさ取得
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


	//シェーダデータ輸送
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


	//テクスチャ設定
	CDrawTexture::m_pDeviceContext->PSSetShaderResources(0, 1, GetTexData());

	// ブレンドなしをセット
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	CDrawTexture::m_pDeviceContext->OMSetBlendState(CDrawTexture::m_pBlendState[m_blend], blendFactor, 0xffffffff);

	//プリミティブをレンダリング
	CDrawTexture::m_pDeviceContext->DrawIndexed(4, 0, 0);

}