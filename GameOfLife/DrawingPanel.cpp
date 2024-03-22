#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::mouseEvent)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, wxStatusBar*& statsBar, Settings* setting, int& livingCells, int& generation, std::vector<std::vector<int>>& neighbours) : wxPanel(parent, wxID_ANY, wxPoint(0, 0)), gameBoardRef(board), statusBarRef(statsBar), settings(setting), livingCellsRef(livingCells), generationRef(generation), neighboursRef(neighbours)
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
	if (settings->isShowGrid) {
		wxColour* colour = new wxColour(0,0,0,100);
		graphicsContext->SetPen(*colour);
	}
	else {
		graphicsContext->SetPen(wxTransparentColour);
	}

	//creating the grid
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)settings->gridSize;
	float cellHeight = panelSize.GetHeight() / (float)settings->gridSize;

	graphicsContext->SetFont(wxFontInfo(8), *wxRED); //text font

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

			//show neighbour count drawing text
			if (settings->isShowNeighbourCount) {
				if (neighboursRef[i][j] > 0) {
					wxString cellText = std::to_string(neighboursRef[i][j]);
					double textWidth;
					double textHeight;
					graphicsContext->GetTextExtent(cellText, &textWidth, &textHeight);
					graphicsContext->DrawText(cellText, (i * cellWidth) + cellWidth / 2 - textWidth / 2, (j * cellHeight) + cellHeight / 2 - textHeight / 2);
				}
			}
		}
	}
	if (settings->isGridLines) {
		int numLines = settings->gridSize / 10;
		wxPen* thickPen = new wxPen(*wxBLACK, 4);
		dc.SetPen(*thickPen);
		for (int i = 0; i < numLines; i++) {
			wxPoint vStart(cellWidth * (i * 10), 0);
			wxPoint vEnd(cellWidth * (i * 10), this->GetSize().y);
			dc.DrawLine(vStart, vEnd);
		}
		for (int i = 0; i < numLines; i++) {
			wxPoint vStart(0, cellHeight * (i * 10));
			wxPoint vEnd(this->GetSize().x, cellHeight * (i * 10));
			dc.DrawLine(vStart, vEnd);
		}
	}
	if (settings->isShowHUD) {
		wxSize panelSize = this->GetSize();
		int cellWidth = panelSize.GetWidth();
		int cellHeight = panelSize.GetHeight();

		graphicsContext->SetFont(wxFontInfo(10), *wxRED); //text font
		double x;
		double y;
		wxString windowSize = "Window Size: " + std::to_string(cellWidth) + "x" + std::to_string(cellHeight);
		std::string boundary;
		if (settings->isFiniteUniverse) {
			boundary = "Finite";
		}
		else {
			boundary = "Toroidal";
		}
		wxString boundaryType = "Boundary Type: " + boundary;
		wxString livingCells = "Living Cells: " + std::to_string(livingCellsRef);
		wxString generation = "Generation: " + std::to_string(generationRef);
		wxString timerInterval = "Timer Interval(ms): " + std::to_string(settings->milisec4timer);
		wxString gridSize = "Grid Size: " + std::to_string(settings->gridSize);
		wxString text = windowSize +  '\n' + boundaryType + '\n' + livingCells + '\n' + generation + '\n' + gridSize + '\n' + timerInterval;
		graphicsContext->GetTextExtent(text, &x, &y);
		graphicsContext->DrawText(text, 5, GetSize().y - y - 5);
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