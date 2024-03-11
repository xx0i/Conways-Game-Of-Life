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
EVT_TIMER(13124, MainWindow::timerEvent)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500))
{
	//status bar
	statusBar = CreateStatusBar();
	statusBarUpdate();
	//tool bar
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
	//timer
	timer = new wxTimer(this, 13124);
	//drawing panel and grid initialization
	drawingPanel = new DrawingPanel(this, wxSize(100, 100), gameBoard, statusBar, &settings);
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
	gameBoard.resize(settings.gridSize);
	for (int i = 0; i < settings.gridSize; i++) {
		gameBoard[i].resize(settings.gridSize);
	}
}

void MainWindow::statusBarUpdate()
{
	wxString statusText = wxString::Format("Living Cells: %d, Generations: %d",
		settings.livingCells, settings.generation);
	statusBar->SetStatusText(statusText);
}

void MainWindow::playEvent(wxCommandEvent&)
{
	timer->Start(settings.milisec4timer);
}

void MainWindow::pauseEvent(wxCommandEvent&)
{
	timer->Stop();

}

void MainWindow::nextEvent(wxCommandEvent&)
{
	nextGeneration();
}

void MainWindow::clearEvent(wxCommandEvent&)
{
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			gameBoard[i][j] = false;
		}
	}
	settings.livingCells = 0;
	settings.generation = 0;
	statusBarUpdate();
	drawingPanel->Refresh();
	timer->Stop();
}

int MainWindow::neighborCount(int row, int col)
{
	//variable to return the livingNeighbors
	int livingNeighbors = 0;

	for (int i = (row - 1); i <= (row + 1); i++) {
		for (int j = (col - 1); j <= (col + 1); j++) {


			if (i < 0 || i >= settings.gridSize || j < 0 || j >= settings.gridSize || (i == row && j == col) || (!gameBoard[i][j])) {
				continue;
			}
			else if (gameBoard[i][j]) {
				livingNeighbors++;
			}
		}
	}
	return livingNeighbors;
}

void MainWindow::nextGeneration()
{
	//initialize sandbox
	std::vector<std::vector<bool>>sandbox;
	sandbox.resize(gameBoard.size());
	for (int s = 0; s < gameBoard.size(); s++) {
		sandbox[s].resize(gameBoard.size());
	}
	//game of life rules (checks gameboard - applies to sandbox)
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			int neighbors = neighborCount(i, j);

			if (gameBoard[i][j] && neighbors < 2) {
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] && neighbors > 3) {
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] && (neighbors == 2 || neighbors == 3)) {
				sandbox[i][j] = true;
			}
			else if (!gameBoard[i][j] && neighbors == 3) {
				sandbox[i][j] = true;
			}
		}
	}
	//sandbox living cells check (separated into separate loop for ease)
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			if (sandbox[i][j]) {
				settings.livingCells++;
			}
		}
	}
	gameBoard.swap(sandbox);
	settings.generation++;
	statusBarUpdate();
	drawingPanel->Refresh();
}

void MainWindow::timerEvent(wxTimerEvent&)
{
	nextGeneration();
}