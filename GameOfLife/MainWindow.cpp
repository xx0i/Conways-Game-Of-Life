#include "MainWindow.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::WindowResize)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500))
{
	statusBar = CreateStatusBar();
	statusBarUpdate();
	drawingPanel = new DrawingPanel(this, wxSize(100, 100), gameBoard);
	gridInitialize();
	Layout();
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

void MainWindow::gridInitialize()
{
	gameBoard.resize(gridSize);
	for (int i = 0; i < gridSize; i++) {
		gameBoard[i].resize(gridSize);
	}
	drawingPanel->setGridSize(gridSize);
}

void MainWindow::statusBarUpdate()
{
	wxString statusText = wxString::Format("Living Cells: %d, Generations: %d",
		livingCells, generations);
	statusBar->SetStatusText(statusText);
}