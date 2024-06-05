//インクルード
#include <Windows.h>
#include <tchar.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Camera.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//定数宣言
const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //ウィンドウクラス名
const wchar_t* APP_NAME = L"サンプルゲーム";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//エントリーポイント
//アプリケーション　プログラミング　インターフェース = api
//何らかのアプリケーションを作る際に便利機能をまとめたもの
//ソフトウェア　デベロップメント　キット = sdk
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	RegisterClassEx(&wc); //クラスを登録

	//ウィンドウサイズの計算(+（表示領域をWINDOW_WIDT*WINDOW_HEIGHT
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		APP_NAME,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                //ウィンドウ幅
		winH,                //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//Direct3D初期化
	Direct3D::Initialize(winW, winH, hWnd);

	Camera::Initialize();

	Quad* q;
	q = new Quad();
	q->Initialize();
	//if (FAILED(hr))
	//{
	//	return 0;
	//}

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			//ゲームの処理
			Camera::Update();
			Direct3D::BeginDraw();

			//1度ずつ回転する変数
			//static float rot = 0;
			//rot+= 0.5;
			//XMMATRIX rmat = XMMatrixRotationY(XMConvertToRadians((float)rot));
			static float factor = 0.0;
			factor += 0.01;
			//float scale = 1.5 + sin(factor);
			//XMMATRIX smat = XMMatrixScaling(scale, scale, scale);
			////自前の描画処理を追加していく
			//XMMATRIX tmat = XMMatrixTranslation(2.0 * sin(factor), 0, 0);
			XMMATRIX tmat = XMMatrixTranslation(3.0*cos(factor), 3.0*sin(factor), 0);
			//XMMATRIX mat = rmat * smat * tmat;
			//単位行列は、数字の1と同じ
			XMMATRIX mat = XMMatrixIdentity();//Identityは単位行列という意味
			mat = tmat;
			q->Draw(mat);

			//描画処理
			Direct3D::EndDraw();

		}
	}
	//SAFE_DELETE(quad);
	/*q->Release();*/
	Direct3D::Release();
	
	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
