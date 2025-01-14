#pragma once
#include "wx/wx.h"
#include "SettingsStruct.h"

class MainWindow;

class DrawingPanel : public wxPanel
{
	wxDECLARE_EVENT_TABLE();

public:
	DrawingPanel(MainWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, wxStatusBar*& statsBar, Settings* setting, int& livingCells, int& generation, std::vector<std::vector<int>>& neighbours); //constructor
	~DrawingPanel() {} //destructor
	void setSize(wxSize&); //set size method

private:
	MainWindow* parent = nullptr; //MainWindow pointer initialized in constructor
	void OnPaint(wxPaintEvent&); //onPaint method
	std::vector<std::vector<bool>>& gameBoardRef; //vector ref - gets initialized in constructor
	void mouseEvent(wxMouseEvent&); //mouse interaction event
	wxStatusBar*& statusBarRef; //status bar ref - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method (drawing panel vers)
	Settings* settings;
	int& livingCellsRef; //living cells int
	int& generationRef; //generations int
	std::vector<std::vector<int>>& neighboursRef; //vector ref - gets initialized in constructor
};