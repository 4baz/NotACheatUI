#include "includes.hpp"
#include "utils.hpp"

static ID3D11Device           * g_pd3dDevice           = nullptr;
static ID3D11DeviceContext    * g_pd3dDeviceContext    = nullptr;
static IDXGISwapChain         * g_pSwapChain           = nullptr;
static ID3D11RenderTargetView * g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D( HWND );
void CleanupDeviceD3D( );
void CreateRenderTarget( );
void CleanupRenderTarget( );
LRESULT __stdcall WndProc( HWND, UINT , WPARAM, LPARAM );

/*
xo1337 ImGuiBase
- Version: 3.0.0

CProject website: https://cproject.xyz
CProject discord invite: https://discord.gg/dnkdDuUtQu

What is CProject? CProject is the best community to learn more
about programming in C++, C, C# and much more! Our main goal is to 
help and teach people about ImGui. If you're interested in learning more, 
come join us and visit our website!

- Note:
    We also sell ImGui menus! If you want a custom ImGui menu design or a remake of a ImGui menu, 
    we sell it as low and affordable as 14.99 euros! 
*/




int testx1 = 0;
int testy1 = 0;
int test3;
int test4;

int movableyinnit = 60;

int epictestya = 0;
int epictestyb = 0;


int submenu = 0;
int submenuLevel;
int lastSubmenu[20];
int lastOption[20];
int ival = 0;
int optionCount;
int currentOption;
int maxOptions = 10;
bool rightPress = false;
bool leftPress = false;
bool squarePress = false;
const char* optionTextToAdd;
const char* infoText;
#define STB_IMAGE_IMPLEMENTATION

bool renderheader = false;



//Made by 4baz. Based of submenu switch commonly used for gta mod menus

/*

1. opening/closing is pepega need to fix that

2. want to get d3d11  image loading and shit for loading images into gta

3. all this code is pepega tryna retrofit//make 2take1 style submenu switch into it might give up and wait a few years till im better at coding tbh


*/


enum submenus {

    CLOSED,
    MAINMENU,
    TESTMENU,
    TESTMENUDOS,

};

void MainMenu();
void testmenu();
void testmenudos();

void submenuHandler()
{
    switch (submenu)
    {
    case MAINMENU: MainMenu(); break;
    case TESTMENU: testmenu(); break;
    case TESTMENUDOS: testmenudos(); break;

    }
}



//Change submenu function to change submenus 
void changeSubmenu(int newSubmenu)
{
    lastSubmenu[submenuLevel] = submenu;
    lastOption[submenuLevel] = currentOption;
    currentOption = 1;
    submenu = newSubmenu;
    submenuLevel++;
}
struct RGBA {
    int R;
    int G;
    int B;
    int A;
};

RGBA textmain{ 255,255,255,255 };
RGBA highlightmain{ 0,0,0,255 };


void drawtext(const char* text, float x, float y, /* float scalex, float scaley,*/ RGBA colour) {

    ImDrawList* drawtext = ImGui::GetBackgroundDrawList();

    drawtext->AddText(ImVec2(x, y), IM_COL32(colour.R, colour.G, colour.B, colour.A), text);




}

void addoption(const char* text, const char* optioninfo = NULL) {

    optionTextToAdd = text;
    optionCount++;


    //text changes colour when option selected
    if (currentOption == optionCount) {
        infoText = optioninfo;


        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            drawtext(optionTextToAdd, testx1 + 20, optionCount * 40 + 50 + testy1, highlightmain);
        }
        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption) {
            drawtext(optionTextToAdd, testx1 + 20, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlightmain);

        }

    }
    //normal text
    else {


        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            drawtext(optionTextToAdd, testx1 + 20, optionCount * 40 + 50 + testy1, textmain);
        }
        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption) {
            drawtext(optionTextToAdd, testx1 + 20, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

        }





    }



}
void AddSubmenu(const char* option, int submenu, const char* info = NULL)
{



    char buf[30];
    snprintf(buf, sizeof(buf), "%s~h~ ~m~>", option);
    addoption(buf, info);

    if (currentOption == optionCount && GetAsyncKeyState(VK_RETURN) & 1) {
        //  PLAY SUBMENU SWITCH SOUND HERE IF WANTED
        changeSubmenu(submenu);
    }

}

