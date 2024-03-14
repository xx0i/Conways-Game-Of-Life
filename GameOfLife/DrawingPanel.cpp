#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::mouseEvent)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, wxStatusBar*& statsBar, Settings* setting, int& livingCells, int& generation) : wxPanel(parent, wxID_ANY, wxPoint(0, 0)), gameBoardRef(board), statusBarRef(statsBar), settings(setting), livingCellsRef(livingCells), generationRef(generation)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void DrawingPanel::setSize(wxSize& size)
{
	this->SetSize(size);
}

void DrawingPanel::OnPaint(wxPaintEvent&)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* graphicsContext = wxGraphicsContext::Create(dc);
	if (!graphicsContext) {
		return;
	}
	graphicsContext->SetPen(*wxBLACK);

	//creating the grid
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)settings->gridSize;
	float cellHeight = panelSize.GetHeight() / (float)settings->gridSize;

	for (int i = 0; i < settings->gridSize; i++) {
		for (int j = 0; j < settings->gridSize; j++) {
			//calculating cell location
			int x = i * cellWidth;
			int y = j * cellHeight;

			//rectangle coulour based on true or false
			if (gameBoardRef[i][j]) {
				graphicsContext->SetBrush(settings->GetLivingColour());
			}
			else {
				graphicsContext->SetBrush(settings->GetDeadColour());
			}

			graphicsContext->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
	delete graphicsContext;
}

void DrawingPanel::mouseEvent(wxMouseEvent& event)
{
	//x and y coordinates
	float xCoord = event.GetX();
	float yCoord = event.GetY();

	//cell width and cell height
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)settings->gridSize;
	float cellHeight = panelSize.GetHeight() / (float)settings->gridSize;

	//row and column
	float row = xCoord / cellWidth;
	float column = yCoord / cellHeight;

	if (gameBoardRef[row][column]) {
		gameBoardRef[row][column] = false;
		livingCellsRef--;
		statusBarUpdate();
	}
	else {
		gameBoardRef[row][column] = true;
		livingCellsRef++;
		statusBarUpdate();
	}
	Refresh();
}

//same as MainWindow Status Bar Update - so that the living cell count will update as the user explicitly turns cells on or off
void DrawingPanel::statusBarUpdate()
{
	wxString statusText = wxString::Format("Living Cells: %d, Generations: %d",
		livingCellsRef, generationRef);
	statusBarRef->SetStatusText(statusText);
}