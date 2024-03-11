#pragma once
#include "wx/wx.h"

struct Settings
{
	//unsigned ints for living cells - RGB values default 128 - Alpha default 255
	unsigned int livingRed = 217;
	unsigned int livingGreen = 217;
	unsigned int livingBlue = 214;
	unsigned int livingAlpha = 255;

	//unsigned ints for dead cells - RGB values default 255 - Alpha default 255
	unsigned int deadRed = 255;
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

	int gridSize = 15; //grid size variable - default 15
	int milisec4timer = 50; //interval for time variable - default 50
	int livingCells = 0; //living cells int
	int generation = 0; //generations int

	//accessor methods to get wxColour based on the RGB and Alpha values
	wxColor GetLivingColour()const {
		return wxColor(livingRed,livingGreen, livingBlue, livingAlpha);
	}
	wxColor GetDeadColour()const {
		return wxColor(deadRed, deadGreen, deadBlue, deadAlpha);
	}

	//mutator methods to set the RGB and Alpha values based on the passed wxColor
	void SetLivingColour(const wxColor color) {
		livingRed = color.GetRed();
		livingGreen = color.GetGreen();
		livingBlue = color.GetBlue();
		livingAlpha = color.GetAlpha();
	}
	void setDeadColour(const wxColor color) {
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}
};