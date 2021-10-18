#include "animator.h"
#include "GameTimer.h"

void Animator_Initialize(Animator * pThis)
{
	pThis->frame = 0;
	pThis->time = 0.0f;
	pThis->speed = 8.0;  // デフォルトのアニメーション速度
	pThis->isActive = true;  // デフォルトはアニメーションON
}

void Animator_Update(Animator * pThis)
{
	if (pThis->isActive) {  // アニメーションがONの時だけ処理を実行
		// デルタタイムを取得して加算
		pThis->time += GameTimer_GetDeltaTime() * pThis->speed;

		// テーブルアニメーションのデータ宣言
		static const int dragon_animationTable[] = { 0, 0, 1, 2, 2, 1, ANIMATION_LOOP };

		// テーブルを参照するのに使う添え字変数
		// ゲームループ停止時に配列領域オーバーを防ぐため％を使う
		int animationCounter = (int)pThis->time % ARRAYSIZE(dragon_animationTable);

		// ループの最後に達したら
		if (dragon_animationTable[animationCounter] == ANIMATION_LOOP) {
			animationCounter = 0;
			pThis->time = 0;
		}

		pThis->frame = dragon_animationTable[animationCounter];  // テーブルから現在のコマ番号取る
	}
}
