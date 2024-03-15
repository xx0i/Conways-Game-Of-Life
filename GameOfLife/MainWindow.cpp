#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "SettingsDialogUI.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::WindowResize)
EVT_MENU(12789, MainWindow::playEvent)
EVT_MENU(15731, MainWindow::pauseEvent)
EVT_MENU(18324, MainWindow::nextEvent)
EVT_MENU(16430, MainWindow::clearEvent)
EVT_TIMER(13124, MainWindow::timerEvent)
EVT_MENU(10002, MainWindow::settingsMenu)
EVT_MENU(17902, MainWindow::showNeighbourCountEvent)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500))
{
	settings.loadData();
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
	//menu bar
	menuBar = new wxMenuBar();
	SetMenuBar(menuBar);
	//view menu
	viewMenu = new wxMenu();
	showNeighbourCount = new wxMenuItem(viewMenu, 17902, "Show Neighbour Count", wxEmptyString, wxITEM_CHECK);
	showNeighbourCount->SetCheckable(true);
	viewMenu->Append(showNeighbourCount);
	menuBar->Append(viewMenu, "View");
	//options menu
	optionsMenu = new wxMenu();
	optionsMenu->Append(10002, "Settings");
	menuBar->Append(optionsMenu, "Options");
	//drawing panel and grid initialization
	drawingPanel = new DrawingPanel(this, wxSize(100, 100), gameBoard, statusBar, &settings, livingCells, generation, neighbours);
	gridInitialize();
	Layout();
	refreshMenuItems();
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
	neighbours.resize(settings.gridSize);
	for (int i = 0; i < settings.gridSize; i++) {
		gameBoard[i].resize(settings.gridSize);
		neighbours[i].resize(settings.gridSize);
	}
}

void MainWindow::statusBarUpdate()
{
	wxString statusText = wxString::Format("Living Cells: %d, Generations: %d",
		livingCells, generation);
	statusBar->SetStatusText(statusText);
}

void MainWindow::playEvent(wxCommandEvent& event)
{
	timer->Start(settings.milisec4timer);
	event.Skip();
}

void MainWindow::pauseEvent(wxCommandEvent& event)
{
	timer->Stop();
	event.Skip();

}

void MainWindow::nextEvent(wxCommandEvent& event)
{
	nextGeneration();
	event.Skip();
}

void MainWindow::clearEvent(wxCommandEvent& event)
{
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			gameBoard[i][j] = false;
			neighbours[i][j] = 0;
		}
	}
	livingCells = 0;
	generation = 0;
	statusBarUpdate();
	drawingPanel->Refresh();
	timer->Stop();
	event.Skip();
}

int MainWindow::neighbourCount(int row, int col)
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
			int count = neighbourCount(i, j);

			if (gameBoard[i][j] && count < 2) {
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] && count > 3) {
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] && (count == 2 || count == 3)) {
				sandbox[i][j] = true;
			}
			else if (!gameBoard[i][j] && count == 3) {
				sandbox[i][j] = true;
			}
		}
	}
	//sandbox living cells check (separated into separate loop for ease)
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			if (sandbox[i][j]) {
				livingCells++;
			}
		}
	}
	gameBoard.swap(sandbox);

	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			int count = neighbourCount(i, j);
			neighbours[i][j] = count;
		}
	}

	generation++;
	statusBarUpdate();
	drawingPanel->Refresh();
}

void MainWindow::timerEvent(wxTimerEvent& event)
{
	nextGeneration();
	event.Skip();
}

void MainWindow::settingsMenu(wxCommandEvent& event)
{
	SettingsDialogUI settingDialouge(this, wxID_ANY, "Settings", &settings);
	if (settingDialouge.ShowModal() == wxID_OK) {
		gridInitialize();
		drawingPanel->Refresh();
	}
	event.Skip();
	settingDialouge.Destroy();
}

void MainWindow::showNeighbourCountEvent(wxCommandEvent& event)
{
	settings.isShowNeighbourCount = showNeighbourCount->IsChecked();
	refreshMenuItems();
	event.Skip();
}

void MainWindow::refreshMenuItems()
{
	showNeighbourCount->Check(settings.isShowNeighbourCount);
	settings.saveData();
}
