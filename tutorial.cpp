//=============================================================================
//
// チュートリアル画面処理 [tutorial.cpp]
// Author : 
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(4)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial_bg.png",
	"data/TEXTURE/tutorial_text.png",
};



static BOOL		g_Load = FALSE;
static TUTORIAL	g_Tutorial;

static float	alpha;
static BOOL		flag_alpha;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	g_Tutorial.w     = TEXTURE_WIDTH;
	g_Tutorial.h     = TEXTURE_HEIGHT;
	g_Tutorial.pos   = XMFLOAT3(g_Tutorial.w /2, g_Tutorial.h /2, 0.0f);
	g_Tutorial.texNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{


	if (GetKeyboardTrigger(DIK_RETURN))
	{//Enter押したら、ステージを切り替える
		SetFade(FADE_OUT, MODE_GAME);
	}
	//ゲームパッドで入力処理
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
	else if (IsButtonTriggered(0, BUTTON_Y))
	{
		SetFade(FADE_OUT, MODE_GAME);
}

#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();	//2d表示を行います設定	頂点単位ではなく、TEXそのものにエフェクトを行う

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	
	//材質(TEX)の色をそのまま使いますよ設定
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルの色を変更できる
	SetMaterial(material);

	// チュートリアルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float px = 0.0f;			// 頂点座標
		float py = 0.0f;
		float pw = TEXTURE_WIDTH;
		float ph = TEXTURE_HEIGHT;

		float tx = 0.0f;			// Tex座標
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// ポリゴン作成
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);	//4頂点　0番目から描画って設定
	}

	// チュートリアルのテキストを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float px = 0.0f;			// 頂点座標
		float py = 0.0f;
		float pw = TEXTURE_WIDTH;
		float ph = TEXTURE_HEIGHT;

		float tx = 0.0f;			// Tex座標
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		// ポリゴン作成
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);	//4頂点　0番目から描画って設定
	}

}