//If option is pressed do something
int optionPress()
{
    if (GetAsyncKeyState(VK_RETURN) & 1)
        return currentOption;
    else return 0;
}

//ID3D11ShaderResourceView* Image = nullptr;

void drawHeader()
{
    renderheader = true;
 //   fuck->Image((PVOID)Image, ImVec2(80, 80));
    // draw custom bitmap here

}


void ResetVars()
{
    drawHeader();
    rightPress = false;
    leftPress = false;
    squarePress = false;
    optionCount = NULL;
    ival = 0;
}


bool fuckTheScroller = false;
void DrawBackground() {


    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    //background
    draw->AddRectFilled(ImVec2(testx1 + 10, testy1 + 500), ImVec2(testx1 + 350, testy1 + 80), IM_COL32(0, 0, 0, 160), 0.f, 0);
    draw->AddRect(ImVec2(testx1 + 10, testy1 + 500), ImVec2(testx1 + 350, testy1 + 80), IM_COL32(0, 0, 0, 160), 0.f, 0);


    draw->AddRectFilled(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 350, testy1 + 50), IM_COL32(0, 0, 0, 255), 0.f, 0);
    //3 outer rectagles seperating
    draw->AddRect(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 116.666666667, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);
    draw->AddRect(ImVec2(testx1 + 116.666666667, testy1 + 80), ImVec2(testx1 + 233.333333333, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);
    draw->AddRect(ImVec2(testx1 + 233.333333333, testy1 + 80), ImVec2(testx1 + 350, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);

    // }//scroller

    if (!fuckTheScroller) {

        if (currentOption > maxOptions) {


            draw->AddRectFilled(ImVec2(testx1 + 10, (maxOptions * 40) + 40 + testy1), ImVec2(testx1 + 350, (maxOptions * 40) + 75 + testy1), IM_COL32(255, 255, 255, 160), 0.f, 0);//fix after

        }
        else {
            // GRAPHICS::DRAW_RECT(scrollerX, ((currentOption * 0.035f) + scrollerY), MainBackGroundWidth, 0.035f, scrollerRed, scrollerGreen, scrollerBlue, scrollerOpacity, 0);
            // draw->AddRectFilled(ImVec2(testx1 + 10, (currentOption * 40) + 10), ImVec2(testx1 + 350, (currentOption * 0.035f) + 30), IM_COL32(0, 0, 0, 255), 0.f, 0);//fix after
            draw->AddRectFilled(ImVec2(testx1 + 10, (currentOption * 40) + 40 + testy1), ImVec2(testx1 + 350, (currentOption * 40) + 75 + testy1), IM_COL32(255, 255, 255, 160160), 0.f, 0);//fix after

        }

    }



}
void horizontalscrollerHandler() {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    if (submenu == MAINMENU) {
        if (currentOption == 1) {

            if (GetAsyncKeyState(VK_UP) & 1) {
                draw->AddRectFilled(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 116.666666667, testy1 + 50), IM_COL32(255, 255, 255, 160), 0.f, 0);

                fuckTheScroller = true;
            }



        }
        else {
            fuckTheScroller = false;

        }
    }

}
//make option counter example from old native gta5

/*
//Checks whether option count is bigger than 10
    if (optionCount > maxOptions - 5)
        UI::END_TEXT_COMMAND_DISPLAY_TEXT(CounterX - 0.006f, CounterY,0);
    //If not draw normally
    else
        UI::END_TEXT_COMMAND_DISPLAY_TEXT(CounterX, CounterY,0);
*/


void OpenMonitor()
{

    // optionCounter();
    DrawBackground();
    //   DisableControls();

    if (GetAsyncKeyState(VK_UP) & 1) {

        //  PlayFrontendSound("NAV_UP_DOWN");
        currentOption--;

        //     if (currentOption > 1) {
         //        currentOption--;

         //    }



        if (currentOption < 1)
            currentOption = optionCount;

    }

    else if (GetAsyncKeyState(VK_DOWN) & 1) {

        //   PlayFrontendSound("NAV_UP_DOWN");
        currentOption++;

        if (currentOption > optionCount)
            currentOption = 1;

    }

    else if (GetAsyncKeyState(VK_BACK) & 1) {

        //  PlayFrontendSound("BACK");

        if (submenu == MAINMENU) {
            submenu = CLOSED;
        }

        else {
            submenu = lastSubmenu[submenuLevel - 1];
            currentOption = lastOption[submenuLevel - 1];
            submenuLevel--;
        }

    }
    else if (currentOption == 1 && (GetAsyncKeyState(VK_UP) & 1)) {

        currentOption = 0;

    }


}

