#pragma once
#include "wx/wx.h"
class DrawingPanel : public wxPanel
{
public:
	//constructor
	DrawingPanel(wxWindow* parent, wxSize size);
	~DrawingPanel() {} //destructor
	void setSize(wxSize&);

private:
	void OnPaint(wxPaintEvent&); //onPaint method
	int gridSize = 15; //grid size variable

};