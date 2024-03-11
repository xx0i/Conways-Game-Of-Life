#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include "SettingsStruct.h"

class SettingsDialogUI : public wxDialog
{
	wxDECLARE_EVENT_TABLE();

public:
	SettingsDialogUI(wxWindow* parent, wxWindowID id, const wxString& title, Settings* setting); //constructor
private:
	wxWindow* parentWindow; //wxWindow pointer
	wxSpinCtrl* gridSizeSpinCtrl; //grid spin ctrl
	wxSpinCtrl* intervalTimeSpinCtrl; //interval spin ctrl
	wxColourPickerCtrl* liveColourPickerCtrl; //live colour picker ctrl
	wxColourPickerCtrl* deadColourPickerCtrl; //dead colour picker ctrl
	Settings* settings; //settings pointer
	void okButtonEvent(wxCommandEvent&); //ok button event
	void cancelButtonEvent(wxCommandEvent&); //cancel button event
};