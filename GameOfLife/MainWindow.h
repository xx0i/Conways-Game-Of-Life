#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"

class MainWindow : public wxFrame
{
	wxDECLARE_EVENT_TABLE();

public:
	MainWindow(); //constructor
	~MainWindow() {} //destructor
	void WindowResize(wxSizeEvent&); //resize event
	int gridSize = 15; //grid size variable
	void gridInitialize(); //grid initialize method

private:
	DrawingPanel* drawingPanel = nullptr; //DrawingPanel* - used to create a DrawingPanel that's a child of the MainWindow
	std::vector<std::vector<bool>> gameBoard; //gameBoard vector full of vectors, filled with bools (all bools default to false)
	int livingCells = 0; //living cells int
	int generations = 0; //generations int
	wxStatusBar* statusBar = nullptr; //status bar - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method
};