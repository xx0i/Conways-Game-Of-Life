#pragma once
#include "wx/wx.h"
class DrawingPanel : public wxPanel
{
public:
	DrawingPanel(wxWindow* parent, wxSize size); //constructor
	~DrawingPanel() {} //destructor
	void setSize(wxSize&); //set size method
	void setGridSize(int newSize); //set grid size method - sets the drawing panel grid size to the same value as the main window grid size

private:
	void OnPaint(wxPaintEvent&); //onPaint method
	int gridSize = 0; //grid size variable
};