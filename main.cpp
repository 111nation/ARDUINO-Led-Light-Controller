#include <winuser.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "Lib/display.hpp"
#include "Lib/serial.hpp"

// Serial Data
Serial serial;

// Data
namespace LED {
        int RED = 0;
        int GREEN = 0;
        int BLUE = 0;
};

std::string HEX = "#000000";
std::string RGB = "rgb(0,0,0)";

const int SERIAL_TIMER = 1;
const int CLIP_TIMER = 2;

void WindowButtons(HWND hWnd);
void Edits(HWND hWnd);
void EditsClick(HWND hWnd);
void ClipboardMessage(HWND hWnd); 

// Data conversion
void ParseData(std::string & msg); // Parses and grabs rgb values from the sent data over serial
void UpdateTextData(); // Updates strings to reflect the new rgb values
std::string getHex(size_t decimal);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);


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

        // Initialize serial Communication
        serial.port = "COM6";
        serial.Init();

        // Serial Communication Loop initialization
        SetTimer(frmMain, SERIAL_TIMER, 50, (TIMERPROC) NULL);

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
bool rgb = false;
bool hex = false;
LPCTSTR cursor = IDC_ARROW;

bool clip_msg = false;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
        switch (event) {
                case WM_SETCURSOR: {
                        HCURSOR hCursor = LoadCursor(NULL, cursor);
                        SetCursor(hCursor);

                        return TRUE;
                }

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
                        Edits(hWnd);

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

                        EditsClick(hWnd);

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
                        DrawEdits(paint);

                        ClipboardMessage(hWnd);

                        EndPaint(hWnd, &ps);                        
                        break;
                }

                case WM_TIMER: {
                        switch (wParam) {
                                case CLIP_TIMER: {

                                        clip_msg = false;
                                        // Force repainting
                                        RedrawWindow(hWnd,NULL, NULL, RDW_INVALIDATE);
                                        KillTimer(hWnd, CLIP_TIMER);

                                        break;
                                }
                                        
                                // Serial Communication Loop
                                case SERIAL_TIMER: {
                                        if (!serial.Read()) {
                                                ParseData(serial.data);
                                                UpdateTextData();
                                                RedrawWindow(hWnd,NULL, NULL, RDW_INVALIDATE);

                                                serial.data.clear();
                                        } 
                                        

                                        break;
                                }

                        }

                        break;
               }

        }
        

        return DefWindowProc(hWnd, event, wParam, lParam);
}

void ClipboardMessage(HWND hWnd) {
        if (!clip_msg) return; // If not copied from clipboard dont show

        Paint paint;
        paint.area = GetDC(hWnd);

        const int WIDTH = 120;
        const int HEIGHT = 40;
        const std::string msg = "Copied!";
        const int PADDING = 20;
        
        // Container
        paint.x = frmMainData.WIDTH - PADDING - WIDTH;
        paint.y = (frmMainData.HEIGHT + frmMainData.TITLE_BAR) - PADDING - HEIGHT;
        paint.xend += paint.x + WIDTH;
        paint.yend += paint.y + HEIGHT;
        paint.color = RGB(238, 212, 159);
        paint.RoundRect(10);
       
        paint.border.width = 2;
        paint.border.color = 0;
        const int PADDING_CONTAINER = 3;
        paint.x += PADDING_CONTAINER; 
        paint.y += PADDING_CONTAINER;
        paint.xend -= PADDING_CONTAINER;
        paint.yend -= PADDING_CONTAINER;
        paint.RoundRect(10);

        // Text
        paint.color = RGB(24, 25, 38);
        paint.x += 20;
        paint.y += 3;
        paint.font.name = "Montserrat";
        paint.font.size = 25;
        paint.font.weight = 300;
        paint.font.italics = TRUE;
        paint.Text(msg);

        ReleaseDC(hWnd, paint.area);
}

void CopyToClipboard(HWND hWnd, std::string value) { 
        if (!OpenClipboard(hWnd)) return;

        if(!EmptyClipboard()) {
                CloseClipboard();
                return;
        }

        const char * buf = value.c_str();
        const size_t len = strlen(buf) + 1; // Account for terminating char????

        // Memory allocation
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len); // Allocate memory accessable to whole application or OS?
        memcpy(GlobalLock(hMem), buf, len); // Actually put buffer into memory we allocated
        GlobalUnlock(hMem); // Allow data in allocated memory to be accessed

        if (!SetClipboardData(CF_TEXT, hMem)) {
                CloseClipboard();       
                GlobalFree(hMem);
                return;
        }

        CloseClipboard();       
        GlobalFree(hMem);

        // Draw message confirming copying to clipboard
        clip_msg = true;
        ClipboardMessage(hWnd);

        // Set timer which disables message in 2s
        SetTimer(hWnd, CLIP_TIMER, 2000, (TIMERPROC) NULL);
}

