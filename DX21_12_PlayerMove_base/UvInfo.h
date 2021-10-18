#pragma once

#define TEXTURE_SIZE_X  2048.0f
#define TEXTURE_SIZE_Y  2048.0f

// GameObjectにテクスチャ情報を持たせるための構造体
struct UvInfo
{
	// テクスチャの情報
	float offsetU, offsetV;  // テクスチャのどの位置から使うのか
	float sizeU, sizeV;  // １コマのサイズ
};
