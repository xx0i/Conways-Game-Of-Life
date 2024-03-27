#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::mouseEvent)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(MainWindow* parent, wxSize size, std::vector<std::vector<bool>>& board, wxStatusBar*& statsBar, Settings* setting, int& livingCells, int& generation, std::vector<std::vector<int>>& neighbours) : wxPanel(parent, wxID_ANY, wxPoint(0, 0)), parent(parent), gameBoardRef(board), statusBarRef(statsBar), settings(setting), livingCellsRef(livingCells), generationRef(generation), neighboursRef(neighbours)
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
		wxColour* colour = new wxColour(0, 0, 0, 100);
		graphicsContext->SetPen(*colour);
	}
	else {
		graphicsContext->SetPen(wxTransparentColour);
		//increase the size of rectangles by one each direction
	}

	wxColour* colour2 = new wxColour(0, 102, 204, 250); //colour for text font for showNeighbourCount (blue 1-2)
	wxColour* colour3 = new wxColour(0, 153, 0, 250); //colour for text font for showNeighbourCount (green 3-4)
	wxColour* colour4 = new wxColour(179, 0, 179, 250); //colour for text font for showNeighbourCount (purple 5-6)
	wxColour* colour5 = new wxColour(255, 51, 153, 250); //colour for text font for showNeighbourCount (pink 7-8)

	//creating the grid
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)settings->gridSize;
	float cellHeight = panelSize.GetHeight() / (float)settings->gridSize;

	for (int i = 0; i < settings->gridSize; i++) {
		for (int j = 0; j < settings->gridSize; j++) {
			//calculating cell location
			int x = i * cellWidth - 1;
			int y = j * cellHeight - 1;

			//rectangle coulour based on true or false
			if (gameBoardRef[i][j]) {
				graphicsContext->SetBrush(settings->GetLivingColour());
			}
			else {
				graphicsContext->SetBrush(settings->GetDeadColour());
			}

			graphicsContext->DrawRectangle(x, y, cellWidth + 2, cellHeight + 2);

			//show neighbour count drawing text
			if (settings->isShowNeighbourCount) {
				if (neighboursRef[i][j] > 0) {
					if (neighboursRef[i][j] == 1 || neighboursRef[i][j] == 2) {
						graphicsContext->SetFont(wxFontInfo((cellHeight * 0.5)), *colour2); //text font

					}
					else if (neighboursRef[i][j] == 3 || neighboursRef[i][j] == 4) {
						graphicsContext->SetFont(wxFontInfo((cellHeight * 0.5)), *colour3); //text font

					}
					else if (neighboursRef[i][j] == 5 || neighboursRef[i][j] == 6) {
						graphicsContext->SetFont(wxFontInfo((cellHeight * 0.5)), *colour4); //text font

					}
					else {
						graphicsContext->SetFont(wxFontInfo((cellHeight * 0.5)), *colour5); //text font

					}
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
		wxPen* thickPen = new wxPen(*wxBLACK, 3);
		dc.SetPen(*thickPen);
		for (int i = 0; i < numLines; i++) {
			wxPoint vStart(cellWidth * (i + 1) * 10, 0);
			wxPoint vEnd(cellWidth * (i + 1) * 10, this->GetSize().y);
			dc.DrawLine(vStart, vEnd);
		}
		for (int i = 0; i < numLines; i++) {
			wxPoint vStart(0, cellHeight * (i + 1) * 10);
			wxPoint vEnd(this->GetSize().x, cellHeight * (i + 1) * 10);
			dc.DrawLine(vStart, vEnd);
		}
	}
	if (settings->isShowHUD) {
		wxSize panelSize = this->GetSize();
		int cellWidth = panelSize.GetWidth();
		int cellHeight = panelSize.GetHeight();

		graphicsContext->SetFont(wxFontInfo((cellHeight * 0.025)), *wxRED); //text font
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
		wxString text = windowSize + '\n' + boundaryType + '\n' + livingCells + '\n' + generation + '\n' + gridSize + '\n' + timerInterval;
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
	if (settings->isShowNeighbourCount) {
		parent->liveNeighbourCountUpdate();
	}
	else {
		Refresh();
	}
}

//same as MainWindow Status Bar Update - so that the living cell count will update as the user explicitly turns cells on or off
void DrawingPanel::statusBarUpdate()
{
	wxString statusText = wxString::Format("Living Cells: %d, Generations: %d",
		livingCellsRef, generationRef);
	statusBarRef->SetStatusText(statusText);
}