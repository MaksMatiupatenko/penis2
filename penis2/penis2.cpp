#include <windows.h>
#include <fstream>
#include <bitset>
#include <cmath>

#include "GLLOADER.hpp"
#define WGL_WGLEXT_PROTOTYPES
#include "glext.h"
#include "TRIANGLEARRAY.hpp"
#include "TEXTURE.h"
#include "TIME.hpp"
#include "PLAYER.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STR_SIZE 100

LRESULT CALLBACK MainWinProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);


static const PIXELFORMATDESCRIPTOR pfd =
{
    /* nSize          */ sizeof(PIXELFORMATDESCRIPTOR),
    /* nVersion       */ 1,
    /* dwFlags        */ PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    /* iPixelType     */ PFD_TYPE_RGBA,
    /* cColorBits     */ 32,
    /* c...Bits|Shift */ 0, 0, 0, 0, 0, 0, 0, 0,
    /* cAccum...Bits  */ 0, 0, 0, 0, 0,
    /* cDepthBits     */ 16,
    /* cStencilBits   */ 0,
    /* cAuxBuffers    */ 0,
    /* iLayerType     */ PFD_MAIN_PLANE,
    /* bReserved      */ 0,
    /* dw...Mask      */ 0, 0, 0
};

static const int context_attributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 0,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
};

using PFNWGLCREATECONTEXTATRIBSARBPROC = HGLRC(__cdecl*)(HDC, INT_PTR, const int*);

HGLRC createContext(HWND hWindow) {
    const HDC canvas = GetDC(hWindow);

    HGLRC gl = NULL;
    const int format = ChoosePixelFormat(canvas, &pfd);
    if (format != 0) {
        SetPixelFormat(canvas, format, &pfd);

        const HGLRC gl_boostrap = wglCreateContext(canvas);
        wglMakeCurrent(canvas, gl_boostrap);

        gl = reinterpret_cast<PFNWGLCREATECONTEXTATRIBSARBPROC>
            (wglGetProcAddress("wglCreateContextAttribsARB"))
            (canvas, NULL, context_attributes);
        wglMakeCurrent(canvas, gl);
        wglDeleteContext(gl_boostrap);
    }

    ReleaseDC(hWindow, canvas);
    return gl;
}

void destructContext(HWND hWindow, HGLRC gl) {
    const HDC hdc = GetDC(hWindow);
    wglMakeCurrent(hdc, NULL);
    ReleaseDC(hWindow, hdc);

    wglDeleteContext(gl);
}

PLAYER player;

HGLRC context;

GLTXTR tex;
GLTXTR tex2;
GLTXTR tex3;
GLRTXTR rtex;
Camera camera;

int wWidth, wHeigth;

TIME_T prtime;
float timeDiff;

void yaSosuPenis(HWND hwindow);
void processAsyncInput() {
    if (GetAsyncKeyState('W')) {
        camera.move(0, timeDiff);
    }
    if (GetAsyncKeyState('S')) {
        camera.move(0, -timeDiff);
    }
    if (GetAsyncKeyState('A')) {
        camera.move(-timeDiff, 0);
    }
    if (GetAsyncKeyState('D')) {
        camera.move(timeDiff, 0);
    }
    if (GetAsyncKeyState('Q')) {
        camera.rotate(timeDiff);
    }
    if (GetAsyncKeyState('E')) {
        camera.rotate(-timeDiff);
    }
    if (GetAsyncKeyState('Z')) {
        camera.scale(exp(timeDiff));
    }
    if (GetAsyncKeyState('X')) {
        camera.scale(exp(-timeDiff));
    }
     /*if (wParam == 'W') {
         player.move(timeDiff * player.movementSpeed);
         camera.move(0, timeDiff * 100);
        }
     else if (wParam == 'S') {
     player.move(-timeDiff * player.movementSpeed);
     camera.move(0, -timeDiff * 100);
        }
     else if (wParam == 'A') {
     player.rotate(timeDiff * player.rotationSpeed);
     camera.move(-timeDiff * 100, 0);
        }
     else if (wParam == 'D') {
     player.rotate(-timeDiff * player.rotationSpeed);
     camera.move(timeDiff * 100, 0);
        }*/
}

