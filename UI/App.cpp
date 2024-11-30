#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Compiler GUI");
	mainFrame->SetClientSize(1280,720);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
