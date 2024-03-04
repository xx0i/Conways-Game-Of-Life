#include "MainWindow.h"

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(parent, id, title, pos, size)
{
	drawingPanel = new DrawingPanel(this);
}
