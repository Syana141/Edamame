#pragma once


// テーブルアニメーションでカウンターを０に戻せ、という合図
#define ANIMATION_LOOP  -1


// Animatorクラス

// 必要な変数を変数セットとして構造体にする
struct Animator {

	int frame;  // 現在のアニメのフレーム番号を持つ
	float time;  // デルタタイムを加算した時間を持つ変数
	float speed;  // アニメーションの再生速度

	//アニメーションのON/OFFを切り替えるための変数
	bool isActive;

};


// 構造体の変数を初期化する関数
void Animator_Initialize(Animator* pThis);

// 毎フレーム呼び出され、アニメーションのコマを進める関数
void Animator_Update(Animator* pThis);
