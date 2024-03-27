#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "SettingsDialogUI.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"

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
EVT_MENU(13082, MainWindow::randomTimeEvent)
EVT_MENU(18120, MainWindow::randomSeedEvent)
EVT_MENU(wxID_NEW, MainWindow::newEvent)
EVT_MENU(wxID_OPEN, MainWindow::openEvent)
EVT_MENU(wxID_SAVE, MainWindow::saveEvent)
EVT_MENU(wxID_SAVEAS, MainWindow::saveAsEvent)
EVT_MENU(19012, MainWindow::exitEvent)
EVT_MENU(19882, MainWindow::resetSettingsEvent)
EVT_MENU(13340, MainWindow::finiteEvent)
EVT_MENU(11242, MainWindow::toroidalEvent)
EVT_MENU(17892, MainWindow::importEvent)
EVT_MENU(19200, MainWindow::showGridEvent)
EVT_MENU(11992, MainWindow::gridLinesEvent)
EVT_MENU(13322, MainWindow::showHUDEvent)
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
	//file menu
	fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);
	fileMenu->Append(17892, "Import");
	fileMenu->Append(19012, "Exit");
	menuBar->Append(fileMenu, "File");
	//view menu
	viewMenu = new wxMenu();
	showNeighbourCount = new wxMenuItem(viewMenu, 17902, "Show Neighbour Count", wxEmptyString, wxITEM_CHECK);
	showNeighbourCount->SetCheckable(true);
	viewMenu->Append(showNeighbourCount);
	finite = new wxMenuItem(viewMenu, 13340, "Finite", wxEmptyString, wxITEM_CHECK);
	finite->SetCheckable(true);
	viewMenu->Append(finite);
	toroidal = new wxMenuItem(viewMenu, 11242, "Toroidal", wxEmptyString, wxITEM_CHECK);
	toroidal->SetCheckable(true);
	viewMenu->Append(toroidal);
	showGrid = new wxMenuItem(viewMenu, 19200, "Show Grid", wxEmptyString, wxITEM_CHECK);
	showGrid->SetCheckable(true);
	viewMenu->Append(showGrid);
	gridLines = new wxMenuItem(viewMenu, 11992, "Show 10 x10 Grid Lines", wxEmptyString, wxITEM_CHECK);
	gridLines->SetCheckable(true);
	viewMenu->Append(gridLines);
	showHUD = new wxMenuItem(viewMenu, 13322, "Show HUD", wxEmptyString, wxITEM_CHECK);
	showHUD->SetCheckable(true);
	viewMenu->Append(showHUD);
	menuBar->Append(viewMenu, "View");
	//randomize menu
	randomizeMenu = new wxMenu();
	randomizeMenu->Append(13082, "Randomize (Time)");
	randomizeMenu->Append(18120, "Randomize (Seed)");
	menuBar->Append(randomizeMenu, "Randomize");
	//options menu
	optionsMenu = new wxMenu();
	optionsMenu->Append(10002, "Settings");
	optionsMenu->Append(19882, "Reset Settings to default");
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

			if (settings.isFiniteUniverse) {
				if (i < 0 || i >= settings.gridSize || j < 0 || j >= settings.gridSize || (i == row && j == col) || (!gameBoard[i][j])) {
					continue;
				}
				else if (gameBoard[i][j]) {
					livingNeighbors++;
				}
			}
			else {
				int newI = i;
				int newJ = j;
				if (i < 0) {
					newI = settings.gridSize - 1;
				}
				if (j < 0) {
					newJ = settings.gridSize - 1;
				}
				if (i >= settings.gridSize) {
					newI = 0;
				}
				if (j >= settings.gridSize) {
					newJ = 0;
				}
				if (gameBoard[newI][newJ] && !(i == row && j == col)) {
					livingNeighbors++;
				}
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
	if (showNeighbourCount->IsChecked()) {
		liveNeighbourCountUpdate();
	}
	event.Skip();
}

void MainWindow::refreshMenuItems()
{
	showNeighbourCount->Check(settings.isShowNeighbourCount);
	finite->Check(settings.isFiniteUniverse);
	toroidal->Check(!settings.isFiniteUniverse);
	showGrid->Check(settings.isShowGrid);
	gridLines->Check(settings.isGridLines);
	showHUD->Check(settings.isShowHUD);
	settings.saveData();
	drawingPanel->Refresh();
}

