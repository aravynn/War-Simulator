// Zombie Simulator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Zombie Simulator.h"
#include "zombie_map.h"
#include "zmap_cell.h"
#include "Bitmap_Operations.h"

#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// My Global Variables, for processing the project.
Bitmap_Operations* biop;
zombie_map zmap(MAP_WIDTH, MAP_HEIGHT);
int idTimer;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void paintMap(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ZOMBIESIMULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZOMBIESIMULATOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZOMBIESIMULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ZOMBIESIMULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, MAP_WIDTH * MAP_CELL_SIZE + 16, MAP_HEIGHT * MAP_CELL_SIZE + 58, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    //HWND staticHwnd = CreateWindowW(L"STATIC", L"Data Goes Here", WS_CHILD | WS_VISIBLE | SS_LEFT, 10, MAP_HEIGHT * MAP_CELL_SIZE + 58, 500, 20, hWnd, (HMENU)0, NULL, NULL);

    //SetFocus(hWnd);

    switch (message)
    {
    case WM_COMMAND:
        {
        
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        zmap.day();
        paintMap(hWnd);
        UpdateWindow(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    
    case WM_ERASEBKGND: // THIS WORRKS TO HELP REDUCE FLICKERING, HOWEVER THIS DOES NOT BUFFER THE FULL DRAW, which we want.
        return 1;
    case WM_CREATE:
        // im pretty sure this is on window create.
        // start the time here.
        biop = new Bitmap_Operations();
        biop->Initialize_Buffers(hWnd, 1);
        biop->Create_Buffer(0);//(B)

        SetTimer(hWnd, idTimer = 1, 100, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        KillTimer(hWnd, 1);
        //Release Memory of Buffer
        biop->Free_Buffer(0);
        delete biop;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void paintMap(HWND hWnd) {
    // TODO: Add any drawing code that uses hdc here...

    std::vector<zmap_cell> map = zmap.getMap();

    int w = zmap.getWidth();
    int h = zmap.getHeight();

    //int cellSize = 5; // size for each spot on the map, can be changed probably through code at a later time. 

    // run through all rooms, and paint each "pixel" 

    for (int x{ 0 }; x < w; ++x) {
        for (int y{ 0 }; y < h; ++y) {
            // set the pen for that pixel

            //RGB thisColor((char)map.at(x * w + y).GetZombie(), (char)map.at(x * w + y).GetZombie(), (char)map.at(x * w + y).GetZombie());

            std::vector<unsigned char> colors = map.at(x * w + y).getColorCast();

            HPEN pen = CreatePen(PS_SOLID, 1, RGB(colors.at(0), colors.at(1), colors.at(2)));
            HBRUSH brush = CreateSolidBrush(RGB(colors.at(0), colors.at(1), colors.at(2)));
            SelectObject(biop->Get_DC_Buffer(0), pen);
            SelectObject(biop->Get_DC_Buffer(0), brush);

            // draw a rect displaying the spot
            Rectangle(biop->Get_DC_Buffer(0), x * MAP_CELL_SIZE, y * MAP_CELL_SIZE, (x + 1) * MAP_CELL_SIZE, (y + 1) * MAP_CELL_SIZE);



            // delete the pen and brush resources.
            DeleteObject(pen);
            DeleteObject(brush);

        }
    }
    biop->Copy_to_Screen(0);

}

/*
void paintMap(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code that uses hdc here...

    std::vector<zmap_cell> map = zmap.getMap();

    int w = zmap.getWidth();
    int h = zmap.getHeight();

    //int cellSize = 5; // size for each spot on the map, can be changed probably through code at a later time. 

    // run through all rooms, and paint each "pixel" 

    for (int x{ 0 }; x < w; ++x) {
        for (int y{ 0 }; y < h; ++y) {
            // set the pen for that pixel

            //RGB thisColor((char)map.at(x * w + y).GetZombie(), (char)map.at(x * w + y).GetZombie(), (char)map.at(x * w + y).GetZombie());

            std::vector<unsigned char> colors = map.at(x * w + y).getColorCast();

            HPEN pen = CreatePen(PS_SOLID, 1, RGB(colors.at(0), colors.at(1), colors.at(2)));
            HBRUSH brush = CreateSolidBrush(RGB(colors.at(0), colors.at(1), colors.at(2)));
            SelectObject(hdc, pen);
            SelectObject(hdc, brush);

            // draw a rect displaying the spot
            Rectangle(hdc, x * MAP_CELL_SIZE, y * MAP_CELL_SIZE, (x + 1) * MAP_CELL_SIZE, (y + 1) * MAP_CELL_SIZE);

            // delete the pen and brush resources.
            DeleteObject(pen);
            DeleteObject(brush);

        }
    }


    EndPaint(hWnd, &ps);
}*/