#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
class MainWindow : public wxFrame
{
public:
	//constructor
	MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~MainWindow() {} //destructor

private:
	DrawingPanel* drawingPanel;
};