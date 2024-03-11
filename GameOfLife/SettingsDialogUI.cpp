#include "SettingsDialogUI.h"

//EVENT TABLE
wxBEGIN_EVENT_TABLE(SettingsDialogUI, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialogUI::okButtonEvent)
EVT_BUTTON(wxID_CANCEL, SettingsDialogUI::cancelButtonEvent)
wxEND_EVENT_TABLE()

SettingsDialogUI::SettingsDialogUI(wxWindow* parent, wxWindowID id, const wxString& title, Settings* setting) : wxDialog(parent, wxID_ANY, "Settings"), settings(setting)
{
	wxBoxSizer* mainBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainBox);

	//spin control
	wxBoxSizer* gridSpinCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText gridControl(this, wxID_ANY, "Grid Size:");
	gridSizeSpinCtrl = new wxSpinCtrl(this, 11093, std::to_string(settings->gridSize), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, settings->gridSize, "wxSpinCtrlGrid");

	wxBoxSizer* intervalSpinCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText staticControl(this, wxID_ANY, "Timer Interval - in miliseconds:");
	intervalTimeSpinCtrl = new wxSpinCtrl(this, 14210, std::to_string(settings->milisec4timer), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, settings->milisec4timer, "wxSpinCtrlInterval");
	
	//paint control
	wxBoxSizer* liveColourCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText liveControl(this, wxID_ANY, "Live Cell Colour:");
	liveColourPickerCtrl = new wxColourPickerCtrl(this, 15622, *wxLIGHT_GREY, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE, wxDefaultValidator, wxColourPickerCtrlNameStr);

	wxBoxSizer* deadColourCtrl = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText deadControl(this, wxID_ANY, "Dead Cell Colour:");
	deadColourPickerCtrl = new wxColourPickerCtrl(this, 15622, *wxWHITE, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE, wxDefaultValidator, wxColourPickerCtrlNameStr);
	

	//ok and cancel buttons
	wxSizer* okCancel = CreateButtonSizer(wxOK | wxCANCEL);
}

void SettingsDialogUI::okButtonEvent(wxCommandEvent&)
{
	settings->gridSize = gridSizeSpinCtrl->GetValue();
	settings->milisec4timer = intervalTimeSpinCtrl->GetValue();
	settings->SetLivingColour(liveColourPickerCtrl->GetColour());
	settings->setDeadColour(deadColourPickerCtrl->GetColour());
	EndModal(wxID_OK);
}

void SettingsDialogUI::cancelButtonEvent(wxCommandEvent&)
{
	EndModal(wxID_CANCEL);
}