void MainWindow::randomTimeEvent(wxCommandEvent& event)
{
	livingCells = 0;

	srand(time(NULL));

	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			int num = rand();
			gameBoard[i][j] = (num % 3 == 0);
			if (gameBoard[i][j]) {
				livingCells++;
			}
		}
	}
	liveNeighbourCountUpdate();
	statusBarUpdate();
	Refresh();
	event.Skip();
}

void MainWindow::randomSeedEvent(wxCommandEvent& event)
{
	livingCells = 0;

	long seed = wxGetNumberFromUser("Seed", "Enter Seed:", "Randomize", (time(NULL)), 0, LONG_MAX, this, wxDefaultPosition);

	srand(seed);

	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			int num = rand();
			gameBoard[i][j] = (num % 3 == 0);
			if (gameBoard[i][j]) {
				livingCells++;
			}
		}
	}
	liveNeighbourCountUpdate();
	statusBarUpdate();
	Refresh();
	event.Skip();
}

void MainWindow::newEvent(wxCommandEvent& event)
{
	settings.resetToDefault();
	clearEvent(event);
	saveFile.clear();
	statusBarUpdate();
	refreshMenuItems();
	Refresh();
	event.Skip();
}

void MainWindow::openEvent(wxCommandEvent& event)
{
	wxFileDialog fileDialouge(this, "Open File", wxEmptyString, wxEmptyString, "Game of Life (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialouge.ShowModal() == wxID_CANCEL) {
		return;
	}
	for (int i = 0; i < gameBoard.size(); i++) {
		gameBoard[i].clear();
		neighbours[i].clear();
		gameBoard.resize(0);
		neighbours.resize(0);
	}
	gameBoard.clear();
	neighbours.clear();
	gameBoard.resize(0);
	neighbours.resize(0);
	livingCells = 0;

	std::string buffer;
	std::ifstream fileStream;
	std::vector<std::vector<bool>> openVector;
	openVector.resize(0);
	fileStream.open((std::string)fileDialouge.GetPath());
	if (fileStream.is_open()) {
		while (!fileStream.eof()) {
			std::vector<bool> temp;
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			if (buffer.at(0) == '!') { continue; }
			for (int i = 0; i < buffer.size(); i++) {
				if (buffer[i] == '*') {
					temp.push_back(true);
				}
				else {
					temp.push_back(false);
				}
			}
			openVector.push_back(temp);
		}
		fileStream.close();
		gameBoard.resize(openVector.size());
		neighbours.resize(gameBoard.size());
		for (int i = 0; i < gameBoard.size(); i++) {
			gameBoard[i].resize(openVector[i].size());
			neighbours[i].resize(gameBoard[i].size());
		}
		gameBoard.swap(openVector);
		settings.gridSize = gameBoard.size();
		for (int i = 0; i < gameBoard.size(); i++) {
			for (int j = 0; j < gameBoard.size(); j++) {
				if (gameBoard[i][j]) {
					livingCells++;
				}
			}
		}
	}
	Refresh();
	statusBarUpdate();
	event.Skip();
}

void MainWindow::saveEvent(wxCommandEvent& event)
{
	if (!saveFile.empty()) {
		std::ofstream fileStream;
		fileStream.open(saveFile);
		if (fileStream.is_open()) {
			for (int i = 0; i < gameBoard.size(); i++) {
				for (int j = 0; j < gameBoard.size(); j++) {
					if (gameBoard[i][j]) {
						fileStream << '*';
					}
					else {
						fileStream << '.';
					}
				}
				fileStream << '\n';
			}
			fileStream.close();
		}
	}
	else {
		saveAsEvent(event);
	}
	event.Skip();
}

