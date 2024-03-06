#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent, wxSize size, std::vector<std::vector<bool>>& board) : wxPanel(parent, wxID_ANY, wxPoint(0,0)), gameBoardRef(board)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this); 
	this->Bind(wxEVT_LEFT_UP, &DrawingPanel::mouseEvent, this);
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
			graphicsContext->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
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