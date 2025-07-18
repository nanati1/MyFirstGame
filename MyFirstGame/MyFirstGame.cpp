// MyFirstGame.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "MyFirstGame.h"
#include "Direct3D.h"
#include "Quad.h"
#include "Texture.h"
#include "Camera.h"
#include "Dice.h"
#include <d3d11.h>

//リンカ
#pragma comment(lib, "d3d11.lib")

HWND hWnd = nullptr;



#define MAX_LOADSTRING 100

const wchar_t* WIN_CLASS_NAME = L"Sample Game Window";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;






// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
	//Instance	HINSTANCE	現在実行中のアプリケーションインスタンス（識別子）。主にウィンドウ作成やリソース読み込みで使う
	//hPrevInstance	HINSTANCE	常に NULL。昔のWindows（16bit）との互換用。現在は無視してOK
	//lpCmdLine	LPSTR	コマンドライン引数（main(int argc, char* argv[]) の argv[1] 相当）
	//nCmdShow	int	ウィンドウの表示方法。ShowWindow() 関数に渡して「普通に表示するか、最小化して始めるか」などを指定する

{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。

	//szWindowClass = WIN_CLASS_NAME;

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MYFIRSTGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);




	// アプリケーション初期化の実行:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	Camera::Initialize();
	HRESULT hr;
	hr = Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd);
	if (FAILED(hr))
	{
		PostQuitMessage(0);
	}






	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYFIRSTGAME));
	//ShowWindow(hWnd, nCmdShow);



	// メイン メッセージ ループ:
	//メッセージ	OSやユーザー操作が発生させる通知
	//メッセージループ	メッセージを受け取り、WndProcへ渡すループ処理
	//WndProc	受け取ったメッセージを処理する関数（コールバック）
	//Direct3D初期化
	//Direct3D::Initialize(winW, winH, hWnd);

	MSG msg;
	HACCEL aAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYFIRSTGAME));
	ZeroMemory(&msg, sizeof(msg));


	//Quad* q = new Quad();
	//q->Initialize();  // ← 初期化を忘れずに！


	Dice* dice = new Dice();
	dice->Initialize();

	while (msg.message != WM_QUIT)
	{

		//メッセージあり

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}


		//メッセージなし

			//ゲームの処理
			//背景の色
		Camera::Update();
		Direct3D::BeginDraw();

		static float angleX = 0.0f;
		static float angleY = 0.0f;
		angleX += 0.05f;  // ゆっくり
		angleY += 0.1f;

		XMMATRIX worldRotation =
			XMMatrixRotationX(XMConvertToRadians(angleX)) *
			XMMatrixRotationY(XMConvertToRadians(angleY));
		XMMATRIX uv = XMMatrixIdentity(); // 単位行列（回転なし）
		dice->Draw(worldRotation, uv);
		/*XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(0));
		q->Draw(mat);
		  */


		  //描画処理
		Direct3D::EndDraw();

		//スワップ（バックバッファを表に表示する）



	}

	//q->Release();
	//SAFE_DELETE(q);
	dice->Release();
	SAFE_DELETE(dice);
	Direct3D::Release();
	return (int)msg.wParam;
}
//ウィンドウプロシージャ（何かあった時によばれる関数）
//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);  //プログラム終了
//		return 0;
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}





//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);//この構造体のサイズ。`sizeof(WNDCLASSEX)` を指定するのが決まり

	wcex.style = CS_HREDRAW | CS_VREDRAW;//ウィンドウの動作スタイル（再描画のタイミングなど）。例：`CS_HREDRAW	CS_VREDRAW`
	wcex.lpfnWndProc = WndProc;//メッセージを処理する関数（`WinProc`）のアドレスを指定。
	wcex.cbClsExtra = 0;//拡張クラス情報のバイト数（基本は 0）。
	wcex.cbWndExtra = 0;//拡張ウィンドウ情報のバイト数（基本は 0）。
	wcex.hInstance = hInstance;//このウィンドウを作るプログラムのインスタンスハンドル。
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYFIRSTGAME));//アプリケーションのアイコン。`LoadIcon` で標準アイコンを指定可能。
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);//ウィンドウ上で表示されるマウスカーソル。
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//ウィンドウの背景色（ブラシ）。例：`(HBRUSH)(COLOR_WINDOW+1)`
	wcex.lpszMenuName = NULL;//メニューリソースの名前（使わない場合は `nullptr`）。
	wcex.lpszClassName = szWindowClass;//このウィンドウクラスの名前（任意の文字列）。`CreateWindowEx` に渡す。
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));//タスクバーなどに表示される小さいアイコン。`hIcon`と同じでOK

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する


	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	hWnd = CreateWindowW(szWindowClass, WIN_CLASS_NAME, WS_OVERLAPPEDWINDOW, //HWNDをローカル変数として定義したことがダメだった。上で定義しているグローバル変数を用いるのが正解だった。
		CW_USEDEFAULT, 0, winW, winH, nullptr, nullptr, hInstance, nullptr); szWindowClass,
		// szWindowClass lpClassName → ウィンドウクラス名
		// szTitle lpWindowName → ウィンドウのタイトル
		// WS_OVERLAPPEDWINDOW dwStyle → 枠付き普通のウィンドウ
		// CW_USEDEFAULT, 0 X, Y
		// CW_USEDEFAULT, 0 Width, Height
		// nullptr hWndParent → 親なし
		// nullptr hMenu → メニューなし
		// hInstance → アプリのインスタンス
		// nullptr lpParam → 特に渡すデータなし

		//戻り値の型は HWND（ハンドル to ウィンドウ)。
		//成功した場合：
		//→作成されたウィンドウの識別子（ハンドル） が返される。
		//失敗した場合：
		//→NULL（0） が返される。


		/*szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		hInstance,
		nullptr;*/

		szWindowClass,         // クラス名
		WIN_CLASS_NAME,        // ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,   // スタイル
		CW_USEDEFAULT,         // X
		CW_USEDEFAULT,         // Y
		winW,                  // 幅
		winH,                  // 高さ
		nullptr,               // 親ウィンドウ
		nullptr,               // メニュー
		hInstance,             // インスタンス
		nullptr;                // パラメータ

	if (!hWnd) {
		return FALSE;
	}



	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return true;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC を使用する描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
