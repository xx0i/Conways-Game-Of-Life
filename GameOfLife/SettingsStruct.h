#pragma once
#include "wx/wx.h"
#include "fstream"

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

	//accessor methods to get wxColour based on the RGB and Alpha values
	wxColor GetLivingColour()const {
		return wxColor(livingRed, livingGreen, livingBlue, livingAlpha);
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

	//Load Data
	void loadData()const {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		if (file) {
			file.read((char*)this, sizeof(Settings));
			file.close();
		}
	}
	//Save Data
	void saveData()const {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		if (file) {
			file.write((char*)this, sizeof(Settings));
			file.close();
		}
	}
	bool isShowNeighbourCount = true; //show neighbour count
	bool isFiniteUniverse = true; //manages universe type
	bool isShowGrid = true; //manages whether or not the grid shows
	bool isGridLines = true; //manages whether or not the 10x10 grid lines shows

	//reset to default
	void resetToDefault() {
		livingRed = 217;
		livingGreen = 217;
		livingBlue = 214;
		livingAlpha = 255;

		deadRed = 255;
		deadGreen = 255;
		deadBlue = 255;
		deadAlpha = 255;

		gridSize = 15;
		milisec4timer = 50;

		isShowNeighbourCount = true;
		isFiniteUniverse = true;
		isShowGrid = true;
		isGridLines = true;
	}
};