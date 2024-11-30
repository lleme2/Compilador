#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	wxTextCtrl* textCtrl;
	wxTextCtrl* textCtrl2;
	wxTextCtrl* saida_dados;
	wxTextCtrl* codigo;
	wxTextCtrl* Erros;

	// Novas funções de eventos para os menus
	void OnMenuNew(wxCommandEvent& evt);
	void OnMenuOpen(wxCommandEvent& evt);
	void OnMenuOpen2(wxCommandEvent& evt);
	void OnMenuExit(wxCommandEvent& evt);
	void OnMenuPreferences(wxCommandEvent& evt);
	void OnDataMemoryButtonClicked(wxCommandEvent& evt);
	void OnButtonClicked(wxCommandEvent& evt);
	wxDECLARE_EVENT_TABLE();
};

