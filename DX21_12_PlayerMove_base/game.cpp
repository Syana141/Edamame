//*****************************************************************************
// DX21　サンプルソース
// ゲームのメインとなる処理を書いていくソースファイル
//*****************************************************************************

#include "game.h"
#include "direct3d.h"
#include "GameTimer.h"
#include "input.h"
#include "GameObject.h"
#include "ObjectGenerator.h"
#include "Sprite.h"
#include <math.h>

//*****************************************************************************
// 構造体定義
//*****************************************************************************

// 頂点１つあたりを表す構造体
struct VERTEX_POSTEX {
	float x, y, z;  // 頂点の位置

	float u, v;  // テクスチャのUV座標
};


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_SPRITE  64
// ポリゴン２つで１つの四角形（スプライト）。ポリゴンは３頂点なので、１スプライトは６頂点。
#define VERTEX_PER_SPRITE  (3*2)
#define VERTEX_BUFFER_SIZE  (MAX_SPRITE*sizeof(VERTEX_POSTEX)*VERTEX_PER_SPRITE)


// ドラゴンの発生数
#define MAX_OBJECT   4

//*****************************************************************************
// グローバル変数
//*****************************************************************************

ID3D11Buffer* gpVertexBuffer;  // 頂点バッファ用の変数

ID3D11ShaderResourceView* gpTexture; // テクスチャ用変数

GameObject gObjects[MAX_OBJECT];  // オブジェクト配列
GameObject* gpPlayer = gObjects + 1;

Sprite* texter;
Sprite* texter1;

//*****************************************************************************
// 関数の定義　ここから　↓
//*****************************************************************************

BOOL Game_Initialize()
{
	HRESULT hr;

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = VERTEX_BUFFER_SIZE;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Direct3D_GetDevice()->CreateBuffer(&bufferDesc, NULL, &gpVertexBuffer);

	if (FAILED(hr))
		return FALSE;
	// ↑　頂点バッファ作成　ここまで

	// ここで使用したい画像ファイルをテクスチャとして読み込む
	// 第一引数：画像ファイル名。もしフォルダに入っているならフォルダ名も一緒に書く。
	// 第二引数：読み込んだテクスチャが入る変数を指定
	hr = Direct3D_LoadTexture("assets/texture.png", &gpTexture);

	if (FAILED(hr)) {
		return FALSE;
	}

	// ゲーム時間の初期化をし、FPSを60に設定した。
	GameTimer_Initialize(60);
	
	// 配列０番に背景スプライトをセットする
	ObjectGenerator_SetBG(&gObjects[0]);

	// プレイヤー（配列１番）に勇者スプライトをセットする
	ObjectGenerator_Character32x32(gpPlayer, 0);
	gpPlayer->posX = -0.3f;
	gpPlayer->posY = -0.5f;

	// 配列２番にゴーストスプライトをセットする
	ObjectGenerator_Character32x32(&gObjects[2], 1);
	gObjects[2].posX =  0.3f;
	gObjects[2].posY = -0.5f;

	texter = new Sprite("assets/dora01.png", 3, 4);
	texter->SetSize(1, 1);
	texter->SetPart(1, 1);

	texter1 = new Sprite("assets/char02.png", 3, 4);
	texter1->SetSize(1, 1);
	texter1->SetPart(1, 1);
	return TRUE;
}


