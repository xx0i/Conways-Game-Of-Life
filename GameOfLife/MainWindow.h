#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"

class MainWindow : public wxFrame
{
	wxDECLARE_EVENT_TABLE();

public:
	MainWindow(); //constructor
	~MainWindow() { delete timer; } //destructor
	void WindowResize(wxSizeEvent&); //resize event
	int gridSize = 15; //grid size variable
	void gridInitialize(); //grid initialize method

private:
	DrawingPanel* drawingPanel = nullptr; //DrawingPanel* - used to create a DrawingPanel that's a child of the MainWindow
	std::vector<std::vector<bool>> gameBoard; //gameBoard vector full of vectors, filled with bools (all bools default to false)
	int livingCells = 0; //living cells int
	int generation = 0; //generations int
	wxStatusBar* statusBar = nullptr; //status bar - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method
	wxToolBar* toolBar = nullptr; //tool bar - initalized in the constructor
	void playEvent(wxCommandEvent&); //command event - called when the play icon is clicked
	void pauseEvent(wxCommandEvent&); //command event - called when the pause icon is clicked
	void nextEvent(wxCommandEvent&); //command event - called when the next icon is clicked
	void clearEvent(wxCommandEvent&); //command event - called when the trash icon is clicked
	int neighborCount(int row, int col); //calculating the neighbor count method
	void nextGeneration(); //next generation method
	wxTimer* timer = nullptr; //timer pointer for the timer method
	void timerEvent(wxTimerEvent&); //timer event handler - called when timer fires
	int milisec4timer = 50;
};