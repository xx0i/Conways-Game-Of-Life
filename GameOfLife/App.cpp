#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {
	//MEMORY LEAK DETECTION
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); // set block of memory to find memory leak
	_CrtDumpMemoryLeaks();

	MainWindow* mainWindowPtr = new MainWindow(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(200, 200)); //Instantiate the MainWindow pointer
	mainWindowPtr->Show();	//Call the Show() method on the pointer.

	return true;
}