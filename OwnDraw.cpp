// OwnDraw.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "OwnDraw.h"

#define MAX_LOADSTRING 100
#define ID_SMALLER 1
#define ID_LARGER 2
#define BTN_WIDTH ( 8 * cxChar)
#define BTN_HEIGHT ( 4 * cyChar) 

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OWNDRAW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OWNDRAW));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

void Triangle(HDC hdc, POINT pt[])
{
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Polygon(hdc, pt, 3);
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OWNDRAW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OWNDRAW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndSmaller, hwndLarger;
	static int cxClient, cyClient, cxChar, cyChar;
	int cx, cy;
	LPDRAWITEMSTRUCT pdis;
	POINT pt[3];
	RECT rc;

    switch (message)
    {
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		// Create the owner-draw pushbuttons

		hwndSmaller = CreateWindow(TEXT("button"), TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			0, 0, BTN_WIDTH, BTN_HEIGHT,
			hwnd, (HMENU)ID_SMALLER, hInst, NULL);

		hwndLarger = CreateWindow(TEXT("button"), TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			0, 0, BTN_WIDTH, BTN_HEIGHT,
			hwnd, (HMENU)ID_LARGER, hInst, NULL);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		// Move the buttons to the new center

		MoveWindow(hwndSmaller, cxClient / 2 - 3 *
			BTN_WIDTH / 2,
			cyClient / 2 - BTN_HEIGHT / 2,
			BTN_WIDTH, BTN_HEIGHT, TRUE);
		MoveWindow(hwndLarger, cxClient / 2 + BTN_WIDTH /
			2, cyClient / 2 - BTN_HEIGHT / 2,
			BTN_WIDTH, BTN_HEIGHT, TRUE);
		return 0;
    case WM_COMMAND:
		GetWindowRect(hwnd, &rc);

		// Make the window 10% smaller or larger

		switch (wParam)
		{
		case ID_SMALLER:
			rc.left += cxClient / 20;
			rc.right -= cxClient / 20;
			rc.top += cyClient / 20;
			rc.bottom -= cyClient / 20;
			break;

		case ID_LARGER:
			rc.left -= cxClient / 20;
			rc.right += cxClient / 20;
			rc.top -= cyClient / 20;
			rc.bottom += cyClient / 20;
			break;
		}

		MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left,
			rc.bottom -
			rc.top, TRUE);
		return 0;
	case WM_DRAWITEM:
		pdis = (LPDRAWITEMSTRUCT)lParam;

		// Fill area with white and frame it black

		FillRect(pdis->hDC, &pdis->rcItem,
			(HBRUSH)GetStockObject(WHITE_BRUSH));

		FrameRect(pdis->hDC, &pdis->rcItem,
			(HBRUSH)GetStockObject
			(BLACK_BRUSH));
		// Draw inward and outward black triangles
		cx = pdis->rcItem.right - pdis->rcItem.left;
		cy = pdis->rcItem.bottom - pdis->rcItem.top;
		switch (pdis->CtlID)
		{
		case ID_SMALLER:
			pt[0].x = 3 * cx / 8; pt[0].y = 1 * cy / 8;
			pt[1].x = 5 * cx / 8; pt[1].y = 1 * cy / 8;
			pt[2].x = 4 * cx / 8; pt[2].y = 3 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 7 * cx / 8; pt[0].y = 3 * cy / 8;
			pt[1].x = 7 * cx / 8; pt[1].y = 5 * cy / 8;
			pt[2].x = 5 * cx / 8; pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 5 * cx / 8; pt[0].y = 7 * cy / 8;
			pt[1].x = 3 * cx / 8; pt[1].y = 7 * cy / 8;
			pt[2].x = 4 * cx / 8; pt[2].y = 5 * cy / 8;

			Triangle(pdis->hDC, pt);
			pt[0].x = 1 * cx / 8; pt[0].y = 5 * cy / 8;
			pt[1].x = 1 * cx / 8; pt[1].y = 3 * cy / 8;
			pt[2].x = 3 * cx / 8; pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);
			break;

		case ID_LARGER:
			pt[0].x = 5 * cx / 8; pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8; pt[1].y = 3 * cy / 8;
			pt[2].x = 4 * cx / 8; pt[2].y = 1 * cy / 8;

			Triangle(pdis->hDC, pt);

			pt[0].x = 5 * cx / 8; pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8; pt[1].y = 3 * cy / 8;
			pt[2].x = 7 * cx / 8; pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);
			pt[0].x = 3 * cx / 8; pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8; pt[1].y = 5 * cy / 8;
			pt[2].x = 4 * cx / 8; pt[2].y = 7 * cy / 8;

			Triangle(pdis->hDC, pt);
			pt[0].x = 3 * cx / 8; pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8; pt[1].y = 5 * cy / 8;
			pt[2].x = 1 * cx / 8; pt[2].y = 4 * cy / 8;

			Triangle(pdis->hDC, pt);
			break;
		}

		// Invert the rectangle if the button is selected

		if (pdis->itemState & ODS_SELECTED)
			InvertRect(pdis->hDC, &pdis->rcItem);

		// Draw a focus rectangle if the button has the focus

		if (pdis->itemState & ODS_FOCUS)
		{
			pdis->rcItem.left += cx / 16;
			pdis->rcItem.top += cy / 16;
			pdis->rcItem.right -= cx / 16;
			pdis->rcItem.bottom -= cy / 16;

			DrawFocusRect(pdis->hDC, &pdis->rcItem);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
