//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : GP11A132 25 ���{�@����
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

