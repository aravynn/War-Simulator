#pragma once

#define MAX_LOADSTRING 100
#include "framework.h"
#include "Zombie Simulator.h"



class window {
private:
    WCHAR m_szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR m_szWindowClass[MAX_LOADSTRING];            // the main window class name

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE, int);

   

public:
    // Global Variables:
    HWND hWnd;
    HACCEL hAccelTable;

    window(HINSTANCE hInstance, int nCmdShow);

    
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    // references to dialogs etc. 
    static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

    void Msg(MSG* msg);
};
