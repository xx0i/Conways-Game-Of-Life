#include "SettingsDialogUI.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(SettingsDialogUI, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialogUI::okButtonEvent)
EVT_BUTTON(wxID_CANCEL, SettingsDialogUI::cancelButtonEvent)
wxEND_EVENT_TABLE()

SettingsDialogUI::SettingsDialogUI(wxWindow* parent, wxWindowID id, const wxString& title, Settings* setting) : wxDialog(parent, wxID_ANY, "Settings",wxDefaultPosition,wxSize(235,250)), settings(setting)
{
	wxBoxSizer* mainBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainBox);

	//spin control
	wxBoxSizer* gridSpinCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* gridControl = new wxStaticText(this, wxID_ANY, "Grid Size:");
	gridControl->Move(wxPoint(10, 5));
	gridSizeSpinCtrl = new wxSpinCtrl(this, 11093, std::to_string(settings->gridSize), wxPoint(130, 5), wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, settings->gridSize, "wxSpinCtrlGrid");

	wxBoxSizer* intervalSpinCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* intervalControl = new wxStaticText(this, wxID_ANY, "Timer Interval\nin miliseconds:");
	intervalControl->Move(wxPoint(15, 55));
	intervalTimeSpinCtrl = new wxSpinCtrl(this, 14210, std::to_string(settings->milisec4timer), wxPoint(130, 55), wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, settings->milisec4timer, "wxSpinCtrlInterval");
	
	//paint control
	wxBoxSizer* liveColourCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* liveControl = new wxStaticText(this, wxID_ANY, "Live Cell Colour:");
	liveControl->Move(wxPoint(10, 105));
	liveColourPickerCtrl = new wxColourPickerCtrl(this, 15622, settings->GetLivingColour(), wxPoint(110, 105), wxDefaultSize, wxCLRP_DEFAULT_STYLE, wxDefaultValidator, wxColourPickerCtrlNameStr);

	wxBoxSizer* deadColourCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* deadControl = new wxStaticText(this, wxID_ANY, "Dead Cell Colour:");
	deadControl->Move(wxPoint(10, 155));
	deadColourPickerCtrl = new wxColourPickerCtrl(this, 15622, settings->GetDeadColour(), wxPoint(110, 155), wxDefaultSize, wxCLRP_DEFAULT_STYLE, wxDefaultValidator, wxColourPickerCtrlNameStr);
	

	//ok and cancel buttons
	wxSizer* okCancel = CreateButtonSizer(wxOK | wxCANCEL);
	// Create a horizontal sizer
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	// Add the buttons to the sizer
	horizontalSizer->Add(okCancel, 0, wxALIGN_CENTER | wxALL, 10);
	// Set the sizer for your dialog or frame
	SetSizer(horizontalSizer);
}

void SettingsDialogUI::okButtonEvent(wxCommandEvent&)
{
	settings->gridSize = gridSizeSpinCtrl->GetValue();
	settings->milisec4timer = intervalTimeSpinCtrl->GetValue();
	settings->SetLivingColour(liveColourPickerCtrl->GetColour());
	settings->setDeadColour(deadColourPickerCtrl->GetColour());
	settings->saveData();
	EndModal(wxID_OK);
}

void SettingsDialogUI::cancelButtonEvent(wxCommandEvent&)
{
	settings->loadData();
	EndModal(wxID_CANCEL);
}