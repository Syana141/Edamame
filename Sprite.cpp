#include "Sprite.h"

// game.cppのVERTEX_POSTEXと完全に一致させる必要がある。
struct VERTEX_POSTEX {
	float x, y, z;  // 頂点の位置

	float u, v;  // テクスチャのUV座標
};

Sprite::Sprite(const char* texturePath, int horizontalPartNum, int verticalPartNum) {
	HRESULT hr;

	// 頂点バッファ用意。
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(VERTEX_POSTEX) * 6;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Direct3D_GetDevice()->CreateBuffer(&bufferDesc, NULL, &mpVertexBuffer);;
	if (FAILED(hr)) {
		MessageBoxA(nullptr, "頂点バッファー作成失敗。", "Error", MB_OK);
		exit(-1);
	}

	// テクスチャ読み込み。
	hr = Direct3D_LoadTexture(texturePath, &mpTexture);
	if (FAILED(hr)) {
		char errorMsg[256] = "テクスチャ読み込み失敗: ";
		strcat_s(errorMsg, texturePath);
		MessageBoxA(nullptr, errorMsg, "Error", MB_OK);
		exit(-1);
	}

	mHorizontalPartNum = horizontalPartNum;
	mVerticalPartNum = verticalPartNum;
}

Sprite::~Sprite() {
	COM_SAFE_RELEASE(mpTexture);
	COM_SAFE_RELEASE(mpVertexBuffer);
}

void Sprite::Draw() {
	// 描画で使うテクスチャをセット。
	Direct3D_GetContext()->PSSetShaderResources(0, 1, &mpTexture);

	// 頂点シェーダーの情報を組み立てる。
	float l = mX;
	float r = mX + mWidth;
	float t = mY;
	float b = mY - mHeight;

	// テクスチャの一分割分のuv値を割り出す。
	float u = 1.0 / mHorizontalPartNum;
	float v = 1.0 / mVerticalPartNum;

	// 指定した描画部分のuv値を計算する。
	float lu = mPartX * u;
	float ru = (mPartX + 1) * u;
	float tv = mPartY * v;
	float bv = (mPartY + 1) * v;

	VERTEX_POSTEX vertexData[] = {
		{l, t, 0, lu, tv},
		{r, t, 0, ru, tv},
		{r, b, 0, ru, bv},

		{r, b, 0, ru, bv},
		{l, b, 0, lu, bv},
		{l, t, 0, lu, tv},
	};

	// 頂点バッファを更新
	Direct3D_GetContext()->UpdateSubresource(mpVertexBuffer, 0, NULL, vertexData, 0, 0);

	// 描画で使う頂点バッファをセット。
	UINT strides = sizeof(VERTEX_POSTEX);
	UINT offsets = 0;
	Direct3D_GetContext()->IASetVertexBuffers(0, 1, &mpVertexBuffer, &strides, &offsets);

	// スプライト描画。
	Direct3D_GetContext()->Draw(6, 0);
}

void Sprite::SetPos(float x, float y) {
	mX = x;
	mY = y;
}

void Sprite::SetSize(float width, float height) {
	mWidth = width;
	mHeight = height;
}

void Sprite::SetPart(int x, int y) {
	mPartX = x;
	mPartY = y;
}
