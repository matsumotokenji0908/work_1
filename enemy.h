////=============================================================================
////
//// エネミーモデル処理 [enemy.h]
//// Author : GP11A132 25 松本　賢治
////
////=============================================================================
//#pragma once
//#include "model.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define MAX_ENEMY		(3)					// エネミーの数
//
//#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ
//
//
////*****************************************************************************
//// 構造体定義
////*****************************************************************************
//struct ENEMY
//{
//	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
//	XMFLOAT3			pos;				// モデルの位置
//	XMFLOAT3			rot;				// モデルの向き(回転)
//	XMFLOAT3			scl;				// モデルの大きさ(スケール)
//
//	bool				use;
//	bool				load;
//	DX11_MODEL			model;				// モデル情報
//	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
//
//	float				spd;				// 移動スピード
//	float				size;				// 当たり判定の大きさ
//	int					shadowIdx;			// 影のインデックス番号
//	
//	INTERPOLATION_DATA	*tbl_adr;			// アニメデータのテーブル先頭アドレス
//	int					tbl_size;			// 登録したテーブルのレコード総数
//	float				move_time;			// 実行時間
//
//	float				time;				// 線形補間用
//	int					tblNo;				// 行動データのテーブル番号
//	int					tblMax;				// そのテーブルのデータ数
//
//};
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT InitEnemy(void);
//void UninitEnemy(void);
//void UpdateEnemy(void);
//void DrawEnemy(void);
//
//ENEMY *GetEnemy(void);

//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(4)					// エネミーの数

#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	bool				use;
	bool				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
	int					shadowIdx;			// 影のインデックス番号

	INTERPOLATION_DATA* tbl_adr;			// アニメデータのテーブル先頭アドレス
	int					tbl_size;			// 登録したテーブルのレコード総数
	float				move_time;			// 実行時間

	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);

