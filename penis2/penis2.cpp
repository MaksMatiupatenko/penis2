#include <windows.h>
#include <fstream>
#include <bitset>
#include <cmath>

std::ofstream debug("debug.txt");

#include "GLLOADER.hpp"
#define WGL_WGLEXT_PROTOTYPES
#include "glext.h"
#include "MATH.h"
#include "TRIANGLEARRAY.hpp"
#include "SHADER.hpp"
#include "TIME.hpp"
#include "RENDERTEXTURE.hpp"
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


HGLRC context;

SPRG sprg;
GLTXTR tex;
GLTXTR tex2;
GLTXTR tex3;
GLRTXTR rtex;

TIME_T prtime;

void yaSosuPenis(HWND hwindow);

bool windowOpen = true;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE prevInstance,
                   LPSTR lpCommandLine,
                   int nCommandShow) {
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

    HWND hWindow = CreateWindow(L"windowClass",
                                L"windowTitle",
                                WS_OVERLAPPEDWINDOW,
                                0,
                                0,
                                800,
                                600,
                                NULL,
                                NULL,
                                hInstance,
                                NULL);
    if (!hWindow) return FALSE;

    context = createContext(hWindow);

    initOpenGl();
    
    sprg.loadFromFile("shader.vert", "", "", "", "shader.frag");
    tex.open("pic.png", GL_RGBA);
    tex2.open("pic2.png", GL_RGBA);
    tex3.create(GL_RGBA, 800, 600, GL_RGBA, NULL);
    rtex.create();
    rtex.createTexture(GL_RGBA, 800, 600);

    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);

    setTime(prtime);
    sprg.use();

    MSG msg;
    while (windowOpen) {
        yaSosuPenis(hWindow);
        while (PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

void yaSosuPenis(HWND hWindow) {
    TRARR trarr;
    trarr.push(vec2f(0, 0.9), vec3f(0, 0, 0), vec2f(0, 0));
    trarr.push(vec2f(0.9, -0.9), vec3f(0, 0, 0), vec2f(0, 1));
    trarr.push(vec2f(-0.9, -0.9), vec3f(0, 0, 0), vec2f(1, 0));
    trarr.create();

    TRARR trarr2;
    trarr2.push(vec2f(-1, -1), vec3f(0, 0, 0), vec2f(0, 1));
    trarr2.push(vec2f(1, -1), vec3f(0, 0, 0), vec2f(1, 1));
    trarr2.push(vec2f(1, 1), vec3f(0, 0, 0), vec2f(1, 0));
    trarr2.push(vec2f(-1, -1), vec3f(0, 0, 0), vec2f(0, 1));
    trarr2.push(vec2f(-1, 1), vec3f(0, 0, 0), vec2f(0, 0));
    trarr2.push(vec2f(1, 1), vec3f(0, 0, 0), vec2f(1, 0));
    trarr2.create();

    rtex.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    sprg.setUniform("tex", tex);
    trarr.draw(sprg, rotate(getCurTime()));
    //trarr.draw(sprg, mat3f());
    rtex.unbind();

    glClear(GL_COLOR_BUFFER_BIT);
    sprg.setUniform("tex", rtex.getTexture());
    //sprg.setUniform("tex", tex);
    trarr.draw(sprg, mat3f());

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
        //// от этой хуйни все падает
        if (wParam == 'W') {
            MessageBox(hWindow, L"sosi", L"hui", MB_OK);
        }
        if (wParam == 'A') {
            MessageBox(hWindow, L"hui", L"sosi", MB_OK);
        }
        if (wParam == 'S') {
            MessageBox(hWindow, L"ty", L"pidr", MB_OK);
        }
        if (wParam == 'D') {
            MessageBox(hWindow, L"bobr", L"kurwa", MB_OK);
        }
        return 0;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        yaSosuPenis(hWindow);
        BeginPaint(hWindow, &ps);
        EndPaint(hWindow, &ps);
        return 0;
    }
    if (message == WM_SIZE) {
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        rtex.deleteTexture();
        rtex.createTexture(GL_RGBA, LOWORD(lParam), HIWORD(lParam));
        return 0;
    }

    return DefWindowProc(hWindow, message, wParam, lParam);
}
