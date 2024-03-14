#pragma once
#include "wx/wx.h"
#include "SettingsStruct.h"

class DrawingPanel : public wxPanel
{
	wxDECLARE_EVENT_TABLE();

public:
	DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, wxStatusBar*& statsBar, Settings* setting, int& livingCells, int& generation); //constructor
	~DrawingPanel() {} //destructor
	void setSize(wxSize&); //set size method

private:
	void OnPaint(wxPaintEvent&); //onPaint method
	std::vector<std::vector<bool>>& gameBoardRef; //vector ref - gets initialized in constructor
	void mouseEvent(wxMouseEvent&); //mouse interaction event
	wxStatusBar*& statusBarRef; //status bar ref - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method (drawing panel vers)
	Settings* settings;
	int& livingCellsRef; //living cells int
	int& generationRef; //generations int
};