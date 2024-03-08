#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::mouseEvent)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board) : wxPanel(parent, wxID_ANY, wxPoint(0, 0)), gameBoardRef(board)
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
	graphicsContext->SetBrush(*wxWHITE);
	graphicsContext->DrawRectangle(0.0, 0.0, 200.0, 200.0);

	//creating the grid
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)gridSize;
	float cellHeight = panelSize.GetHeight() / (float)gridSize;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			//calculating cell location
			int x = i * cellWidth;
			int y = j * cellHeight;

			//rectangle coulour based on true or false
			if (gameBoardRef[i][j]) {
				graphicsContext->SetBrush(*wxLIGHT_GREY);
			}
			else {
				graphicsContext->SetBrush(*wxWHITE);
			}

			graphicsContext->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
	delete graphicsContext;
}

void DrawingPanel::setGridSize(int newSize)
{
	gridSize = newSize;
}

void DrawingPanel::mouseEvent(wxMouseEvent& event)
{
	//x and y coordinates
	float xCoord = event.GetX();
	float yCoord = event.GetY();

	//cell width and cell height
	wxSize panelSize = this->GetClientSize();
	float cellWidth = panelSize.GetWidth() / (float)gridSize;
	float cellHeight = panelSize.GetHeight() / (float)gridSize;

	//row and column
	float row = xCoord / cellWidth;
	float column = yCoord / cellHeight;

	if (gameBoardRef[row][column]) {
		gameBoardRef[row][column] = false;
	}
	else {
		gameBoardRef[row][column] = true;
	}
	Refresh();
}