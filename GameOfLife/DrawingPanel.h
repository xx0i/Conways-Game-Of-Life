#pragma once
#include "wx/wx.h"
class DrawingPanel : public wxPanel
{
public:
	//constructor
	DrawingPanel(wxWindow* parent);
	~DrawingPanel() {} //destructor
private:
	void OnPaint(wxPaintEvent&); //onPaint method
	int gridSize = 15;
};