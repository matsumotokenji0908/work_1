//=============================================================================
//
// �l�[���Z���N�g��ʏ��� [nameselect.cpp]
// Author : GP11A132 25 ���{�@����
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "nameselect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/nameselect_bg.jpg",
	"data/TEXTURE/aiu.png",
	"data/TEXTURE/left.png",
	"data/TEXTURE/square.png",
	"data/TEXTURE/aiualpha.png",
};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static float tex1 = 1;
static float tex2 = 2;
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
static int sel1 = 0;
static int sel2 = 0;
static int arrow = 0;
static int flg = 0;
static int high = 11;
static int wide = 5;

static float	alpha;
static bool		flag_alpha;

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitNameselect(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use = true;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = true;




	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_sample000);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitNameselect(void)
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
// �X�V����
//=============================================================================
void UpdateNameselect(void)
{

	if (GetKeyboardRelease(DIK_RIGHT))
	{
		sel1++;
		sel1 = sel1 % wide;

	}
	if (GetKeyboardRelease(DIK_LEFT))
	{
		if (sel1 > 0)
		{
			sel1--;
			sel1 = sel1 % wide;
		}
		else
		{
			sel1 = wide-1;
		}
	}
	if (GetKeyboardRelease(DIK_UP))
	{
		if (sel2 > 0)
		{
			sel2--;
			sel2 = sel2 % high;
		}
		else
		{
			sel2 = high-1;
		}
	}

	if (GetKeyboardRelease(DIK_DOWN))
	{
			sel2++;
			sel2 = sel2 % high;
	}




	//switch (i)
	//{
	//case 0://�͂�
	//	tex1 = 1;
	//	tex2 = 2;
	//	flg = 0;
	//	break;
	//case 1://������
	//	tex1 = 2;
	//	tex2 = 1;
	//	flg = 1;
	//	break;
	//}

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_GAME);
	}
	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_GAME);
	}

	if (flag_alpha == true)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			flag_alpha = false;
		}
	}
	else
	{
		alpha += 0.02f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = true;
		}
	}






#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawNameselect(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �l�[���Z���N�g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �l�[���Z���N�g�̃��S��`�� 1(�܏\���\)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y, (TEXTURE_WIDTH_LOGO*1.5f) / tex1, (TEXTURE_HEIGHT_LOGO*3.0f) / tex1, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �l�[���Z���N�g�̃��S��`�� 2(���)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, 250+((TEXTURE_WIDTH_LOGO * 1.5f)/5 *sel1), 160+((2+TEXTURE_HEIGHT_LOGO * 3.0f)/11*sel2), (TEXTURE_WIDTH_LOGO / 2.0f) / tex2, (TEXTURE_HEIGHT_LOGO) / tex2, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �l�[���Z���N�g�̃��S��`�� 3(�g)
	for(int i=0; i<4; i++)
	{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH/3.0f + (i*100), 80, (200) / tex2, (200) / tex2, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
	}


	// �l�[���Z���N�g�̃��S��`�� 4(���� 1)

		// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, SCREEN_WIDTH/3 + (0*100), 80, (200) / 2, (200) / 2, (1.0f*sel1)/5, (1.0f*sel2)/11 , 1.0f/5, 1.0f/11,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

	//	// �����Z�̃e�X�g
	//	SetBlendState(BLEND_MODE_ADD);		// ���Z����
	////	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����
	//	for(int i=0; i<30; i++)
	//	{
	//		// �e�N�X�`���ݒ�
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);
	//
	//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//		float dx = 100.0f;
	//		float dy = 100.0f;
	//		float sx = (float)(rand() % 100);
	//		float sy = (float)(rand() % 100);
	//
	//
	//		SetSpriteColor(g_VertexBuffer, dx+sx, dy+sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
	//			XMFLOAT4(0.3f, 0.3f, 1.0f, 0.5f));
	//
	//		// �|���S���`��
	//		GetDeviceContext()->Draw(4, 0);
	//	}
	//	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

}


//=============================================================================
// ���[�h�̎擾
//=============================================================================
int GetNameselect(void)
{
	return flg;
}