bool windowOpen = true;
float MAX_FPS = 60;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCommandLine, int nCommandShow) {
    /*                      */
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(lpCommandLine);

    WNDCLASS windowClass;
    {
        windowClass.style = 0;
        windowClass.lpfnWndProc = MainWinProc;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = hInstance;
        windowClass.hIcon = NULL;
        windowClass.hCursor = NULL;
        windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + -2);
        windowClass.lpszMenuName = NULL;
        windowClass.lpszClassName = L"windowClass";
    }
    if (!RegisterClass(&windowClass)) {
        return FALSE;
    }

    HWND hWindow;
    {
        HWND hWindow1 = CreateWindow(L"windowClass",
                                     L"windowTitle",
                                     WS_OVERLAPPEDWINDOW,
                                     0,
                                     0,
                                     0,
                                     0,
                                     NULL,
                                     NULL,
                                     hInstance,
                                     NULL);

        HMONITOR hmon = MonitorFromWindow(hWindow1,
                                          MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        if (!GetMonitorInfo(hmon, &mi)) return NULL;

        wWidth = mi.rcMonitor.right - mi.rcMonitor.left;
        wHeigth = mi.rcMonitor.bottom - mi.rcMonitor.top;
        hWindow = CreateWindow(L"windowClass",
                               L"windowTitle",
                               WS_POPUP | WS_VISIBLE,
                               mi.rcMonitor.left,
                               mi.rcMonitor.top,
                               wWidth,
                               wHeigth,
                               NULL, NULL, hInstance, 0);
    }


    context = createContext(hWindow);

    initOpenGl();
    initShaders();

    tex.open("pic.png", GL_RGBA);
    tex2.open("pic2.png", GL_RGBA);
    tex3.create(GL_RGBA, wWidth, wHeigth, GL_RGBA, NULL);
    rtex.create();
    rtex.createTexture(GL_RGBA, wWidth, wHeigth);
    camera.setViewArea((float)wWidth / wHeigth, 1);

    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);

    setTime(prtime);

    MSG msg;
    while (windowOpen) {
        timeDiff = getTimeDiff(prtime);
        if (timeDiff >= 1 / MAX_FPS) {
            debug << 1 / timeDiff << '\n';

            setTime(prtime);
            processAsyncInput();
            yaSosuPenis(hWindow);
            GL_SHADERS_ORDER_HANDLER.clear();
        }
        

        while (PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

void yaSosuPenis(HWND hWindow) {
    TRARR trarr;
    trarr.push(vec2f(0, 0.9), GLBLACK, vec2f(0, 0));
    trarr.push(vec2f(0.9, -0.9), GLBLACK, vec2f(0, 1));
    trarr.push(vec2f(-0.9, -0.9), GLBLACK, vec2f(1, 0));
    trarr.create();

    TRARR trarr2;
    trarr2.push(vec2f(-1, -1), GLBLACK, vec2f(0, 1));
    trarr2.push(vec2f(1, -1), vec3f(0, 0, 0), vec2f(1, 1));
    trarr2.push(vec2f(1, 1), vec3f(0, 0, 0), vec2f(1, 0));
    trarr2.push(vec2f(-1, -1), vec3f(0, 0, 0), vec2f(0, 1));
    trarr2.push(vec2f(-1, 1), vec3f(0, 0, 0), vec2f(0, 0));
    trarr2.push(vec2f(1, 1), vec3f(0, 0, 0), vec2f(1, 0));
    trarr2.create();

    //debug << camera.getPos().x << ' ' << camera.getPos().y << '\n';
    //debug << camera.getPos().x << ' ' << camera.getPos().y << '\n';

    rtex.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    textureApplier.setUniform("tex", tex);
    textureApplier.setActiveShader();
    //trarr.draw(textureApplier, rotatem(getCurTime()), camera);
    //trarr.draw(sprg, mat3f());
    rtex.unbind();
    textureApplier.setInactive();

    glClear(GL_COLOR_BUFFER_BIT);
    //textureApplier.setUniform("tex", rtex.getTexture());
    textureApplier.setUniform("tex", tex);
    textureApplier.setActiveShader();
    trarr.draw(textureApplier, mat3f(), camera);
    textureApplier.setInactive();

    SwapBuffers(GetDC(hWindow));
}


LRESULT CALLBACK MainWinProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        destructContext(hWindow, context);
        PostQuitMessage(0);
        tex.destruct();
        rtex.destruct();
        tex2.destruct();
        tex3.destruct();
        windowOpen = false;
        return 0;
    }
    if (message == WM_KEYDOWN) {
        if (wParam == VK_ESCAPE) {
            SendMessage(hWindow, WM_CLOSE, 0, 0);
        }
        return 0;
    }

    return DefWindowProc(hWindow, message, wParam, lParam);
}
