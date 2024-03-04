#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
class MainWindow : public wxFrame
{
public:
	//constructor
	MainWindow();
	~MainWindow() {} //destructor
	void WindowResize(wxSizeEvent&);

private:
	DrawingPanel* drawingPanel = nullptr; //DrawingPanel* - used to create a DrawingPanel that's a child of the MainWindow
};