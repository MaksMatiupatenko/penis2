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
#include "SHADOWDRAWER.hpp"
#include "POLYGONDRAWABLE.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "COLLIDER.hpp"

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

std::vector <Drawable*> drawables;
std::vector <PolygonObstacle> obstacles;

PLAYER player;

HGLRC context;

GLTXTR tex;
GLTXTR tex2;
GLTXTR tex3;
GLRTXTR rtex;
Camera camera;
ShadowDrawer shadowDrawer;

vec2f mousePos;

int wWidth, wHeight;

TIME_T prtime;
float timeDiff;

void yaSosuPenis(HWND hwindow);

void updateFrame();

void processAsyncInput() {
    /*if (GetAsyncKeyState('W')) {
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
    }*/
    vec2f pos0 = player.getPos();
    vec2f mw = { 0, 0 };
    if (GetAsyncKeyState(VK_UP)) {
        mw.y += 1;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        mw.y -= 1;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        mw.x -= 1;
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        mw.x += 1;
    }
    mw = normalize(mw) * timeDiff;
    player.move(mw.x, mw.y);
    bool flag = false;
    for (const auto& obstacle : obstacles) {
        if (player.collide(obstacle)) {
            flag = true;
            break;
        }
    }
    
    if (flag) {
        float l = 0, r = 1;
        for (int iter = 0; iter < 30; ++iter) {
            float m = (l + r) / 2;
            player.setPos(pos0);
            player.move(mw * m);

            flag = false;
            for (const auto& obstacle : obstacles) {
                if (player.collide(obstacle)) {
                    flag = true;
                    break;
                }
            }

            if (flag) r = m;
            else l = m;
        }
        
        player.setPos(pos0);
        player.move(mw * r);

        PolygonObstacle coll({}, {});
        bool ff = false;
        for (const auto& obstacle : obstacles) {
            if (player.collide(obstacle)) {
                coll = obstacle;
                ff = true;
                break;
            }
        }
        
        player.setPos(pos0);
        player.move(mw * l);
        pos0 = player.getPos();

        if (false && ff) {
            vec2f nm = player.getCollNormal(coll);

            mw = mw * (1 - l);
            mw = mw - nm * dt(nm, mw);

            l = 1; r = 1;
            for (int iter = 0; iter < 30; ++iter) {
                float m = (l + r) / 2;
                player.setPos(pos0);
                player.move(mw * m);

                flag = false;
                for (const auto& obstacle : obstacles) {
                    if (player.collide(obstacle)) {
                        flag = true;
                        break;
                    }
                }

                if (flag) r = m;
                else l = m;
            }

            player.setPos(pos0);
            player.move(mw * l);
        }
        
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tex.open("pic.png", GL_RGBA);
    tex2.open("pic2.png", GL_RGBA);
    tex3.create(GL_RGBA, wWidth, wHeight, GL_RGBA, NULL);
    rtex.create();
    rtex.createTexture(GL_RGBA, wWidth, wHeight);
    camera.setViewArea((float)wWidth / wHeight, 1);
    shadowDrawer.create(wWidth, wHeight);
    shadowDrawer.setShadowColor(vec4f(0, 0, 0, 0.5));

    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);

    setTime(prtime);

    MSG msg;
    while (windowOpen) {
        timeDiff = getTimeDiff(prtime);
        if (timeDiff >= 1/ MAX_FPS) {
            setTime(prtime);
            updateFrame();
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

void updateFrame() {
    obstacles.clear();
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
    const FLOAT MAXABS = 1.f * wWidth / wHeight;
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

    for (auto i : drawables) {
        delete i;
    }
    drawables.clear();
    for (auto obstacle : obstacles) {
        drawables.push_back(new PolygonObstacle(obstacle));
    }

    shadowDrawer.clear();
    for (const auto& obs : drawables) {
        shadowDrawer.push(player.getPos(), (Drawable*) obs);
    }

}

void yaSosuPenis(HWND hWindow) {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    Drawable(Polygonf{
        {-0.5, 0.0},
        {-0.4, 0.0},
        {-0.4, 0.1},
        {-0.5, 0.1}
                    }, GLLIGHTBLUE).draw(camera);
    shadowDrawer.draw(camera);
    for (auto ptr : drawables) {
        ptr->draw(camera);
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
        shadowDrawer.destruct();
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