//Monitors our menu, whenever it is closed
void ClosedMonitor()
{
    if (GetAsyncKeyState(VK_F8) & 1) {

        //  PlayFrontendSound("SELECT");
          //	is_user_authed();
        submenu = MAINMENU;
        submenuLevel = 0;
        currentOption = 1;
        renderheader = false;
    }
}


void mainmenuloop() {
    //	while (g_Running) {
        //	Big::WAIT(0);
            //	static bool first_time = false;
            //	if (!first_time)
            //	{
                //	Features();
            //	}
        //	featureloop();
    if (submenu != CLOSED) {
        OpenMonitor();
        ResetVars();
        submenuHandler();
        //   horizontalscrollerHandler();

          // DisableControls();
    }

    else ClosedMonitor();
    //	}
}






void MainMenu() {
    addoption("Test");
    addoption("Test2");
    addoption("Test3");
    addoption("Test4");
    addoption("Test5");
    AddSubmenu("Test SUbmenu", TESTMENU);
    addoption("Test6");
    addoption("Test7");
    addoption("Test8");
    addoption("Test9");
    addoption("Test10");
    addoption("Test11");
    addoption("Test12");

}

void testmenu() {
    addoption("Test");
    addoption("Test2");
    addoption("Test3");
    addoption("Test4");
    addoption("Test5");
    AddSubmenu("Test SUbmenu 2", TESTMENUDOS);

}
void testmenudos() {

}




































void Colors( ) {
    ImGuiStyle& style = ImGui::GetStyle( );

    style.ScrollbarRounding = 0;
    style.WindowMinSize = ImVec2( 480 , 295 );

    style.Colors[ImGuiCol_WindowBg] = ImColor( 16 , 16 , 16 );
    style.Colors[ImGuiCol_ChildBg] = ImColor( 24 , 24 , 24 );
    style.Colors[ImGuiCol_Text] = ImColor( 255 , 255 , 255 );
    style.Colors[ImGuiCol_CheckMark] = ImColor( 255 , 255 , 255 );

    style.Colors[ImGuiCol_Header] = ImColor( 30 , 30 , 30 );
    style.Colors[ImGuiCol_HeaderActive] = ImColor( 28 , 28 , 28 );
    style.Colors[ImGuiCol_HeaderHovered] = ImColor( 28 , 28 , 28 );

    style.Colors[ImGuiCol_Button] = ImColor( 31 , 31 , 31 );
    style.Colors[ImGuiCol_ButtonActive] = ImColor( 34 , 34 , 34 );
    style.Colors[ImGuiCol_ButtonHovered] = ImColor( 34 , 34 , 34 );

    style.Colors[ImGuiCol_Border] = ImColor( 0 , 0 , 0 , 255 );
}
ID3D11ShaderResourceView* Image = nullptr;

