#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500))
{
	drawingPanel = new DrawingPanel(this, wxSize(100, 100));
	Bind(wxEVT_SIZE, &MainWindow::WindowResize, this);
}

void MainWindow::WindowResize(wxSizeEvent& event)
{
	if (drawingPanel != nullptr) {
		wxSize size = event.GetSize();
		drawingPanel->SetSize(size);
		Refresh();
	}
	event.Skip();
}