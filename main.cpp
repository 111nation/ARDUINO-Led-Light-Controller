#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Lib/PaintLib/paint.h"


struct Form {
        const int TITLE_BAR;
        const int WIDTH;
        const int HEIGHT;
        const int WINDOW_BUTTONS;
        const char* CLASS;
        bool dragging = false;
};

const int TRANSPARENT_COLOR = RGB(24, 25, 30);

struct Form frmMainData = {25, 500, 300, 60, "MAIN", false};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hApplication, HINSTANCE hPrev, PSTR cmd_arg, int window_mode) {

        // Registering Form Blue prints
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hApplication;
        wc.lpszClassName = frmMainData.CLASS;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClass(&wc);
        // Creating and Showing Instance of the blue print
        HWND frmMain = CreateWindowEx(WS_EX_LAYERED, frmMainData.CLASS, "LED Manager",
                        WS_POPUP,
                        
                        (1920 - frmMainData.WIDTH)/2, (1080 - frmMainData.HEIGHT - frmMainData.TITLE_BAR)/2,
                        frmMainData.WIDTH, frmMainData.HEIGHT + frmMainData.TITLE_BAR,
                        NULL,
                        NULL,
                        hApplication,
                        NULL
        );
        // Error handling
        if (frmMain == NULL) {
                MessageBox(NULL, "Error launching application", "Error", MB_ICONERROR);
                return 0;
        }
       
        ShowWindow(frmMain, window_mode);
        // Transparency alpha color
        SetLayeredWindowAttributes(frmMain, TRANSPARENT_COLOR, 0, LWA_COLORKEY);

        // Event/ App LOOP
        MSG event = {};
        while (GetMessage(&event, NULL, 0, 0) > 0) {
                TranslateMessage(&event);
                DispatchMessage(&event);
        }

        return 0;
}

POINT mouse;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
        switch (event) {
                case WM_LBUTTONDOWN: {
                        GetCursorPos(&mouse);
                        ScreenToClient(hWnd, &mouse); // Convert mouse position to pos on client area

                        if (!(mouse.x <= frmMainData.WIDTH && mouse.x >= frmMainData.WINDOW_BUTTONS)) break;
                        if (!(mouse.y >= 0 && mouse.y <= frmMainData.TITLE_BAR)) break; 

                        GetCursorPos(&mouse); // Convert back to screen coordinates
                        SetCapture(hWnd);
                        frmMainData.dragging = true;

                        break;
                }

                case WM_MOUSEMOVE: {
                        // Dragging window
                        if (!frmMainData.dragging) break;

                        POINT offset;
                        GetCursorPos(&offset);

                        RECT window;
                        GetWindowRect(hWnd, &window);
                        window.left += offset.x - mouse.x;
                        window.top += offset.y - mouse.y;


                        MoveWindow(hWnd, window.left, window.top,
                                        frmMainData.WIDTH,
                                        frmMainData.HEIGHT + frmMainData.TITLE_BAR,
                                        false);

                        mouse = offset;

                        break;
                }

                case WM_LBUTTONUP: {
                        if (!frmMainData.dragging) break;

                        frmMainData.dragging = false;
                        ReleaseCapture();

                        break;
                }


                case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;
                
                case WM_PAINT: {
                        const int PADDING = 5;
                        const int MID_X = frmMainData.WIDTH / 2;
                        const int MID_Y = (frmMainData.HEIGHT / 2) + frmMainData.TITLE_BAR;
                        const int MID_TITLE = frmMainData.TITLE_BAR / 2;

                        // Initialize painting
                        Paint paint;
                        
                        // Transparent layer
                        PAINTSTRUCT ps;
                        paint.area = BeginPaint(hWnd, &ps);
                        paint.RECTtoPos(ps.rcPaint);
                        paint.color = TRANSPARENT_COLOR;
                        paint.xend += 1;
                        paint.yend += 1;
                        paint.border.width = 0;
                        paint.Rectangle();
                        
                        // Background
                        paint.color = RGB(24, 25, 38);
                        paint.border.color = RGB(198, 160, 246);
                        paint.border.width = PADDING;
                        paint.x += PADDING;
                        paint.y += PADDING;
                        paint.xend -= PADDING;
                        paint.yend -= PADDING;
                        paint.RoundRect(50);

                        // Title text
                        paint.font.name = "Montserrat";
                        paint.color = RGB(165, 173, 203);
                        paint.font.italics = TRUE; 
                        paint.font.weight = 700;
                        paint.font.size = 20;
                        paint.x = MID_X - 80;
                        paint.y = MID_TITLE + 2;
                        paint.Text("LED Light Controller");

                        // Window buttons
                        const int BUTTON_RADIUS = (frmMainData.TITLE_BAR - 6) / 2;
                        paint.border.width = 0;
                        paint.color = RGB(237, 135, 150);
                        paint.x = 20;
                        paint.y = MID_TITLE + BUTTON_RADIUS/2;
                        paint.xend = paint.x + BUTTON_RADIUS*2;
                        paint.yend = paint.y + BUTTON_RADIUS*2;
                        paint.Circle();        

                        paint.x += 20;
                        paint.xend += 20;
                        paint.color = RGB(238, 212, 159);
                        paint.Circle();

                        EndPaint(hWnd, &ps);                        
                        break;
                }

        }
        

        return DefWindowProc(hWnd, event, wParam, lParam);
}