// ゲームループ中に実行したい、ゲームの処理を書く関数
void Game_Update()
{
	Input_Update();  // このゲームで使うキーの押下状態を調べて保存

	CharController* pPlayerCtrl = &gpPlayer->charController;

	if (Input_GetKeyPress(VK_LEFT)) {  // 左
		CharController_Accelerate(pPlayerCtrl, ACCEL_LEFT);
		pPlayerCtrl->direction = DIR_LEFT;
	}
	if (Input_GetKeyPress(VK_RIGHT)) {  // 右
		CharController_Accelerate(pPlayerCtrl, ACCEL_RIGHT);
		pPlayerCtrl->direction = DIR_RIGHT;
	}

	gpPlayer->animator.speed = fabsf(pPlayerCtrl->moveSpeedX / pPlayerCtrl->maxMoveSpeedX * 20.0f);

	CharController* pEnemyCtrl = &gObjects[2].charController;

	if (Input_GetKeyPress('A')) {  // 左
		CharController_Accelerate(pEnemyCtrl, ACCEL_LEFT);
		pEnemyCtrl->direction = DIR_LEFT;
	}
	if (Input_GetKeyPress('D')) {  // 右
		CharController_Accelerate(pEnemyCtrl, ACCEL_RIGHT);
		pEnemyCtrl->direction = DIR_RIGHT;
	}

	// 動いていればAnimatorのspeedにプラスの値を入れる、動いてなければ０を入れる
	gObjects[2].animator.speed = 1;

	// ポリゴンの頂点を定義
	// 頂点を結んでポリゴンを形成するときの法則
	// ・頂点は、配列の番号の若い順番に使用される
	// ・頂点を結んでポリゴンが形成されたとき、その結び順が時計回りになる場合だけポリゴンが表示される
	VERTEX_POSTEX vx[MAX_SPRITE*VERTEX_PER_SPRITE];

	// オブジェクト配列のXY計算、UV計算、頂点配列への適用を一括処理
	for (int i = 0; i < MAX_OBJECT; i++) {
		
		GameObject_Update(&gObjects[i]);
		// XY計算
		FRECT xy = GameObject_GetXY( &gObjects[i] );
		// UV計算
		FRECT uv = GameObject_GetUV( gObjects+i );
		// 頂点配列への適用
		vx[0 + i * 6 + 0] = { xy.left, xy.top, 0, uv.left, uv.top },  // １つめの頂点   ①左上
		vx[0 + i * 6 + 1] = { xy.right, xy.top, 0, uv.right, uv.top },  // ２つめの頂点　②右上
		vx[0 + i * 6 + 2] = { xy.right,  xy.bottom, 0, uv.right, uv.bottom },  // ３つめの頂点　③右下

		vx[0 + i * 6 + 3] = { xy.right,  xy.bottom, 0, uv.right, uv.bottom },  // ４つめの頂点　③右下
		vx[0 + i * 6 + 4] = { xy.left,  xy.bottom, 0, uv.left, uv.bottom },  // ５つめの頂点　④左下
		vx[0 + i * 6 + 5] = { xy.left,  xy.top, 0, uv.left, uv.top };  // ６つめの頂点　　  ①左上　　  ①左上
	}

	// 頂点バッファのデータをvx配列のデータで更新する
	Direct3D_GetContext()->UpdateSubresource(
		gpVertexBuffer,		// 更新対象のバッファ
		0,				// インデックス(0)
		NULL,				// 更新範囲(nullptr)
		vx,		// 反映データ
		0,				// データの1行のサイズ(0)
		0);				// 1深度スライスのサイズ(0)
}


void Game_Draw()
{
	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	Direct3D_GetContext()->ClearRenderTargetView(Direct3D_GetRenderTargetView(), clearColor);

	// ↓　自前の描画処理をここに書く *******

	// 描画で使用するテクスチャを指定する関数呼び出し
	// ここで指定されたテクスチャが、ピクセルシェーダーのグローバル変数にセットされる
	Direct3D_GetContext()->PSSetShaderResources(0, 1, &gpTexture);

	// 描画に使う頂点バッファを指定する
	UINT strides = sizeof(VERTEX_POSTEX);
	UINT offsets = 0;
	Direct3D_GetContext()->IASetVertexBuffers(0, 1, &gpVertexBuffer, &strides, &offsets);   // gpVertexBufferが頂点バッファ

	// これまでの設定で実際に描画する
	Direct3D_GetContext()->Draw(MAX_SPRITE*VERTEX_PER_SPRITE, 0);

	texter->Draw();
	texter1->Draw();
	// ↑　自前の描画処理をここに書く *******

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

void Game_Relese()
{
	COM_SAFE_RELEASE(gpTexture);  // テクスチャを読み込んだら、忘れずリリースすること
	COM_SAFE_RELEASE(gpVertexBuffer); // 頂点バッファを作成したら、忘れずにリリースすること
}

