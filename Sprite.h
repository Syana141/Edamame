#pragma once

#include "direct3d.h"

/*
分割画像ファイル対応の描画クラス。

スプライトサイズの初期値は0x0なので、
描画するまでにSetSizeを呼び出してサイズを指定する必要がある。

game.cppの
Direct3D_GetSwapChain()->Present(0, 0);
の直前でDrawを呼び出すことで描画できる。

ゲームオブジェクトに持たせて使用すると良い。

最小使用例: 画面右下にドラゴンの二列目の二番目の画像を表示する

game.cpp

...いろいろ

Sprite sprite("assets/parts/dora01.png", 3, 4);
sprite.SetSize(1, 1);
sprite.SetPart(1, 1);
sprite.Draw();

Direct3D_GetSwapChain()->Present(0, 0);
*/

class Sprite {
public:
	// texturePath=画像パス
	// horizontalPartNum=横に何分割するか
	// verticalPartNum=縦に何分割するか
	Sprite(const char* texturePath, int horizontalPartNum, int verticalPartNum);
	~Sprite();

	void Draw();

	void SetPos(float x, float y);
	void SetSize(float width, float height);
	void SetPart(int x, int y);

private:
	int mHorizontalPartNum;
	int mVerticalPartNum;

	float mX = 0;
	float mY = 0;
	float mWidth = 0;
	float mHeight = 0;

	// 分割されたテクスチャのどれを描画するのか。
	int mPartX = 0; // 0～horizontalPartNum - 1
	int mPartY = 0; // 0～verticalPartNum - 1

	ID3D11Buffer* mpVertexBuffer = nullptr;
	ID3D11ShaderResourceView* mpTexture = nullptr;
};
