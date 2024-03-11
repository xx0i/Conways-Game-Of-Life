#pragma once
#include "wx/wx.h"
class DrawingPanel : public wxPanel
{
	wxDECLARE_EVENT_TABLE();

public:
	DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, int& livingCells, int& generationsRef, wxStatusBar*& statsBar); //constructor
	~DrawingPanel() {} //destructor
	void setSize(wxSize&); //set size method
	void setGridSize(int newSize); //set grid size method - sets the drawing panel grid size to the same value as the main window grid size

private:
	void OnPaint(wxPaintEvent&); //onPaint method
	int gridSize = 0; //grid size variable
	std::vector<std::vector<bool>>& gameBoardRef; //vector ref - gets initialized in constructor
	void mouseEvent(wxMouseEvent&); //mouse interaction event
	int& livingCellRef; //int ref - gets initialized in constructor
	int& generationsRef; //int ref - gets initialized in constructor
	wxStatusBar*& statusBarRef; //status bar ref - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method (drawingpanel vers)
};