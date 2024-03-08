#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::WindowResize)
EVT_MENU(12789, MainWindow::playEvent)
EVT_MENU(15731, MainWindow::pauseEvent)
EVT_MENU(18324, MainWindow::nextEvent)
EVT_MENU(16430, MainWindow::clearEvent)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500))
{
	statusBar = CreateStatusBar();
	statusBarUpdate();
	toolBar = CreateToolBar();
	wxBitmap playIcon(play_xpm);
	toolBar->AddTool(12789, "Play", playIcon);
	wxBitmap pauseIcon(pause_xpm);
	toolBar->AddTool(15731, "Pause", pauseIcon);
	wxBitmap nextIcon(next_xpm);
	toolBar->AddTool(18324, "Next", nextIcon);
	wxBitmap trashIcon(trash_xpm);
	toolBar->AddTool(16430, "Clear", trashIcon);
	toolBar->Realize();
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

void MainWindow::playEvent(wxCommandEvent&)
{
}

void MainWindow::pauseEvent(wxCommandEvent&)
{
}

void MainWindow::nextEvent(wxCommandEvent&)
{
}

void MainWindow::clearEvent(wxCommandEvent&)
{
}

int MainWindow::neighborCount(int row, int col)
{
	//variable to return the livingNeighbors
	int livingNeighbors = 0;

	for (int i = (row - 1); i <= (row + 1); i++) {
		for (int j = (col - 1); j <= (col + 1); j++) {


			if (i < 0 || i >= gridSize || j < 0 || j >= gridSize || (i == row && j == col) || (!gameBoard[i][j])) {
				continue;
			}
			else if (gameBoard[i][j]) {
				livingNeighbors++;
			}
		}
	}
	return livingNeighbors;
}