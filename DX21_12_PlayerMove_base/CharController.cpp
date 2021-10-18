#include "CharController.h"
#include "GameTimer.h"
#include <math.h>

void CharController_Initialize(CharController * pThis)
{
	pThis->isActive = true;
	pThis->direction = DIR_RIGHT;
	pThis->maxMoveSpeedX = 0.6f;
	pThis->accelForceX = pThis->maxMoveSpeedX * 0.1f;
	pThis->stopForceX = pThis->accelForceX * 0.6f;
}

void CharController_Update(CharController * pThis, float* pPosX, float* pPosY)
{
	if (pThis->isActive) {
		// 移動処理
		*pPosX += 1;

		// 減速
	}
}

void CharController_Accelerate(CharController * pThis, float directionX)
{
	// 加速処理　directionXを使って

	// 速度が最高速度を超えないようにする
}