int StartRendering( ) 
{   
    const auto title = utils::random_string( );
    utils::spoof_exe( false );

    WNDCLASSEX wc = 
    { 
        sizeof( WNDCLASSEX ),
        CS_CLASSDC,
        WndProc, 
        0, 
        0, 
        0, 
        0, 
        0, 
        0, 
        0, 
        title.c_str(),
        0
    };

    RegisterClassExA( &wc );
    HWND hwnd = CreateWindowExA(0, wc.lpszClassName , title.c_str() , WS_SYSMENU , 100 , 100 , 1280 , 800 , 0 , 0 , wc.hInstance , 0 );

    if ( !CreateDeviceD3D( hwnd ) )
    {
        CleanupDeviceD3D( );
        UnregisterClassA( wc.lpszClassName , wc.hInstance );

        return 1;
    }



  //  D3DX11_IMAGE_LOAD_INFO info;
  //  ID3DX11ThreadPump* pump{ nullptr };
 //   D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rawData, sizeof(rawData), &info,
  //      pump, &Image, 0);



    D3DX11_IMAGE_LOAD_INFO info;
    ID3DX11ThreadPump* pump{ nullptr };
    D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rawData, sizeof(rawData), &info,
        pump, &Image, 0);

    //drawHeader(*Image);


   



    ShowWindow( GetConsoleWindow( ) , SW_HIDE );
    ShowWindow( hwnd , SW_SHOWDEFAULT );
    UpdateWindow( hwnd );

    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); 

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    io.WantSaveIniSettings = false;
    io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Tahoma.ttf" , 16.0f );

    Colors( );










    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( g_pd3dDevice , g_pd3dDeviceContext );

    const auto clear_color = ImVec4( 0.45f , 0.55f , 0.60f , 1.00f );
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    while ( true )
    {
        MSG msg;

        while ( PeekMessageA( &msg , NULL , 0U , 0U , PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessageA( &msg );

            if ( msg.message == WM_QUIT )
                break;
        }

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );




     





      



        ImGui::NewFrame( );
       ImGui::Begin("Hello world!");
        ImGui::Image((PVOID)Image, ImVec2(80, 80));
        ImDrawList* fuck = ImGui::GetBackgroundDrawList();

        if (renderheader) {
            fuck->AddImage((PVOID)Image, ImVec2(testx1 + 10, testy1 + 50), ImVec2(testx1 + 350, testy1 -10));
        }
       
        ImGui::End();

     ImGui::Begin("Window");
     {
         ImGui::Text("Hello, Dear ImGUI!");
         ImGui::SliderInt("X###sizex", &testx1, 0, 2000);
         ImGui::SliderInt("X###sizy", &testy1, 0, 2000);
         ImGui::SliderInt("XMovableY###MovY", &movableyinnit, 0, 2000);
         ImGui::Text("------------------------");
         ImGui::SliderInt("yyyMovableY###MoyyvY", &epictestya, 0, 2000);
         ImGui::SliderInt("yyyMovableY###MovyyY", &epictestyb, 0, 2000);

     }
   
     mainmenuloop();
     ImGui::End();

        // 480 x 295
    

        ImGui::Render( );
       
        g_pd3dDeviceContext->OMSetRenderTargets( 1 , &g_mainRenderTargetView , NULL );
        g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView , clear_color_with_alpha );
       
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        g_pSwapChain->Present(0, 0); 
    }

    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    DestroyWindow( hwnd );
    UnregisterClassA( wc.lpszClassName , wc.hInstance );

    return 0;
}


bool CreateDeviceD3D( HWND hWnd )
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd , sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    if ( D3D11CreateDeviceAndSwapChain( NULL , D3D_DRIVER_TYPE_HARDWARE , NULL , createDeviceFlags , featureLevelArray , 2 , D3D11_SDK_VERSION , &sd , &g_pSwapChain , &g_pd3dDevice , &featureLevel , &g_pd3dDeviceContext ) != S_OK )
        return false;

    CreateRenderTarget( );
    return true;
}

void CleanupDeviceD3D( )
{
    CleanupRenderTarget( );
    if ( g_pSwapChain ) { g_pSwapChain->Release( ); g_pSwapChain = NULL; }
    if ( g_pd3dDeviceContext ) { g_pd3dDeviceContext->Release( ); g_pd3dDeviceContext = NULL; }
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
}

void CreateRenderTarget( )
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer( 0 , IID_PPV_ARGS( &pBackBuffer ) );
    g_pd3dDevice->CreateRenderTargetView( pBackBuffer , NULL , &g_mainRenderTargetView );
    pBackBuffer->Release( );
}

void CleanupRenderTarget( )
{
    if ( g_mainRenderTargetView ) { g_mainRenderTargetView->Release( ); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

LRESULT WINAPI WndProc( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd , msg , wParam , lParam ) )
        return true;

    switch ( msg )
    {
    case WM_SIZE:
        if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
        {
            CleanupRenderTarget( );
            g_pSwapChain->ResizeBuffers( 0 , (UINT)LOWORD( lParam ) , (UINT)HIWORD( lParam ) , DXGI_FORMAT_UNKNOWN , 0 );
            CreateRenderTarget( );
        }
        return 0;
    case WM_SYSCOMMAND:
        if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
            return 0;
        break;
    case WM_DESTROY:
        ExitProcess( 0 );
        PostQuitMessage( 0 );
        return 0;
    }
    return ::DefWindowProc( hWnd , msg , wParam , lParam );
}