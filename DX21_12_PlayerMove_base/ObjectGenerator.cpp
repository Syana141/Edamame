#include "ObjectGenerator.h"

void ObjectGenerator_SetDragon(GameObject * pObj)
{
	// ポインタ変数の左に＊をつけると、アドレスが差している変数そのものとして振る舞う
	*pObj = {
		0, 0,  // 中心座標
		0.25f, 0.25f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化

	pObj->uvinfo = { 0, 0, 80.0f/TEXTURE_SIZE_X, 64.0f/TEXTURE_SIZE_Y };  // テクスチャ情報を渡す
}

void ObjectGenerator_SetBG(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		2, 2,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	pObj->animator.isActive = false;  // アニメーションOFF

	pObj->uvinfo = { 0, 0.5f, 640.0f / TEXTURE_SIZE_X, 480.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す
}

// この関数を成立させるためには、テクスチャを以下の条件で作成する
// ・32x32キャラは横並びで配置する
// ・32x32キャラはUV(0, 0.25f)の位置を左上として並べる
void ObjectGenerator_Character32x32(GameObject * pObj, int id)
{
	// ポインタ変数の左に＊をつけると、アドレスが差している変数そのものとして振る舞う
	*pObj = {
		0, 0,  // 中心座標
		0.15f, 0.20f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化

	// テクスチャ情報を渡す
	pObj->uvinfo = {
		(float)id*96.0f/ TEXTURE_SIZE_X, 0.25f,  // オフセットUV
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y  // １コマサイズUV
	};

	// キャラクターコントローラ初期化
	CharController_Initialize(&pObj->charController);
}
