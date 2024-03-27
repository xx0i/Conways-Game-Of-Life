#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "SettingsStruct.h"

class MainWindow : public wxFrame
{
	wxDECLARE_EVENT_TABLE();

public:
	MainWindow(); //constructor
	~MainWindow() { delete timer; } //destructor
	void WindowResize(wxSizeEvent&); //resize event
	void gridInitialize(); //grid initialize method
	void liveNeighbourCountUpdate(); //live neighbour count update

private:
	DrawingPanel* drawingPanel = nullptr; //DrawingPanel* - used to create a DrawingPanel that's a child of the MainWindow
	std::vector<std::vector<bool>> gameBoard; //gameBoard vector full of vectors, filled with bools (all bools default to false)
	wxStatusBar* statusBar = nullptr; //status bar - initalized in the constructor
	void statusBarUpdate(); //status bar update logic method
	wxToolBar* toolBar = nullptr; //tool bar - initalized in the constructor
	void playEvent(wxCommandEvent&); //command event - called when the play icon is clicked
	void pauseEvent(wxCommandEvent&); //command event - called when the pause icon is clicked
	void nextEvent(wxCommandEvent&); //command event - called when the next icon is clicked
	void clearEvent(wxCommandEvent&); //command event - called when the trash icon is clicked
	int neighbourCount(int row, int col); //calculating the neighbor count method
	void nextGeneration(); //next generation method
	wxTimer* timer = nullptr; //timer pointer for the timer method
	void timerEvent(wxTimerEvent&); //timer event handler - called when timer fires
	Settings settings; //settings struct variable
	wxMenuBar* menuBar = nullptr; //menu bar (only one)
	wxMenu* optionsMenu = nullptr; //options menu (one of many)
	void settingsMenu(wxCommandEvent&); //command event - called when the settings menu is selected
	int livingCells = 0; //living cells int
	int generation = 0; //generations int
	wxMenu* viewMenu = nullptr; //view menu (one of many)
	wxMenuItem* showNeighbourCount = nullptr; //one of the checkable options in the view menu
	void showNeighbourCountEvent(wxCommandEvent&); //show neighbour count event
	std::vector<std::vector<int>> neighbours; //neighbours vector full of vectors of ints
	void refreshMenuItems(); //refreshes menu items
	wxMenu* randomizeMenu = nullptr; //randomize menu (one of many)
	void randomTimeEvent(wxCommandEvent&); //randomize with time event
	void randomSeedEvent(wxCommandEvent&); //randomize with seed event
	wxMenu* fileMenu = nullptr; //file menu (one of many)
	void newEvent(wxCommandEvent&); //new file event
	void openEvent(wxCommandEvent&); //open file event
	void saveEvent(wxCommandEvent&); //save event
	void saveAsEvent(wxCommandEvent&); //save as event
	void exitEvent(wxCommandEvent&); //exit event
	void resetSettingsEvent(wxCommandEvent&); //reset settings to default event
	wxMenuItem* finite = nullptr; //one of the checkable options in the view menu
	wxMenuItem* toroidal = nullptr; //one of the checkable options in the view menu
	void finiteEvent(wxCommandEvent&); //finite universe selection event
	void toroidalEvent(wxCommandEvent&); //toroidal universe selection event
	std::string saveFile; //string for saving the path
	void importEvent(wxCommandEvent&); //import file event
	wxMenuItem* showGrid = nullptr; //one of the checkable options in the view menu
	wxMenuItem* gridLines = nullptr; //one of the checkable options in the view menu
	void showGridEvent(wxCommandEvent&); //show grid event
	void gridLinesEvent(wxCommandEvent&); //show 10x10 grid lines event
	wxMenuItem* showHUD = nullptr; //one of the checkable options in the view menu
	void showHUDEvent(wxCommandEvent&); //show HUD event
};