#include "GameObject.h"
#include "GameTimer.h"


FRECT GameObject_GetXY(GameObject * pThis)
{
	// ポインタ　→　ある変数の存在してる場所を表す数字（番地/アドレス）を入れるための箱（変数）

	// アドレスから構造体の中の変数にアクセスするにはアロー演算子（->）を使う
	float leftX = pThis->posX - pThis->sizeX / 2;
	float rightX = leftX + pThis->sizeX;
	float topY = pThis->posY + pThis->sizeY / 2;
	float bottomY = topY - pThis->sizeY;

	FRECT rect = { leftX, rightX, topY, bottomY };

	return rect;
}

FRECT GameObject_GetUV(GameObject * pThis)
{
	float leftU =  pThis->uvinfo.offsetU + (float)pThis->animator.frame * pThis->uvinfo.sizeU;
	float rightU = leftU + pThis->uvinfo.sizeU;
	float topV =  pThis->uvinfo.offsetV + (float)pThis->charController.direction * pThis->uvinfo.sizeV;
	float bottomV = topV + pThis->uvinfo.sizeV;

	FRECT rect = { leftU, rightU, topV, bottomV };

	return rect;
}

void GameObject_Update(GameObject * pThis)
{
	// アニメーションのコマを進める
	Animator_Update(&pThis->animator);

	// キャラコントローラを使っての移動
	CharController_Update(&pThis->charController, &pThis->posX, &pThis->posY);
}

