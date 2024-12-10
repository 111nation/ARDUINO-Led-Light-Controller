#include "font.h"

Font::~Font() {
	if (hFont) {
		DeleteObject(hFont);
		hFont = nullptr;
	}
}

void Font::Reset() {
	// Reset attributes
	size = 24;
	italics = FALSE;
	underline = FALSE;
	weight = FW_NORMAL;
}

HFONT Font::Use() {	
	hFont = CreateFont(
		size,
		0, 0, 0,
		weight, italics, underline, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		0, 0,
		name.c_str());	

	if (Valid() == false) {
		DefaultFont();
	}

	return hFont;

}

bool Font::Valid() {
	if (hFont == nullptr) return false;
	return true;
}

void Font::DefaultFont() {
	std::string font_name = "Roboto";

	hFont = CreateFont(size, 0, 0, 0, FW_NORMAL, 
         			FALSE, FALSE, FALSE, // Italics, Underline, Strikeout
                		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                		0, font_name.c_str());// Default font
}
