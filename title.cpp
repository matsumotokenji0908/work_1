//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : GP11A132 25 ���{�@����
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"
#include "debugproc.h"

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
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_bg.jpg",
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/newgame.png",
	"data/TEXTURE/loadgame.png",
	"data/TEXTURE/sun_moon.png",

};

time_t timer;        /* ���������o�����߂̌^�i���ۂ�unsigned long�^�j */
static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static float tex1 = 1;
static float tex2 = 2;
static float tex3 = 0;
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
static int sel = 0;
static int titleflg = 0;

float	alpha;
bool	flag_alpha;

int z = 0;


static BOOL						g_Load = FALSE;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

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
	g_Use   = true;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
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
void UninitTitle(void)
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
void UpdateTitle(void)
{
	tex3 = 1.0f / 2;//��

	struct tm* local;    /* tm�\���́i���������� */

	/* �N�����Ǝ����b�ۑ��p */
	int year, month, day, hour, minute, second;

	timer = time(NULL);        /* ���ݎ������擾 */
	local = localtime(&timer);    /* �n�����ɕϊ� */

	/* �N�����Ǝ����b��tm�\���̂̊e�p�����^����ϐ��ɑ�� */
	year = local->tm_year + 1900;        /* 1900�N����̔N�����擾����邽�� */
	month = local->tm_mon + 1;        /* 0��1���Ƃ��Ă��邽�� */
	day = local->tm_mday;
	hour = local->tm_hour;
	minute = local->tm_min;
	second = local->tm_sec;

	PrintDebugProc("%d�N%d��%d��%d����%d��%d�b" ,year, month, day, hour, minute, second);


	if (hour>6)
	{
		if (hour<18)
		{
			tex3 = 1.0f;//���z
		}
	}

	if (GetKeyboardRelease(DIK_RIGHT) || IsButtonTriggered(0,BUTTON_RIGHT))
	{
		sel++;
		sel = sel % 2;

	}
	if (GetKeyboardRelease(DIK_LEFT) || IsButtonTriggered(0,BUTTON_LEFT))
	{
		if (sel > 0)
		{
			sel--;
			sel = sel % 2;
		}
		else
		{
			sel = 1;
		}
	}

	switch (sel)
	{
	case 0://�j���[�Q�[��
		tex1 = 1;
		tex2 = 2;
		titleflg = 0;

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}
		// �Q�[���p�b�h�œ��͏���
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}
		else if (IsButtonTriggered(0, BUTTON_Y))
		{
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}


		break;
	case 1://���[�h�Q�[��
		tex1 = 2;
		tex2 = 1;
		titleflg = 1;

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
		else if (IsButtonTriggered(0, BUTTON_Y))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}

		break;
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
void DrawTitle(void)
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

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �^�C�g���̃��S��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y-100, TEXTURE_WIDTH_LOGO/2, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �^�C�g���Z���N�g�̃��S��`�� 1
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, g_Pos.x - 100, g_Pos.y, (TEXTURE_WIDTH_LOGO / 2) / tex1, (TEXTURE_HEIGHT_LOGO) / tex1, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �^�C�g���Z���N�g�̃��S��`�� 2
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, g_Pos.x + 100, g_Pos.y, (TEXTURE_WIDTH_LOGO / 2) / tex2, (TEXTURE_HEIGHT_LOGO) / tex2, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ���z�A����`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, g_Pos.x , g_Pos.y-200, (TEXTURE_WIDTH_LOGO / 6) , (TEXTURE_HEIGHT_LOGO), tex3, 0.0f, 1.0f/2, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

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
int GetTitle(void)
{
	return titleflg;
}