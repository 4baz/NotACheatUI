#include "includes.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <functional>

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



//menu settings
int testx1 = 0;
int testy1 = 47;
bool opensettings = false;



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

bool testbool = false;
bool testbool2 = false;

ID3D11ShaderResourceView* Image = nullptr;//pointer to image thats loaded in imgui main


//Made by 4baz. Based of submenu switch commonly used for gta mod menus

/*

1. opening/closing is pepega need to fix that


3. all this code is pepega tryna retrofit//make 2take1 style submenu switch into it might give up and wait a few years till im better at coding tbh


*/


enum submenus {

    CLOSED,
    MAINMENU,
    SETTINGS,
    TESTMENUDOS,

};

void MainMenu();
void settings();
void testmenudos();

void submenuHandler()
{
    switch (submenu)
    {
    case MAINMENU: MainMenu(); break;
    case SETTINGS: settings(); break;
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

//rgb structure to set  colours
struct RGBA {
    int R;
    int G;
    int B;
    int A;
};


//menu colours 
RGBA textmain{ 255,255,255,255 };
RGBA highlighttextmain{ 0,0,0,255 };

//submenu arrows

RGBA submenuarrows{ 255,255,255,255 };
RGBA highlightsubmenuarrows{ 0,0,0,255 };


//seperatorrect

RGBA seperatorrect{ 0,0,0,255 };
RGBA seperatorrectoutline{ 255,255,255,255 };

//scroller

RGBA scrollerrect{ 255,255,255,160 };


//background

RGBA backgroundrect{ 0,0,0,160 };
RGBA backgroundrectoutline{ 0,0,0,160 };



//parse data to low level addtext imgui function
void drawtext(const char* text, float x, float y, /* float scalex, float scaley,*/ RGBA colour) {

    ImDrawList* drawtext = ImGui::GetBackgroundDrawList();

    drawtext->AddText(ImVec2(x, y), IM_COL32(colour.R, colour.G, colour.B, colour.A), text);




}
//break option with centred (ish)
void addbreak(const char* breaktext, const char* optioninfo = NULL) {

    optionTextToAdd = breaktext;
    optionCount++;


    int toChar = strlen(breaktext);
   

  //  const char* str = breaktext;
       
  

    //text changes colour when option selected
    if (currentOption == optionCount) {
        infoText = optioninfo;


        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            drawtext(optionTextToAdd, testx1 + 160- toChar, optionCount * 40 + 50 + testy1, highlighttextmain);
        }
        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption) {
            drawtext(optionTextToAdd, testx1 + 160- toChar, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

        }

    }
    //normal text
    else {


        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            drawtext(optionTextToAdd, testx1 + 160 - toChar, optionCount * 40 + 50 + testy1, textmain);
        }
        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption) {
            drawtext(optionTextToAdd, testx1 + 160 - toChar, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

        }





    }



}
//base add option that every other option is based off
void addoption(const char* text, const char* optioninfo = NULL) {

    optionTextToAdd = text;
    optionCount++;


    //text changes colour when option selected
    if (currentOption == optionCount) {
        infoText = optioninfo;


        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            drawtext(optionTextToAdd, testx1 + 20, optionCount * 40 + 50 + testy1, highlighttextmain);
        }
        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption) {
            drawtext(optionTextToAdd, testx1 + 20, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

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
//like addoption but can execute code in the function
bool AddFunction(const char* text, std::function<void()> function,const char* optioninfo = NULL) {

    addoption(text,optioninfo);

    if (currentOption == optionCount && GetAsyncKeyState(VK_RETURN) & 1) {
        function();
       // PlayFrontendSound("SELECT");
        return true;
    }

    return false;


    /*Example use case:   
                   if (AddFunction("Classic Five", [] {

					        int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
					        VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
					        VEHICLE::SET_VEHICLE_WHEEL_TYPE(VehID, 1);
					        VEHICLE::SET_VEHICLE_MOD(VehID, 23, 0, 0);
					
                    }));
  
    */


}
bool AddInt(const char* option, int* inttochange, int min, int max, int increments, std::function<void()> functiontorunifenter, const char* info = NULL)
{
    addoption(option, info);

    char buf[30];

    snprintf(buf, sizeof(buf), "[%i]", *inttochange);


    if (currentOption == optionCount) {

        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 295, optionCount * 40 + 50 + testy1, highlighttextmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 295, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

    }
    else {
        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 295, optionCount * 40 + 50 + testy1, textmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 295, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

    }

    if (currentOption == optionCount) {

        if (GetAsyncKeyState(VK_RIGHT) & 1) {

          //  PlayFrontendSound("NAV_UP_DOWN");

            if (*inttochange >= max)
                *inttochange = min;

            else
                *inttochange = *inttochange + increments;
        }

        else if (GetAsyncKeyState(VK_LEFT) & 1) {
        //    PlayFrontendSound("NAV_UP_DOWN");

            if (*inttochange <= min)
                *inttochange = max;

            else
                *inttochange = *inttochange - increments;
        }
    }
   // if (currentOption == optionCount && GetAsyncKeyState(VK_RETURN) & 1) 
      if (currentOption == optionCount && (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) & 1) {
        functiontorunifenter();
        // PlayFrontendSound("SELECT");
        return true;
    }

    return false;
}
bool AddFloat(const char* option, float* floattochange, float min, float max, int increments, std::function<void()> functiontorunifenter, const char* info = NULL)
{
    addoption(option, info);

    char buf[30];

    snprintf(buf, sizeof(buf), "[%g]", *floattochange);


    if (currentOption == optionCount) {

        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 295, optionCount * 40 + 50 + testy1, highlighttextmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 295, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

    }
    else {
        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 295, optionCount * 40 + 50 + testy1, textmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 295, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

    }

    if (currentOption == optionCount) {
        if (GetAsyncKeyState(VK_RIGHT) & 1) {

        //    PlayFrontendSound("NAV_UP_DOWN");
            if (*floattochange >= max)
                *floattochange = min;
            else
                *floattochange = *floattochange + increments;

        }

        else if (GetAsyncKeyState(VK_LEFT) & 1) {

       //     PlayFrontendSound("NAV_UP_DOWN");
            if (*floattochange <= min)
                *floattochange = max;
            else
                *floattochange = *floattochange - increments;
        }
    }
    if (currentOption == optionCount && (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) & 1) {
        functiontorunifenter();
        // PlayFrontendSound("SELECT");
        return true;
    }

    return false;
}
void AddString(const char* option,const char* chartochange, int *var, int min, int max, const char* info = NULL)
{

    char buf[30];

    sprintf(buf, "< %s >", chartochange);

    addoption(option, info);

    if (currentOption == optionCount) {

        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 280, optionCount * 40 + 50 + testy1, highlighttextmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 280, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

    }
    else {
        if (currentOption <= maxOptions && optionCount <= maxOptions)
            drawtext(buf, testx1 + 280, optionCount * 40 + 50 + testy1, textmain);

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
            drawtext(buf, testx1 + 280, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

    }

    if (currentOption == optionCount) {
        if (GetAsyncKeyState(VK_RIGHT) & 1) {

        //    PlayFrontendSound("NAV_UP_DOWN");
            if (*var >= max)
                *var = min;
            else
                *var = *var + 1;

        }
        else if (GetAsyncKeyState(VK_LEFT) & 1) {

      //      PlayFrontendSound("NAV_UP_DOWN");
            if (*var <= min)
                *var = max;
            else
                *var = *var - 1;
        }
    }
  ///  if (currentOption == optionCount && (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) & 1) {
  //      functiontorunifenter();
        // PlayFrontendSound("SELECT");
   //     return true;
   // }

   
}
//overload to allow function execution
void AddString(char* optiontext,const char* option, int* var, int min, int max, std::function<void()> callback, const char* info = NULL)
{
    AddString(optiontext, option, var, min, max, info);
    if (currentOption == optionCount && (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) & 1)
        callback();
}



void AddToggle(const char* text, bool* toggle,const char* optioninfo=NULL) {

    addoption(text, optioninfo);

    //If Enter is pressed it changes the bool to On/Off
    if (currentOption == optionCount && GetAsyncKeyState(VK_RETURN) & 1) {
      //  PlayFrontendSound("SELECT");
        *(bool*)toggle = !*(bool*)toggle;
    }


    //if toggle is presssed change toggle on menu ting
    if (*(bool*)toggle) {
        if (currentOption == optionCount) {
        
            if (currentOption <= maxOptions && optionCount <= maxOptions)
                drawtext("ON", testx1 + 300, optionCount * 40 + 50 + testy1, highlighttextmain);
          
            else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
                drawtext("ON", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

        }
        else {
            if (currentOption <= maxOptions && optionCount <= maxOptions)
                drawtext("ON", testx1 + 300, optionCount * 40 + 50 + testy1, textmain);

            else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
                drawtext("ON", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);

        }



    }
    else {
        if (currentOption == optionCount) {


            if (currentOption <= maxOptions && optionCount <= maxOptions)
                drawtext("OFF", testx1 + 300, optionCount * 40 + 50 + testy1, highlighttextmain);

            else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
                drawtext("OFF", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);

        }
        else {
            if (currentOption <= maxOptions && optionCount <= maxOptions)
                drawtext("OFF", testx1 + 300, optionCount * 40 + 50 + testy1, textmain);

            else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
                drawtext("OFF", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);
        }

    }


}







void AddSubmenu(const char* option, int submenu, const char* info = NULL)
{
    int  linetest3 = -240;//too lazy to change values so this is now permanent integer


    char buf[30];
    snprintf(buf, sizeof(buf), "%s", option);
    addoption(buf, info);

    if (currentOption == optionCount && GetAsyncKeyState(VK_RETURN) & 1) {
        //  PLAY SUBMENU SWITCH SOUND HERE IF WANTED
        changeSubmenu(submenu);
    }


    //select option list

    //professional coding right here boys look at that maths 
    ImDrawList* subarrow = ImGui::GetBackgroundDrawList();

  /*  if (currentOption == optionCount) {
        if (currentOption <= maxOptions && optionCount <= maxOptions) {
            subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount * 40) + testy1 + 298) + linetest3), IM_COL32(highlightsubmenuarrows.R, highlightsubmenuarrows.G, highlightsubmenuarrows.B, highlightsubmenuarrows.A));

        }
        else {
            subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 298) + linetest3), IM_COL32(highlightsubmenuarrows.R, highlightsubmenuarrows.G, highlightsubmenuarrows.B, highlightsubmenuarrows.A));

        }
    }
    else {
        if (currentOption <= maxOptions && optionCount <= maxOptions) {
           subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount * 40) + testy1 + 298) + linetest3), IM_COL32(submenuarrows.R, submenuarrows.G, submenuarrows.B, submenuarrows.A));

        }
        else {
            subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 298) + linetest3), IM_COL32(submenuarrows.R, submenuarrows.G, submenuarrows.B, submenuarrows.A));

        }

    }*/


    if (currentOption == optionCount) {

        if (currentOption <= maxOptions && optionCount <= maxOptions)
        //    drawtext(">", testx1 + 300, optionCount * 40 + 50 + testy1, highlighttextmain);
        subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount * 40) + testy1 + 298) + linetest3), IM_COL32(highlightsubmenuarrows.R, highlightsubmenuarrows.G, highlightsubmenuarrows.B, highlightsubmenuarrows.A));

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
         //   drawtext(">", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, highlighttextmain);
            subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 298) + linetest3), IM_COL32(highlightsubmenuarrows.R, highlightsubmenuarrows.G, highlightsubmenuarrows.B, highlightsubmenuarrows.A));

    }
    else {
        if (currentOption <= maxOptions && optionCount <= maxOptions)
          //  drawtext(">", testx1 + 300, optionCount * 40 + 50 + testy1, textmain);
            subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount * 40) + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount * 40) + testy1 + 298) + linetest3), IM_COL32(submenuarrows.R, submenuarrows.G, submenuarrows.B, submenuarrows.A));

        else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
          //  drawtext(">", testx1 + 300, (optionCount - (currentOption - maxOptions)) * 40 + 50 + testy1, textmain);
        subarrow->AddTriangleFilled(ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 306) + linetest3), ImVec2(testx1 + 322, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 290) + linetest3), ImVec2(testx1 + 331, ((optionCount - (currentOption - maxOptions)) * 40 + testy1 + 298) + linetest3), IM_COL32(submenuarrows.R, submenuarrows.G, submenuarrows.B, submenuarrows.A));

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
void AddTitle(const char* titletext) {

    ImDrawList* title = ImGui::GetBackgroundDrawList();

    title->AddText(ImVec2(testx1+ 90,testy1+55),IM_COL32(textmain.R, textmain.G, textmain.B, textmain.A),titletext);

}
void drawHeader()
{
  
    ImDrawList* fuck = ImGui::GetBackgroundDrawList();




     //   fuck->AddImage((PVOID)Image, ImVec2(testx1 + 10, testy1 + 50), ImVec2(testx1 + 350, testy1 - 30));

        fuck->AddImage((PVOID)Image, ImVec2(testx1 + 10, testy1 - 30), ImVec2(testx1 + 350, testy1 + 50));

        //white rectangles around header break
        fuck->AddRectFilled(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 350, testy1 + 50), IM_COL32(seperatorrect.R, seperatorrect.G, seperatorrect.B, seperatorrect.A), 0.f, 0);

        fuck->AddRect(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 233.333333333, testy1 + 50), IM_COL32(seperatorrectoutline.R, seperatorrectoutline.B, seperatorrectoutline.B, seperatorrectoutline.A), 0.f, 0);
        fuck->AddRect(ImVec2(testx1 + 233.333333333, testy1 + 80), ImVec2(testx1 + 350, testy1 + 50), IM_COL32(seperatorrectoutline.R, seperatorrectoutline.B, seperatorrectoutline.B, seperatorrectoutline.A), 0.f, 0);
      



        //render option counter
        std::string str1 = std::to_string(currentOption);
        std::string str2 = std::to_string(optionCount);
        std::string slash = "/";
        std::string opcnt = str1 + slash + str2;
       const char * printoptioncount = opcnt.c_str();

        if (optionCount > maxOptions - 5) {
            fuck->AddText(ImVec2(testx1 + 310, testy1 + 55), IM_COL32(textmain.R, textmain.G, textmain.B, textmain.A), printoptioncount);

        }
        else {
            fuck->AddText(ImVec2(testx1 + 320, testy1 + 55), IM_COL32(textmain.R, textmain.G, textmain.B, textmain.A), printoptioncount);

        }
     

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
    draw->AddRectFilled(ImVec2(testx1 + 10, testy1 + 500), ImVec2(testx1 + 350, testy1 + 80), IM_COL32(backgroundrect.R, backgroundrect.G, backgroundrect.B, backgroundrect.A), 0.f, 0);
    draw->AddRect(ImVec2(testx1 + 10, testy1 + 500), ImVec2(testx1 + 350, testy1 + 80), IM_COL32(backgroundrectoutline.R, backgroundrectoutline.G, backgroundrectoutline.B, backgroundrectoutline.A), 0.f, 0);


    //3 outer rectagles seperating
  //  draw->AddRect(ImVec2(testx1 + 10, testy1 + 80), ImVec2(testx1 + 116.666666667, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);
   // draw->AddRect(ImVec2(testx1 + 116.666666667, testy1 + 80), ImVec2(testx1 + 233.333333333, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);
  //  draw->AddRect(ImVec2(testx1 + 233.333333333, testy1 + 80), ImVec2(testx1 + 350, testy1 + 50), IM_COL32(255, 255, 255, 255), 0.f, 0);

    // }//scroller

    if (!fuckTheScroller) {

        if (currentOption > maxOptions) {


            draw->AddRectFilled(ImVec2(testx1 + 10, (maxOptions * 40) + 40 + testy1), ImVec2(testx1 + 350, (maxOptions * 40) + 75 + testy1), IM_COL32(scrollerrect.R, scrollerrect.G, scrollerrect.B, scrollerrect.A), 0.f, 0);//fix after

        }
        else {
            // GRAPHICS::DRAW_RECT(scrollerX, ((currentOption * 0.035f) + scrollerY), MainBackGroundWidth, 0.035f, scrollerRed, scrollerGreen, scrollerBlue, scrollerOpacity, 0);
            // draw->AddRectFilled(ImVec2(testx1 + 10, (currentOption * 40) + 10), ImVec2(testx1 + 350, (currentOption * 0.035f) + 30), IM_COL32(0, 0, 0, 255), 0.f, 0);//fix after
            draw->AddRectFilled(ImVec2(testx1 + 10, (currentOption * 40) + 40 + testy1), ImVec2(testx1 + 350, (currentOption * 40) + 75 + testy1), IM_COL32(scrollerrect.R, scrollerrect.G, scrollerrect.B, scrollerrect.A), 0.f, 0);//fix after

        }

    }



}


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
    else if (GetAsyncKeyState(VK_F8) & 1) {
        if (submenu == MAINMENU) {
            submenu = CLOSED;
        }
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
       

          // DisableControls();
    }

    else ClosedMonitor();
   
}





