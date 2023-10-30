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
#include "DRAWABLE.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "RIGIDBODY.hpp"

#include "COLLIDER.hpp"
#include "DRAWHUI.h"

LRESULT CALLBACK MainWinProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);


static const PIXELFORMATDESCRIPTOR pfd =
{
    /* nSize          */ sizeof(PIXELFORMATDESCRIPTOR),
    /* nVersion       */ 3,
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

        INT64 tmp = (INT64) (PROC) wglGetProcAddress("wglCreateContextAttribsARB");
        gl = reinterpret_cast<PFNWGLCREATECONTEXTATRIBSARBPROC> (tmp)
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
RigidBody penis;



HGLRC context;

GLTXTR tex;
GLTXTR tex2;
GLTXTR tex3;
GLRTXTR rtex;
Camera camera;
ShadowDrawer shadowDrawer;

vec2f mousePos;

std::vector <RigidBody*> bodies;

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
        penis.rotate(timeDiff);
    }
    if (GetAsyncKeyState('E')) {
        camera.rotate(-timeDiff);
        penis.rotate(-timeDiff);
    }
    if (GetAsyncKeyState('Z')) {
        camera.scale(exp(timeDiff));
    }
    if (GetAsyncKeyState('X')) {
        camera.scale(exp(-timeDiff));
    }

    if (GetAsyncKeyState(VK_LBUTTON)) {
        bodies.push_back(
            new RigidBody(
                0.005, 1,
                new PolygonCollider(getCircleModel(0.01, 4)),
                new Drawable(makeCircle(GLBLUE, 0.01f, 4))
            )
        );
        bodies.back()->setPos(penis.getMat() * vec2f(0.15, 0));
        bodies.back()->velocity = vec2f(penis.getMat() * vec3f(1, 0, 0));
    }



    vec2f pos0 = player.getPos();
    vec2f mw = { 0, 0 };
    mw = { 0, 0 };
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
    //mw = mw * timeDiff;
    penis.velocity += vec2f(penis.getMat() * vec3f(normalize(mw) * timeDiff, 0));
    
    camera.setPos(penis.getPos());
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

    penis.setMass(0.1);
    penis.restitution = 1;
    penis.setDrawModel(new Drawable(makeCircle(GLRED, 0.1f, 3)));
    penis.setCollider(new PolygonCollider(getCircleModel(0.1, 3)));

    bodies.push_back(&penis);

    for (int i = 0; i < 20; ++i) {
        int cnt = 3 + rnd() % 7;
        float r = 0.01 + rnd01() * 0.02;
        Polygonf eldak;
        for (int j = 0; j < cnt; ++j) {
            float angle = rnd01() * 2 * PI;
            eldak.push(r * cos(angle), r * sin(angle));
        }
        eldak.normalizeOrder();
        bodies.push_back(
            new RigidBody(
                r * r, 0.6,
                new PolygonCollider(eldak),
                new Drawable(eldak, GLBLUE)
            )
        );
        bodies.back()->setPos(rnd01() - 0.5, rnd01() - 0.5);
    }

    Polygonf eldak = 
    { { -1, -1 },
    { 1, -1 },
    { 1, -0.9 },
    { -1, -0.9 } };
    bodies.push_back(
        new RigidBody(
            0, 5,
            new PolygonCollider(eldak),
            new Drawable(eldak, GLLIGHTGREY)
        )
    );

    eldak = 
    { { -1, 1 },
    { 1, 1 },
    { 1, 0.9 },
    { -1, 0.9 } };
    std::reverse(eldak.begin(), eldak.end());
    bodies.push_back(
        new RigidBody(
            0, 5,
            new PolygonCollider(eldak),
            new Drawable(eldak, GLLIGHTGREY)
        )
    );

    eldak =
    { {-1, -0.9},
    {-0.9, -0.9},
    {-0.9, 0.9},
    {-1, 0.9} };
    bodies.push_back(
        new RigidBody(
            0, 5,
            new PolygonCollider(eldak),
            new Drawable(eldak, GLLIGHTGREY)
        )
    );

    eldak =
    { {1, -0.9},
    {0.9, -0.9},
    {0.9, 0.9},
    {1, 0.9} };
    std::reverse(eldak.begin(), eldak.end());
    bodies.push_back(
        new RigidBody(
            0, 5,
            new PolygonCollider(eldak),
            new Drawable(eldak, GLLIGHTGREY)
        )
    );

    camera.setScale(1);

    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);

    setTime(prtime);

    MSG msg;
    while (windowOpen) {
        timeDiff = getTimeDiff(prtime);
        if (timeDiff >= 1 / MAX_FPS) {
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
    drawhui.clear();

    for (int iter = 0; iter < 4; ++iter) {
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                resolveCollision(*bodies[i], *bodies[j]);
            }
        }
    }
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        //debug << len(bodies[i]->velocity) << std::endl;
        bodies[i]->update(timeDiff);
    }

    shadowDrawer.clear();
    for (const auto& obs : bodies) {
        if (obs != &penis) {
            shadowDrawer.push(penis.getPos(), obs);
        }
    }

    gravity *= exp(timeDiff * 0.01);
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

    for (auto i : bodies) {
        i->draw(camera);
    }
    //penis.draw(camera);
    player.draw(camera);
    for (auto i : drawhui) {
        i.draw(camera);
    }

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
        for (auto& x : bodies) {
            delete x;
        }
        bodies.clear();
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
