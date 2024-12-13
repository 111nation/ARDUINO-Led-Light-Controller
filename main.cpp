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

struct Form frmMainData = {25, 500, 300, 60, "MAIN", false};

struct Button {
        bool close = false;
        bool minimize = false;
};

struct Button windowButton;

const int TRANSPARENT_COLOR = RGB(24, 25, 30);

void WindowButtons(HWND hWnd);

void DrawButtons(Paint &);
void DrawLED(Paint &);

// Drawing constants
const int PADDING = 5;
const int MID_X = frmMainData.WIDTH / 2;
const int MID_Y = (frmMainData.HEIGHT / 2) + frmMainData.TITLE_BAR;
const int MID_TITLE = frmMainData.TITLE_BAR / 2;

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
bool click = false;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
        switch (event) {
                case WM_LBUTTONDOWN: {
                        click = true;     

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
                        WindowButtons(hWnd);

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
                        // Window Buttons
                        if (!click) break;

                        if (windowButton.close) {
                                SendMessage(hWnd, WM_DESTROY, wParam, lParam);

                                windowButton.close = false;
                                windowButton.minimize = false;
                        }

                        if (windowButton.minimize) {
                                ShowWindow(hWnd, SW_MINIMIZE);

                                windowButton.close = false;
                                windowButton.minimize = false;
                        }

                        click = false;

                        // Dragging
                        if (!frmMainData.dragging) break;
                        
                        frmMainData.dragging = false;
                        
                        ReleaseCapture();

                        break;
                }


                case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;
                
                case WM_PAINT: {
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

                        DrawButtons(paint);
                        DrawLED(paint);

                        EndPaint(hWnd, &ps);                        
                        break;
                }

        }
        

        return DefWindowProc(hWnd, event, wParam, lParam);
}

void DrawLED(Paint & paint) {
        paint.Reset();
        
        int color = RGB(73, 77, 100);
        color = RGB(255, 255, 0);

        const int LED_DIAMETER = 100;
        const int START_X = 75;
        const int START_Y = 75;

        // Circle
        paint.x = START_X;
        paint.xend = paint.x + LED_DIAMETER;
        paint.y = START_Y;
        paint.yend = paint.y + LED_DIAMETER;
        paint.color = color; 
        paint.Circle();
        // Remove rounding at top
        paint.y += LED_DIAMETER/2; // Ensure stats at diameter
        paint.yend = paint.y + 10;
        paint.Rectangle();
        // Round
        paint.yend = paint.y + LED_DIAMETER/2;
        paint.RoundRect(10);

        // Draw legs with equal spacing
        const int LEG_WIDTH = 5;
        const int LEG_HEIGHT =  50;
        const int LEG_SPACING = (LED_DIAMETER/4) - LEG_WIDTH;

        paint.x = START_X - LEG_WIDTH/2;
        paint.y = paint.yend-1; // Start at end of the rectangle
        paint.yend = paint.y + LEG_HEIGHT;
        paint.color = RGB(128, 135, 162);
        for (int leg = 0; leg < 4; leg++) {
                paint.x += LEG_SPACING;
                paint.xend = paint.x + LEG_WIDTH;
                paint.Rectangle();                
        }
}

void WindowButtons(HWND hWnd) {
        if (frmMainData.dragging) return;
        
        GetCursorPos(&mouse);
        ScreenToClient(hWnd, &mouse); // Convert mouse position to pos on client area
        
        const int BUTTON = (frmMainData.TITLE_BAR - 6);
        int START_X = 20;
        int START_Y = MID_TITLE + BUTTON/4;
        
        const bool CLOSE_HOVER = (mouse.x >= START_X && mouse.x <= START_X + BUTTON)
                                && (mouse.y >= START_Y && mouse.y <= START_Y + BUTTON);

        // Close Button
        if (CLOSE_HOVER) {
                windowButton.close = true;
        } else {
                windowButton.close = false;
        }

        START_X += START_X;
        
        const bool MINIMIZE_HOVER = (mouse.x >= START_X && mouse.x <= START_X + BUTTON)
                                && (mouse.y >= START_Y && mouse.y <= START_Y + BUTTON);
        
        // Minimize Button
        if (MINIMIZE_HOVER) {
                windowButton.minimize = true;
        } else {
                windowButton.minimize = false;
        }

        // Draws window buttons
        Paint paint;
        paint.area = GetDC(hWnd);
        DrawButtons(paint);
        ReleaseDC(hWnd, paint.area);
}

void DrawButtons(Paint & paint) {
        COLORREF close = RGB(54, 58, 79);
        COLORREF minimize = RGB(54, 58, 79);
        
        if (windowButton.close) {
                close = RGB(237, 135, 150);
        }

        if (windowButton.minimize) {
                minimize = RGB(238, 212, 159);
        }

        // Window buttons
        const int BUTTON_RADIUS = (frmMainData.TITLE_BAR - 6) / 2;
        paint.border.width = 0;
        paint.color = close;
        paint.x = 20;
        paint.y = MID_TITLE + BUTTON_RADIUS/2;
        paint.xend = paint.x + BUTTON_RADIUS*2;
        paint.yend = paint.y + BUTTON_RADIUS*2;
        paint.Circle();        

        paint.x += 20;
        paint.xend += 20;
        paint.color = minimize; 
        paint.Circle();
}
