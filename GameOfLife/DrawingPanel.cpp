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
}