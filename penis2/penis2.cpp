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
#include "POLYGONDRAWABLE.hpp"
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

std::vector <PolygonDrawable> drawables;

PLAYER player;

HGLRC context;

GLTXTR tex;
GLTXTR tex2;
GLTXTR tex3;
GLRTXTR rtex;
Camera camera;

vec2f mousePos;

int wWidth, wHeight;

TIME_T prtime;
float timeDiff;

void yaSosuPenis(HWND hwindow);

void updateFrame();

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
    if (GetAsyncKeyState(VK_UP)) {
        player.move(0, timeDiff);
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        player.move(0, -timeDiff);
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        player.move(-timeDiff, 0);
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        player.move(timeDiff, 0);
    }
}

void updateConditions();

bool windowOpen = true;
float MAX_FPS = 60;

void boundVector(vec2f& vec) {
    vec.x = min(vec.x, 1);
    vec.y = min(vec.y, 1);
    vec.x = max(vec.x, -1);
    vec.y = max(vec.y, -1);
}

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
        wHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
        hWindow = CreateWindow(L"windowClass",
                               L"windowTitle",
                               WS_POPUP | WS_VISIBLE,
                               mi.rcMonitor.left,
                               mi.rcMonitor.top,
                               wWidth,
                               wHeight,
                               NULL, NULL, hInstance, 0);
    }


    context = createContext(hWindow);
    initOpenGl();
    initShaders();

    glClearColor(1, 1, 1, 1);

    tex.open("pic.png", GL_RGBA);
    tex2.open("pic2.png", GL_RGBA);
    tex3.create(GL_RGBA, wWidth, wHeight, GL_RGBA, NULL);
    rtex.create();
    rtex.createTexture(GL_RGBA, wWidth, wHeight);
    camera.setViewArea((float)wWidth / wHeight, 1);

    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);

    setTime(prtime);

    MSG msg;
    while (windowOpen) {
        timeDiff = getTimeDiff(prtime);
        if (timeDiff >= 1/ MAX_FPS) {
            setTime(prtime);
            processAsyncInput();
            updateFrame();
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

void updateFrame() {
    std::vector <PolygonDrawable> obstacles;
    obstacles.emplace_back(Polygonf{
        {0.1, 0.1},
        {0.3, 0.1},
        {0, 0.5}
    }, GLLIGHTGREY);
    obstacles.emplace_back(Polygonf{
        {-0.1, -0.1},
        {-0.3, -0.1},
        {0, -0.5}
        }, GLLIGHTGREY);
    obstacles.emplace_back(Polygonf{
        {0.5, -0.3},
        {0.8, -0.3},
        {0.7, -0.2},
        {0.8, -0.1},
        {0.5, -0.1}
        }, GLLIGHTGREY);
    const FLOAT MAXABS = wWidth / wHeight;
    const FLOAT WIDTH = 0.03f;
    obstacles.emplace_back(Polygonf{
        {-MAXABS, -1},
        {MAXABS, -1},
        {MAXABS, -1 + WIDTH},
        {-MAXABS, -1 + WIDTH}
        }, GLLIGHTGREY);
    obstacles.emplace_back(Polygonf{
        {-MAXABS, 1},
        {-MAXABS, 1 - WIDTH},
        {MAXABS, 1 - WIDTH},
        {MAXABS, 1}
        }, GLLIGHTGREY);
    obstacles.emplace_back(Polygonf{
        {-MAXABS, -1},
        {-MAXABS + WIDTH, -1},
        {-MAXABS + WIDTH, 1},
        {-MAXABS, 1}
        }, GLLIGHTGREY);
    obstacles.emplace_back(Polygonf{
        {MAXABS - WIDTH, -1},
        {MAXABS, -1},
        {MAXABS, 1},
        {MAXABS - WIDTH, 1}
        }, GLLIGHTGREY);


    drawables.clear();

    for (size_t i = 0; i < obstacles.size(); ++i) {
        auto obstacle = obstacles[i];
        for (size_t i = 0; i < obstacle.box().size(); ++i) {
            Polygonf hitbox;

            vec2f dir1(player.getPos(), obstacle.box().get(i));
            hitbox.push_back(obstacle.box().get(i));
            vec2f t1 = obstacle.box().get(i) + dir1 * 1e9f;
            hitbox.push_back(t1);

            vec2f dir2(player.getPos(), obstacle.box().get(i + 1));
            vec2f t2 = obstacle.box().get(i + 1) + dir2 * 1e9f;
            hitbox.push_back(t2);
            hitbox.push_back(obstacle.box().get(i + 1));

            drawables.push_back(PolygonDrawable(hitbox, GLGREY));
        }
    }
    for (auto obstacle : obstacles) {
        drawables.push_back(obstacle);
    }
}

void yaSosuPenis(HWND hWindow) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto ptr : drawables) {
        ptr.draw(camera);
    }

    player.draw(camera);

    auto hdc = GetDC(hWindow);
    SwapBuffers(hdc);
    ReleaseDC(hWindow, hdc);
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
