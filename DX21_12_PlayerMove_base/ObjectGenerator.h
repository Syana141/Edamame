#pragma once

#include "GameObject.h"

// ObjectGeneratorクラス

// メンバー関数プロトタイプ宣言

// ドラゴンとしてGameObjectを初期化する関数
void ObjectGenerator_SetDragon(GameObject* pObj);

// 背景としてGameObjectを初期化する関数
void ObjectGenerator_SetBG(GameObject* pObj);

// 32x32キャラとしてGameObjectを初期化する関数
void ObjectGenerator_Character32x32(GameObject* pObj, int id);
