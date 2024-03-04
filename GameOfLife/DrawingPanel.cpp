#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent) : wxPanel(parent)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
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
	int cellWidth = panelSize.GetWidth() / gridSize;
	int cellHeight = panelSize.GetHeight() / gridSize;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			//calculating cell location
			int x = i * cellWidth;
			int y = j * cellHeight;
			graphicsContext->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
}