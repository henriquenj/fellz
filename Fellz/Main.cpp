/******************************************************************
*   Fellz by Henrique Jung and Wyllian Hossein                    *
*                           2012/2 - Class Assigment              *
*																  *
*    A game made with Cocos2D-x and RakNet                        *
*                                                                 *
******************************************************************/

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView& eglView = CCEGLView::sharedOpenGLView();
    eglView.setViewName("Fellz");
    eglView.setFrameSize(800, 600);
    // set the design resolution screen size, if you want to use Design Resoulution scaled to current screen, please uncomment next line.
    // eglView.setDesignResolutionSize(480, 320);
    return CCApplication::sharedApplication().run();
}
