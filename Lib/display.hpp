#pragma once 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "PaintLib/paint.h"

struct Form {
        const int TITLE_BAR;
        const int WIDTH;
        const int HEIGHT;
        const int WINDOW_BUTTONS;
        const char* CLASS;
        bool dragging = false;
};

struct Button {
        bool close = false;
        bool minimize = false;
};

// Instancies
struct Form frmMainData = {25, 500, 300, 
        60, "MAIN", false};
struct Button windowButton;


// CONSTANTS
const int EDIT_HEIGHT = 25;
const int EDIT_WIDTH = 150;
const int EDIT_PADDING = 5;
const int TRANSPARENT_COLOR = RGB(24, 25, 30);
const int PADDING = 5;
const int MID_X = frmMainData.WIDTH / 2;
const int MID_Y = (frmMainData.HEIGHT / 2) + frmMainData.TITLE_BAR;
const int MID_TITLE = frmMainData.TITLE_BAR / 2;

// Drawing functions
void DrawButtons(Paint &);
void DrawLED(Paint &);
void DrawEdits(Paint &);


