#include "System.h"
#include "GraphicsEngine.h"
#include <memory>
#include "Camera.h"
#include "tkEngine.h"
#include "RenderContext.h"

//メッセージプロシージャ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
	if (imguiSystem::WindowProcHandler(hwnd, msg, wparam, lparam))return true;
#endif // _DEBUG


    if (msg == WM_DESTROY)
    {
        //OSに終了を伝える
        PostQuitMessage(0);
        return 0;
    }

    //規定の処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}


System::System():
	frame_Buffer_width_(1280),
	frame_Buffer_height_(720),
	hWnd_(NULL),
	imgui_System_()
{
	imgui_System_ = std::make_unique<imguiSystem>();
}

System::~System()
{
}

//ウィンドウメッセージをディスパッチ
bool System::DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		//ウィンドウからメッセージを受け取る
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//まだメッセージが残っている
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//ウィンドウメッセージが空になった。
			break;
		}
	}


	return msg.message != WM_QUIT;
}

//window 初期化
void System::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//ウィンドウクラスのパラメータを設定
	WNDCLASSEX wc;
    //構造体のサイズ
	wc.cbSize = sizeof(WNDCLASSEX);
	//ウィンドウのスタイル
	wc.style = CS_CLASSDC;
    //コールバック関数
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;
	//ウィンドウ クラス構造体の後に割り当てる余分なバイト数
	wc.cbClsExtra = 0;
	//ウィンドウ インスタンスの後に割り当てる余分なバイト数
	wc.cbWndExtra = 0;
	//クラスのウィンドウ プロシージャを含むインスタンスへのハンドル
	wc.hInstance = GetModuleHandle(0);
	//クラス アイコンへのハンドル
	//NULLなので、システムには既定のアイコンが表示されます。
	wc.hIcon = NULL;
	//クラス カーソルへのハンドル
	//NULLなので、アプリケーションは、マウスがアプリケーションのウィンドウに移動するたびに、カーソルの図形を明示的に設定する必要があります。
	wc.hCursor = NULL;
	//クラスの背景ブラシへのハンドル
	//NULLなので，アプリケーションはクライアント領域で描画を要求されるたびに、独自の背景を描画する必要がある
	wc.hbrBackground = NULL;
	//リソース ファイルに名前が表示されるように
	//NULLなので，クラスに属するウィンドウには既定のメニューはありません
	wc.lpszMenuName = NULL;
    //アプリケーションクラス
    wc.lpszClassName = appName;
	//ウィンドウ クラスに関連付けられている小さなアイコンへのハンドル
	//NULLなので，自動で適切なサイズのアイコンを検索する
	wc.hIconSm = NULL;
	//ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	//ウィンドウ作成
	// ウィンドウの作成。
	this->hWnd_ = CreateWindow(
		appName,				//ウィンドウクラスの名前
		appName,				//ウィンドウの名前。ウィンドウクラスの名前と別名でもよい
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい
		0,						//ウィンドウの初期X座標
		0,						//ウィンドウの初期Y座標
		frame_Buffer_width_,	//ウィンドウの幅
		frame_Buffer_height_,	//ウィンドウの高さ
		NULL,					//親ウィンドウ。ゲームでは基本的にNULLでいい
		NULL,					//メニュー
		hInstance,				//アプリケーションのインスタンス
		NULL
	);

	//window表示
	ShowWindow(this->hWnd_, nCmdShow);
}

//初期化
void System::Init(const GraphicsEngine* graphicsEngine)
{
#ifdef _DEBUG
	//imgui 初期化
	imgui_System_->Init(graphicsEngine, this->hWnd_);
#endif
}

//更新
void System::Update(const tkEngine* engine)
{
#ifdef _DEBUG
	imgui_System_->Update();
#endif
}

//描画
void System::Render(const tkEngine* engine)
{
#ifdef _DEBUG
	imgui_System_->Drow(engine->GetGraphicsEngine());
#endif // _DEBUG

}