int  linetest1 = 350;
float  linetest2 = 300;
int page;
const char* pages[]{ "Page1","Page2","Page3","Page4" };
void MainMenu() {
    AddTitle("Main Menu");
    addoption("Test");
    AddToggle("test toggle",&testbool);
    AddToggle("test toggle2", &testbool2);
    addbreak("[Test Break]");
    if (AddInt("integer test", &linetest1,1,10,1, [] {}));
    if (AddFloat("float test", &linetest2, 1.111, 10.160, 1.116, [] {}));
    AddString("Test String", page[pages], &page, 0, 3);
    AddFunction("Open ClickUI Settings menu", [] {
        opensettings = true;
        });
    AddSubmenu("Settings Submenu", SETTINGS);
    addoption("Test3");

   

}

void settingsmenu() {
    if (opensettings) {

        ImGui::Begin("Settings");
        ImGui::Text("Change menu X & Y Pos");
        ImGui::SliderInt("X###sizex", &testx1, 0, 2000);
        ImGui::SliderInt("X###sizy", &testy1, 0, 2000);


        //  ImGui::ColorButton();
        if(ImGui::Button("Close settings")){
            opensettings = false;
        }
        ImGui::End();


    }
 
}


void settings() {
    AddTitle("Settings");
    if (AddInt("integer test", &testx1, -1000, 2000, 25, [] {}));
    if (AddInt("integer test", &testy1, -1000, 2000, 25, [] {}));


}
void testmenudos() {
    AddTitle("Menu 3");

    addoption("Test");
    addoption("Test2");
    addoption("Test3");
    AddSubmenu("Back to main menu", MAINMENU);
    AddSubmenu("Back to main menu", MAINMENU);
    AddSubmenu("Back to main menu", MAINMENU);
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






    D3DX11_IMAGE_LOAD_INFO info;
    ID3DX11ThreadPump* pump{ nullptr };
    D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rawData, sizeof(rawData), &info,
        pump, &Image, 0);



   



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




     





      



         ImGui::NewFrame();
    

     ImGui::Begin("Test Window");
     {
         ImGui::Text("Hello, Dear ImGUI!");
         ImGui::SliderInt("X###sizex", &testx1, 0, 2000);
         ImGui::SliderInt("X###sizy", &testy1, 0, 2000);
         ImGui::SliderInt("XMovableY###MovY", &movableyinnit, 0, 2000);
         ImGui::Text("------------------------");
         ImGui::SliderInt("yyyMovableY###MoyyvY", &epictestya, 0, 2000);
         ImGui::SliderInt("yyyMovableY###MovyyY", &epictestyb, 0, 2000);
      
         ImGui::Text("------------------------");

         ImGui::SliderInt("yyMovableY###MofyvY", &linetest1, 0, 2000);
         ImGui::SliderFloat("yyMovableY###MoyyY", &linetest2, 0, 2000);
       //  ImGui::SliderInt("yyMovableY###MoyhyvY", &linetest3, -1000, 10);
       //  ImGui::SliderInt("yyMovableY###MovyhyY", &linetest4, -10, 10);
     }
   
     mainmenuloop();
     settingsmenu();
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