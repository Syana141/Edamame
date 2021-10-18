//*****************************************************************************
// DX21　サンプルソース
// WinMain関数とWndProc関数
//*****************************************************************************

#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

#include <windows.h>
#include "direct3d.h"
#include "game.h"
#include "GameTimer.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	CLASS_NAME		"サンプル"				// ウインドウクラスの名前
#define WINDOW_NAME		"スケルトンプログラム"	// ウィンドウの名前

#define	SCREEN_WIDTH	(800)					// ウインドウの幅
#define	SCREEN_HEIGHT	(600)					// ウインドウの高さ

//*****************************************************************************
// 関数のプロトタイプ宣言
//*****************************************************************************

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// メイン関数
//  (WINAPI : Win32API関数を呼び出す時の規約)
//  hInstance     : このアプリケーションのインスタンスハンドル(識別子)
//  hPrevInstance : 常にNULL(16bit時代の名残り)
//  lpCmdLine     : コマンドラインから受け取った引数へのポインタ
//  nCmdShow      : アプリケーションの初期ウインドウ表示方法の指定
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,						// 表示するウインドウのスタイルを設定
		WndProc,						// ウィンドウプロシージャのアドレス(関数名)を指定
		0,								// 通常は使用しないので"0"を指定
		0,								// 通常は使用しないので"0"を指定
		hInstance,						// WinMainのパラメータのインスタンスハンドルを設定
		NULL,							// 使用するアイコンを指定(Windowsがもっているアイコンを使うならNULL)
		LoadCursor(NULL, IDC_ARROW),	// マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),		// ウインドウのクライアント領域の背景色を設定
		NULL,							// Windowにつけるメニューを設定
		CLASS_NAME,						// ウインドウクラスの名前
		NULL							// 小さいアイコンが設定された場合の情報を記述
	};

	// ウィンドウクラスの登録
	RegisterClassExA(&wcex);

	// ウインドウを作成
	HWND hWnd = CreateWindowEx(0,						// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,			// ウィンドウの幅
		SCREEN_HEIGHT,			// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューハンドルまたは子ウィンドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ作成データ

	// Direct3Dの初期化処理を呼び出し
	HRESULT hr = Direct3D_Initialize(hWnd);

	if (FAILED(hr)) {
		MessageBoxA(hWnd, "Direct3Dの初期化に失敗", "エラー", MB_OK | MB_ICONERROR);
		goto INIT_ERROR;
	}

//	hr = Input_Initialize(hInstance, hWnd);

	if (FAILED(hr)) {
		MessageBoxA(hWnd, "Direct Inputの初期化に失敗", "エラー", MB_OK | MB_ICONERROR);
		goto INIT_ERROR;
	}

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);	// 指定されたウィンドウの表示状態を設定(ウィンドウを表示)
	UpdateWindow(hWnd);			// ウィンドウの状態を直ちに反映(ウィンドウのクライアント領域を更新)

	BOOL isSucceed;
	// ゲームの初期化処理を呼び出し
	isSucceed = Game_Initialize();

	if (isSucceed == FALSE) {
		MessageBoxA(hWnd, "Gameの初期化に失敗", "エラー", MB_OK | MB_ICONERROR);
		goto INIT_ERROR;
	}

	MSG msg;

	// メッセージループ　→　ゲームのメインループ
	// 第１引数：メッセージ情報
	// 第２引数：ウィンドウのハンドル
	// 第３引数：最初のメッセージ
	// 第４引数：最後のメッセージ
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				// 呼び出し側スレッドのメッセージキューからメッセージを取得
				TranslateMessage(&msg);					// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);					// ウィンドウプロシージャへメッセージを送出
			}
		}
		else {  // メッセージが何も発生していない時
			// ↓　ゲームに関するプログラムを書く場所

			// 60FPSなら約16ミリ秒に１回、if文が成立する
			if (GameTimer_ReachedNextFrame() == TRUE) {
			//	Input_Update();
				Game_Update();
				Game_Draw();
			}
			// ↑　ゲームに関するプログラムを書く場所
		}
	}

INIT_ERROR:
//	Input_Release();

	// Direct3Dの開放処理を呼び出し
	Direct3D_Release();

	// ウィンドウクラスの登録を解除
	//  第１引数：ウインドウクラス名
	//  第２引数：アプリケーションインスタンスのハンドル
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//  (CALLBACK : Win32API関数を呼び出す時の規約)
//  hWnd   : ウィンドウのハンドル
//  uMsg   : メッセージの識別子
//  wParam : メッセージの最初のパラメータ
//  lParam : メッセージの2番目のパラメータ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int sts;
	switch (uMsg)
	{
	case WM_DESTROY:		// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);			// "WM_QUIT"メッセージを返す　→　アプリ終了
		break;

	case WM_KEYDOWN:  // 何かキーが押されたら
		switch (wParam)
		{
		case VK_ESCAPE:  // ESCキーだったら
			int sts = MessageBoxA(hWnd, "終了しますか?", "終了確認", MB_YESNO);
			if (sts == IDYES) {
				DestroyWindow(hWnd);  // ウインドウを終了させる関数呼び出し
			}
			break;
		}
		break;

	case WM_CLOSE:  // xボタンが押されたら
		sts = MessageBoxA(hWnd, "終了しますか?", "終了確認", MB_YESNO);
		if (sts == IDYES) {
			DestroyWindow(hWnd);  // ウインドウを終了させる関数呼び出し
		}
		break;

	case WM_ACTIVATE:  // ウインドウがアクティブまたは非アクティブに変化した場合
		if (LOWORD(wParam) != WA_INACTIVE) {  // アクティブになった場合
			GameTimer_OnResume();
		}
		break;

	case WM_EXITSIZEMOVE: // ウインドウのサイズ変更または移動が完了した場合
		GameTimer_OnResume();
		break;

	default:
		// 上のcase以外の場合の処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}
