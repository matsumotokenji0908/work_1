//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP11A132 25 ���{�@����
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/player01.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_PARTS				// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(0.8f)							// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��

#define PLAYER_PARTS_MAX	(2)								// �v���C���[�̃p�[�c�̐�



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static float		roty = 0.0f;

static LIGHT		g_Light;





// �v���C���[�̊K�w�A�j���[�V�����f�[�^


// �v���C���[�̓������E�ɓ������Ă���A�j���f�[�^
static INTERPOLATION_DATA move_tbl[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI/2, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 240 },
	{ XMFLOAT3(20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	     XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 240 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	     XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


// �v���C���[�̕����Ă���A�j���f�[�^
static INTERPOLATION_DATA walk_tbl[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 240 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	     XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


// �v���C���[�̑����Ă���A�j���f�[�^
static INTERPOLATION_DATA run_tbl[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),       XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 240 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	     XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};







//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;
	LoadModel(MODEL_PLAYER, &g_Player.model);

	g_Player.pos = XMFLOAT3(-10.0f, PLAYER_OFFSET_Y+50.0f, -50.0f);
	g_Player.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Player.scl = XMFLOAT3(0.5f, 0.5f, 0.5f);

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A

	g_Player.use = TRUE;			// true:�����Ă�
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �L�[�����������̃v���C���[�̌���
	roty = 0.0f;

	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = TRUE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);



		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
	//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].tbl_adr = move_tbl;	// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i].move_time = 0.0f;	// ���s���Ԃ��N���A

		// �p�[�c�̓ǂݍ���
		//g_Parts[i].load = TRUE;
		//LoadModel(MODEL_PLAYER_PARTS, &g_Parts[i].model);
	}

	g_Parts[0].parent = &g_Player;		// 
	g_Parts[0].tbl_adr = move_tbl;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	
	g_Parts[1].parent = &g_Player;		// 
	g_Parts[1].tbl_adr = move_tbl2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g


	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load == TRUE)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	g_Player.spd *= 0.96f;

	// �ړ�����
	if (GetKeyboardPress(DIK_LEFT)|| IsButtonPressed(0, BUTTON_LEFT))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x -= g_Player.spd;
		roty = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.x += g_Player.spd;
		roty = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z += g_Player.spd;
		roty = XM_PI;
	}
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
	{
		g_Player.spd = VALUE_MOVE;
		g_Player.pos.z -= g_Player.spd;
		roty = 0.0f;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.spd = 0.0f;
		roty = 0.0f;
	}
#endif


	{	// �����������Ƀv���C���[���ړ�������
		// �����������Ƀv���C���[���������Ă��鏊
		g_Player.rot.y = roty + cam->rot.y;

		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}


	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
	bool ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Player.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}


	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_Y))
	{
		SetBullet(g_Player.pos, g_Player.rot);

		if (GetScore() > 0)
		{
			// �X�R�A������
			AddScore(-1);
		}

	}


	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == TRUE)
		{
			// �ړ�����
			int		index = (int)g_Parts[i].move_time;
			float	time = g_Parts[i].move_time - index;
			int		size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);

			float dt = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
			g_Parts[i].move_time += dt;					// �A�j���[�V�����̍��v���Ԃɑ���

			if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
			{
				g_Parts[i].move_time = 0.0f;
				index = 0;
			}

			// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
			XMVECTOR p1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
			XMVECTOR p0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
			XMVECTOR vec = p1 - p0;
			XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

			// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
			XMVECTOR r1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].rot);	// ���̊p�x
			XMVECTOR r0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
			XMVECTOR rot = r1 - r0;
			XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

			// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
			XMVECTOR s1 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].scl);	// ����Scale
			XMVECTOR s0 = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].scl);	// ���݂�Scale
			XMVECTOR scl = s1 - s0;
			XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);

		}
	}




	// �|�C���g���C�g�̃e�X�g
	{
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

//	quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);




#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// �����̐ݒ�
	SetFuchi(0); //�����ON / OFF

	// ���f���`��
	DrawModel(&g_Player.model);



	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																			// ��
																			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}