void MainWindow::saveAsEvent(wxCommandEvent& event)
{
	wxFileDialog fileDialouge(this, "Save File", wxEmptyString, wxEmptyString, "Game of Life (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (fileDialouge.ShowModal() == wxID_CANCEL) {
		return;
	}
	saveFile = (std::string)fileDialouge.GetPath();
	std::ofstream fileStream;
	fileStream.open(saveFile);
	if (fileStream.is_open()) {
		for (int i = 0; i < gameBoard.size(); i++) {
			for (int j = 0; j < gameBoard.size(); j++) {
				if (gameBoard[i][j]) {
					fileStream << '*';
				}
				else {
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}
	event.Skip();
}

void MainWindow::exitEvent(wxCommandEvent& event)
{
	Close();
	event.Skip();
}

void MainWindow::resetSettingsEvent(wxCommandEvent& event)
{
	settings.resetToDefault();
	settings.saveData();
	gridInitialize();
	Refresh();
	refreshMenuItems();
	event.Skip();
}

void MainWindow::finiteEvent(wxCommandEvent& event)
{
	settings.isFiniteUniverse = true;
	refreshMenuItems();
	drawingPanel->Refresh();
	event.Skip();
}

void MainWindow::toroidalEvent(wxCommandEvent& event)
{
	settings.isFiniteUniverse = false;
	refreshMenuItems();
	drawingPanel->Refresh();
	event.Skip();
}

void MainWindow::importEvent(wxCommandEvent& event)
{

	wxFileDialog fileDialouge(this, "Import File", wxEmptyString, wxEmptyString, "Game of Life (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialouge.ShowModal() == wxID_CANCEL) {
		return;
	}

	livingCells = 0;
	int size;

	std::string buffer;
	std::ifstream fileStream;
	std::vector<std::vector<bool>> openVector;
	openVector.resize(0);
	fileStream.open((std::string)fileDialouge.GetPath());
	if (fileStream.is_open()) {
		while (!fileStream.eof()) {
			std::vector<bool> temp;
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			if (buffer.at(0) == '!') { continue; }
			if (buffer.size() < gameBoard.size()) {
				size = buffer.size();
			}
			else {
				size = gameBoard.size();
			}
			for (int i = 0; i < size; i++) {
				if (i == size) {
					break;
				}
				if (buffer[i] == '*') {
					temp.push_back(true);
				}
				else {
					temp.push_back(false);
				}
			}
			openVector.push_back(temp);
		}
		fileStream.close();

		//OPTIONAL CENTERING
		int height = openVector.size();
		int width = openVector[0].size();
		if (height > settings.gridSize || width > settings.gridSize) {
			settings.gridSize = (height > width ? height : width) + 10;
			gridInitialize();
		}

		int paddingHeight = (settings.gridSize - height) / 2;
		int paddingWidth = (settings.gridSize - width) / 2;

		if (paddingWidth < 0 || paddingHeight < 0) { return; }

		for (int i = 0; i < openVector.size(); i++) {
			for (int j = 0; j < openVector.size(); j++) {
				gameBoard[i + paddingHeight][j + paddingWidth] = openVector[i][j];
			}
		}

		for (int i = 0; i < gameBoard.size(); i++) {
			for (int j = 0; j < gameBoard.size(); j++) {
				if (gameBoard[i][j]) {
					livingCells++;
				}
			}
		}
	}
	Refresh();
	statusBarUpdate();
	event.Skip();
}

void MainWindow::showGridEvent(wxCommandEvent& event)
{
	settings.isShowGrid = showGrid->IsChecked();
	refreshMenuItems();
	drawingPanel->Refresh();
	event.Skip();
}

void MainWindow::gridLinesEvent(wxCommandEvent& event)
{
	settings.isGridLines = gridLines->IsChecked();
	refreshMenuItems();
	drawingPanel->Refresh();
	event.Skip();
}

void MainWindow::showHUDEvent(wxCommandEvent& event)
{
	settings.isShowHUD = showHUD->IsChecked();
	refreshMenuItems();
	drawingPanel->Refresh();
	event.Skip();
}

void MainWindow::liveNeighbourCountUpdate()
{
	if (neighbours.size() == 0) { return; }
	for (int i = 0; i < settings.gridSize; i++) {
		for (int j = 0; j < settings.gridSize; j++) {
			neighbours[i][j] = neighbourCount(i,j);
		}
	}
	Refresh();
}
