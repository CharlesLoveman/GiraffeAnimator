// GiraffeAnimator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GiraffeAnimator.h"
#include "Giraffe.h"
#include <Windows.h>
#include <timeapi.h>
#include <CommCtrl.h>
#include <ShObjIdl.h>
#include <ShObjIdl_core.h>
#include <atlbase.h>
#include <string>
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
Giraffe myGiraffe;                              // bet you cant figure this one out
BYTE flags;
RECT clientRect;
bool selected;
int next = 0;
Vec2 Force;
float Damage;
float Knockback;
float Scale;
bool Fixed;

HWND skelButton;
HWND cpButton;
HWND lineButton;
HWND fwdFrameBttn;
HWND bckFrameBttn;
HWND rsrtFrameBttn;
HWND prevButton;
HWND hurtButton;
HWND hitButton;
HWND frameBar;
HWND saveButton;
HWND loadButton;
HWND srlzButton;
HWND instButton;
HWND dltButton;
HWND mrgButton;
//HWND flxButton;
//HWND flyButton;


//struct COMDLG_FILTERSPEC
//{
//    LPCWSTR pszName;
//    LPCWSTR pszSpec;
//};


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_GIRAFFEANIMATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GIRAFFEANIMATOR));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GIRAFFEANIMATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GIRAFFEANIMATOR);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   if (!GetClientRect(hWnd, &clientRect)) {
       return FALSE;
   }

   INITCOMMONCONTROLSEX initcntrls;
   initcntrls.dwICC = ICC_BAR_CLASSES;
   initcntrls.dwSize = sizeof(initcntrls);
   InitCommonControlsEx(&initcntrls);

   skelButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Skeleton",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       10,         // y position 
       100,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU) IDD_SKELBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   cpButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Control Points",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       50,         // y position 
       150,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_CPBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   lineButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Outline",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       90,         // y position 
       150,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_LINEBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   prevButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Display Previous",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       210,         // y position 
       150,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_PREVBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   hurtButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Hurtboxes",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       130,         // y position 
       150,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_HURTBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   hitButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Hitboxes",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
       10,         // x position 
       170,         // y position 
       150,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_HITBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.



   fwdFrameBttn = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L">",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       240,         // x position 
       10,         // y position 
       20,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_FWDBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   bckFrameBttn = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"<",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       220,         // x position 
       10,         // y position 
       20,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_BCKBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   rsrtFrameBttn = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"<<",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       200,         // x position 
       10,         // y position 
       20,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_RSRTBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   saveButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Save",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       700,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_SAVEBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   loadButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Load",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       600,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_LOADBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   srlzButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Export",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       750,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_SRLZBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   mrgButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Merge",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       650,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_MRGBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   instButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Insert",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       850,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_INSTBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   dltButton = CreateWindowEx(
       0,
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Delete",      // Button text 
       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
       900,         // x position 
       10,         // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)IDD_DLTBUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   //flxButton = CreateWindowEx(
   //    0,
   //    L"BUTTON",  // Predefined class; Unicode assumed 
   //    L"Flip X",      // Button text 
   //    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
   //    1000,         // x position 
   //    10,         // y position 
   //    50,        // Button width
   //    20,        // Button height
   //    hWnd,     // Parent window
   //    (HMENU)IDD_FLXBUTTON,       // No menu.
   //    (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
   //    NULL);      // Pointer not needed.

   //flyButton = CreateWindowEx(
   //    0,
   //    L"BUTTON",  // Predefined class; Unicode assumed 
   //    L"Flip Y",      // Button text 
   //    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
   //    1050,         // x position 
   //    10,         // y position 
   //    50,        // Button width
   //    20,        // Button height
   //    hWnd,     // Parent window
   //    (HMENU)IDD_FLYBUTTON,       // No menu.
   //    (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
   //    NULL);      // Pointer not needed.

   frameBar = CreateWindowEx(
       0,
       TRACKBAR_CLASS,  // Predefined class; Unicode assumed 
       L"Frame",      // Label?
       WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS | TBS_ENABLESELRANGE,  // Styles 
       300,         // x position 
       10,         // y position 
       200,        // width
       30,        // height
       hWnd,     // Parent window
       (HMENU)IDD_FRAMEBAR,
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.
   SendMessage(frameBar, TBM_SETRANGE,
       (WPARAM)TRUE,                   // redraw flag 
       (LPARAM)MAKELONG(0, 0));  // min. & max. positions

   SendMessage(frameBar, TBM_SETPAGESIZE,
       0, (LPARAM)4);                  // new page size 

   //SendMessage(frameBar, TBM_SETSEL,
   //    (WPARAM)FALSE,                  // redraw flag 
   //    (LPARAM)MAKELONG(0, 0));

   SendMessage(frameBar, TBM_SETPOS,
       (WPARAM)TRUE,                   // redraw flag 
       (LPARAM)0);

   SetFocus(frameBar);


   myGiraffe = Giraffe(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

   selected = false;
   next = timeGetTime();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



char InputHolder[10];
BOOL CALLBACK HurtProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SetDlgItemTextA(hwnd, IDC_EDIT1, std::to_string(Damage).c_str());
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            if (GetDlgItemTextA(hwnd, IDC_EDIT1, InputHolder, 10)) {
                Damage = std::stof(InputHolder);
            }
        case IDCANCEL:
            EndDialog(hwnd, wParam);
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CALLBACK HitProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SetDlgItemTextA(hwnd, IDC_EDIT1, std::to_string(Force.x).c_str());
        SetDlgItemTextA(hwnd, IDC_EDIT2, std::to_string(Force.y).c_str());
        SetDlgItemTextA(hwnd, IDC_EDIT3, std::to_string(Damage).c_str());
        SetDlgItemTextA(hwnd, IDC_EDIT4, std::to_string(Knockback).c_str());
        SetDlgItemTextA(hwnd, IDC_EDIT5, std::to_string(Scale).c_str());
        CheckDlgButton(hwnd, IDC_CHECK1, Fixed);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            if (GetDlgItemTextA(hwnd, IDC_EDIT1, InputHolder, 10)) {
                Force.x = std::stof(InputHolder);
            }
            if (GetDlgItemTextA(hwnd, IDC_EDIT2, InputHolder, 10)) {
                Force.y = std::stof(InputHolder);
            }
            Force = Force.Normalise();
            if (GetDlgItemTextA(hwnd, IDC_EDIT3, InputHolder, 10)) {
                Damage = std::stof(InputHolder);
            }
            if (GetDlgItemTextA(hwnd, IDC_EDIT4, InputHolder, 10)) {
                Knockback = std::stof(InputHolder);
            }
            if (GetDlgItemTextA(hwnd, IDC_EDIT5, InputHolder, 10)) {
                Scale = std::stof(InputHolder);
            }
            Fixed = IsDlgButtonChecked(hwnd, IDC_CHECK1);
        case IDCANCEL:
            EndDialog(hwnd, wParam);
            return TRUE;
        }
    }
    return FALSE;
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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDD_SKELBUTTON:
                flags ^= DRAW_SKELETON;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_CPBUTTON:
                flags ^= DRAW_CP;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_LINEBUTTON:
                flags ^= DRAW_OUTLINE;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_PREVBUTTON:
                flags ^= DRAW_PREV;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_HURTBUTTON:
                flags ^= DRAW_HURT;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_HITBUTTON:
                flags ^= DRAW_HIT;
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_FWDBUTTON:
                if (myGiraffe.NextFrame(hWnd, clientRect)) {
                    SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
                }
                SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_BCKBUTTON:
                if (myGiraffe.PrevFrame(hWnd, clientRect)) {
                    SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
                    InvalidateRect(hWnd, &clientRect, true);
                }
                break;
            case IDD_RSRTBUTTON:
                myGiraffe.SetFrame(0);
                SendMessageW(frameBar, TBM_SETPOS, true, 0);
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_INSTBUTTON:
                myGiraffe.InsertFrame(hWnd, clientRect);
                SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
                SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_DLTBUTTON:
                myGiraffe.DeleteFrame();
                SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
                SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
                InvalidateRect(hWnd, &clientRect, true);
                break;
            /*case IDD_FLXBUTTON:
                myGiraffe.FlipX();
                InvalidateRect(hWnd, &clientRect, true);
                break;
            case IDD_FLYBUTTON:
                myGiraffe.FlipY();
                InvalidateRect(hWnd, &clientRect, true);
                break;*/
            case IDD_SAVEBUTTON:
            {
                HRESULT hr;
                CComPtr<IFileSaveDialog> pDlg;
                COMDLG_FILTERSPEC aFileTypes[] = {
                    { L"Text files", L"*.txt" },
                    { L"All files", L"*.*" }
                };

                // Create the file-open dialog COM object.
                hr = pDlg.CoCreateInstance(__uuidof(FileSaveDialog));

                if (FAILED(hr))
                    return DefWindowProc(hWnd, message, wParam, lParam);

                // Set the dialog's caption text and the available file types.
                // NOTE: Error handling omitted here for clarity.
                pDlg->SetFileTypes(_countof(aFileTypes), aFileTypes);
                pDlg->SetTitle(L"Save Move");

                hr = pDlg->Show(hWnd);
                if (SUCCEEDED(hr))
                {
                    CComPtr<IShellItem> pItem;

                    hr = pDlg->GetResult(&pItem);

                    if (SUCCEEDED(hr))
                    {
                        LPOLESTR pwsz = NULL;

                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                        if (SUCCEEDED(hr))
                        {
                            USES_CONVERSION;
                            myGiraffe.Save(OLE2CA(pwsz));
                            CoTaskMemFree(pwsz);
                        }
                    }
                }
                break;
            }
            case IDD_LOADBUTTON:
            {
                HRESULT hr;
                CComPtr<IFileOpenDialog> pDlg;
                COMDLG_FILTERSPEC aFileTypes[] = {
                    { L"Text files", L"*.txt" },
                    { L"All files", L"*.*" }
                };

                // Create the file-open dialog COM object.
                hr = pDlg.CoCreateInstance(__uuidof(FileOpenDialog));

                if (FAILED(hr))
                    return DefWindowProc(hWnd, message, wParam, lParam);

                // Set the dialog's caption text and the available file types.
                // NOTE: Error handling omitted here for clarity.
                pDlg->SetFileTypes(_countof(aFileTypes), aFileTypes);
                pDlg->SetTitle(L"Load Move");

                hr = pDlg->Show(hWnd);
                if (SUCCEEDED(hr))
                {
                    CComPtr<IShellItem> pItem;

                    hr = pDlg->GetResult(&pItem);

                    if (SUCCEEDED(hr))
                    {
                        LPOLESTR pwsz = NULL;

                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                        if (SUCCEEDED(hr))
                        {
                            USES_CONVERSION;
                            myGiraffe.Load(OLE2CA(pwsz));
                            CoTaskMemFree(pwsz);
                            SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
                            SendMessageW(frameBar, TBM_SETPOS, true, 0);
                            selected = false;
                            InvalidateRect(hWnd, &clientRect, true);
                        }
                    }
                }
                break;
            }
            case IDD_SRLZBUTTON:
            {
                HRESULT hr;
                CComPtr<IFileSaveDialog> pDlg;
                COMDLG_FILTERSPEC aFileTypes[] = {
                    { L"Text files", L"*.txt" },
                    { L"All files", L"*.*" }
                };

                // Create the file-open dialog COM object.
                hr = pDlg.CoCreateInstance(__uuidof(FileSaveDialog));

                if (FAILED(hr))
                    return DefWindowProc(hWnd, message, wParam, lParam);

                // Set the dialog's caption text and the available file types.
                // NOTE: Error handling omitted here for clarity.
                pDlg->SetFileTypes(_countof(aFileTypes), aFileTypes);
                pDlg->SetTitle(L"Export Move");

                hr = pDlg->Show(hWnd);
                if (SUCCEEDED(hr))
                {
                    CComPtr<IShellItem> pItem;

                    hr = pDlg->GetResult(&pItem);

                    if (SUCCEEDED(hr))
                    {
                        LPOLESTR pwsz = NULL;

                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                        if (SUCCEEDED(hr))
                        {
                            USES_CONVERSION;
                            myGiraffe.Serialize(OLE2CA(pwsz));
                            CoTaskMemFree(pwsz);
                        }
                    }
                }
                break;
            }
            case IDD_MRGBUTTON:
            {
                HRESULT hr;
                CComPtr<IFileOpenDialog> pDlg;
                COMDLG_FILTERSPEC aFileTypes[] = {
                    { L"Text files", L"*.txt" },
                    { L"All files", L"*.*" }
                };

                // Create the file-open dialog COM object.
                hr = pDlg.CoCreateInstance(__uuidof(FileOpenDialog));

                if (FAILED(hr))
                    return DefWindowProc(hWnd, message, wParam, lParam);

                // Set the dialog's caption text and the available file types.
                // NOTE: Error handling omitted here for clarity.
                pDlg->SetFileTypes(_countof(aFileTypes), aFileTypes);
                pDlg->SetTitle(L"Load Move To Merge");

                hr = pDlg->Show(hWnd);
                if (SUCCEEDED(hr))
                {
                    CComPtr<IShellItem> pItem;

                    hr = pDlg->GetResult(&pItem);

                    if (SUCCEEDED(hr))
                    {
                        LPOLESTR pwsz = NULL;

                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                        if (SUCCEEDED(hr))
                        {
                            USES_CONVERSION;
                            myGiraffe.Merge(OLE2CA(pwsz));
                            CoTaskMemFree(pwsz);
                            SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
                            selected = false;
                            InvalidateRect(hWnd, &clientRect, true);
                        }
                    }
                }
                break;
            }
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
    case WM_LBUTTONDBLCLK:
        if (flags & DRAW_HIT) {
            myGiraffe.AddRemoveHitbox(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &clientRect, true);
        }
    case WM_LBUTTONDOWN:
         selected = myGiraffe.Select(LOWORD(lParam), HIWORD(lParam), flags, false);
        break;
    case WM_LBUTTONUP:
        myGiraffe.Deselect();
        selected = false;
        break;
    case WM_RBUTTONDBLCLK: 
    {
        int index;
        switch (myGiraffe.CheckIntersect(LOWORD(lParam), HIWORD(lParam), flags, index))
        {
        case 1:
            myGiraffe.GetHitboxData(Damage, Force, Knockback, Scale, Fixed, index);
            if (DialogBox(hInst, MAKEINTRESOURCE(IDD_HITDLG), hWnd, (DLGPROC)HitProc) == IDOK) {
                myGiraffe.SetHitboxData(Damage, Force, Knockback, Scale, Fixed, index);
            }
            break;
        case 2:
            Damage = myGiraffe.GetHurtboxData(index);
            if (DialogBox(hInst, MAKEINTRESOURCE(IDD_HURTDLG), hWnd, (DLGPROC)HurtProc) == IDOK) {
                myGiraffe.SetHurtboxData(Damage, index);
            }
            break;
        default:
            break;
        }
        break;
    }
    case WM_RBUTTONDOWN:
        selected = myGiraffe.Select(LOWORD(lParam), HIWORD(lParam), flags, true);
        break;
    case WM_RBUTTONUP:
        myGiraffe.Deselect();
        selected = false;
        break;
    case WM_MOUSEMOVE:
        if (selected && timeGetTime() > next) {
            myGiraffe.Update(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &clientRect, true);
            next += 50;
        }
        break;
    case WM_HSCROLL:
    {
        int cmd = LOWORD(wParam);
        switch (cmd) {
        case TB_THUMBTRACK:
            if (!myGiraffe.SetFrame(HIWORD(wParam))) {
                SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
            }
            InvalidateRect(hWnd, &clientRect, true);
            break;
        case TB_THUMBPOSITION:
            if (!myGiraffe.SetFrame(HIWORD(wParam))) {
                SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
            }
            InvalidateRect(hWnd, &clientRect, true);
            break;
        case TB_LINEDOWN:
            if (myGiraffe.NextFrame(hWnd, clientRect)) {
                SendMessageW(frameBar, TBM_SETRANGEMAX, true, myGiraffe.GetMaxFrames());
            }
            SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
            InvalidateRect(hWnd, &clientRect, true);
            break;
        case TB_LINEUP:
            if (myGiraffe.PrevFrame(hWnd, clientRect)) {
                //SendMessageW(frameBar, TBM_SETPOS, true, myGiraffe.GetFrameNum());
                InvalidateRect(hWnd, &clientRect, true);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            myGiraffe.Draw(hdc, flags, clientRect);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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