void EditsClick(HWND hWnd) {
        if (rgb) {
                CopyToClipboard(hWnd, RGB); 
        } else if (hex) {
                CopyToClipboard(hWnd, HEX); 
        }

}

void DrawEdits(Paint & paint) {
        paint.Reset();
        
        paint.x = 250;
        paint.y = 125;
        paint.xend = paint.x + EDIT_WIDTH;
        paint.yend = paint.y + EDIT_HEIGHT;
        paint.border.color = RGB(203, 166, 247);
        paint.border.width = 1;
        paint.color = RGB(41, 44, 60);
        paint.RoundRect(10);

        paint.color = RGB(205, 214, 244);
        paint.font.size = EDIT_HEIGHT - PADDING;
        paint.font.weight = 500;
        paint.font.italics = TRUE;
        paint.x += PADDING*3;
        paint.Text(RGB);

        paint.x = 250;
        paint.y = 175;
        paint.xend = paint.x + EDIT_WIDTH;
        paint.yend = paint.y + EDIT_HEIGHT;
        paint.border.color = RGB(203, 166, 247);
        paint.border.width = 1;
        paint.color = RGB(41, 44, 60);
        paint.RoundRect(10);

        paint.color = RGB(205, 214, 244);
        paint.font.size = EDIT_HEIGHT - PADDING;
        paint.font.weight = 500;
        paint.font.italics = TRUE;
        paint.y += PADDING/2;
        paint.x += PADDING*3;
        paint.Text(HEX);
}

void DrawLED(Paint & paint) {
        paint.Reset();
        
        const int LED_DIAMETER = 100;
        const int START_X = 75;
        const int START_Y = 75;

        // Circle
        paint.x = START_X;
        paint.xend = paint.x + LED_DIAMETER;
        paint.y = START_Y;
        paint.yend = paint.y + LED_DIAMETER;
        paint.color = RGB(LED::RED, LED::GREEN, LED::BLUE); 
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

void Edits(HWND hWnd) {
        rgb = false;
        hex = false;

        if (frmMainData.dragging) return;

        GetCursorPos(&mouse);
        ScreenToClient(hWnd, &mouse); // Convert mouse position to pos on client area
        
        const bool inrange_x = mouse.x >= 250 && mouse.x <= 250 + EDIT_WIDTH;
        if (inrange_x && (mouse.y >= 125 && mouse.y <= 125 + EDIT_HEIGHT)) {
                rgb = true;
                hex = false;
        } else if (inrange_x && (mouse.y >= 175 && mouse.y <= 175 + EDIT_HEIGHT)) {
                rgb = false;
                hex = true;
        } else {
                cursor = IDC_ARROW;
                return;
        }

        cursor = IDC_HAND;
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

void ParseData(std::string & msg) {
        /*
         * Expects data to be in RED,GREEN,BLUE format
         * Delimeter = ','
         *
         */
        const int TEMP_RED = LED::RED;
        const int TEMP_GREEN = LED::GREEN;
        const int TEMP_BLUE = LED::BLUE;

        size_t delim;

        try {
                // Copy red channel value
                delim = msg.find(",");
                if (delim == std::string::npos) { 
                        return;
                }

                LED::RED = std::stoi(msg.substr(0,delim));
                msg = msg.substr(delim+1);

                // Copy green channel value
                delim = msg.find(",");
                if (delim == std::string::npos) { 
                        return;
                }

                LED::GREEN = std::stoi(msg.substr(0,delim));
                msg = msg.substr(delim+1);

                LED::BLUE = std::stoi(msg);
        } catch (...) {
                // Reroll
                LED::RED = TEMP_RED;
                LED::GREEN = TEMP_GREEN;
                LED::BLUE = TEMP_BLUE;

                return;
        }
        
}

std::string getHex(size_t decimal) {
        if (decimal == 0) return "00";

        std::string hex = "";

        do {
                int rem = decimal % 16;
                decimal /= 16;                

                if (rem < 10) {
                        hex = std::to_string(rem) + hex;
                } else {
                        // Get letter associated with hex
                        char hex_char = 'A' + (rem - 10);

                        hex = hex_char + hex;
                }
        } while (decimal != 0);

        return hex;
}

void UpdateTextData() {
        RGB = "";
        HEX = "";
        
        // Get RGB values
        RGB = "rgb(";
        RGB += std::to_string(LED::RED) + ",";
        RGB += std::to_string(LED::GREEN) + ",";
        RGB += std::to_string(LED::BLUE);
        RGB += ")";

        // Convert to hexadecimal
        HEX = "#";
        HEX += getHex(LED::RED);
        HEX += getHex(LED::GREEN);
        HEX += getHex(LED::BLUE);
}
