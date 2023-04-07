﻿#include <windows.h>
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
    debug << "hui" << std::endl;
    tex.open("pic.png", GL_RGBA);
    debug << "hui" << std::endl;
    sprg.setUniform("tex", tex);

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
    TRARR trarr1;
    trarr1.push(vec2f(0, 0.9), vec3f(abs(sin(getCurTime() * 0.143)) + 0.1, 0, 0), vec2f(0, 0));
    trarr1.push(vec2f(0.9, -0.9), vec3f(0, abs(sin(getCurTime() * 0.343)) + 0.1, 0), vec2f(0, 1));
    trarr1.push(vec2f(-0.9, -0.9), vec3f(0, 0, abs(sin(getCurTime() * 0.413)) + 0.1), vec2f(1, 0));
    trarr1.create();

    glClear(GL_COLOR_BUFFER_BIT);
    trarr1.draw(sprg, translate(sin(getCurTime()), cos(getCurTime())) * rotate(getCurTime()));
    SwapBuffers(GetDC(hWindow));
}


LRESULT CALLBACK MainWinProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        destructContext(hWindow, context);
        PostQuitMessage(0);
        windowOpen = false;
        return 0;
    }
    if (message == WM_KEYDOWN) {
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
        yaSosuPenis(hWindow);
        return 0;
    }
    if (message == WM_SIZE) {
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        return 0;
    }

    return DefWindowProc(hWindow, message, wParam, lParam);
}
