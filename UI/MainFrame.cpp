#define PATH_COMPILER "Caminho para compiler.cpp"
#define PATH_OUTPUT "Caminho para output da compilacao"

#include "MainFrame.h"
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <string>  

enum IDs {
    BUTTON_ID = 2,
    TEXT_ID = 3,
    STATIC_TEXT_ID = 4
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_ID, MainFrame::OnButtonClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);

    wxButton* button = new wxButton(panel, BUTTON_ID, "Compilar", wxPoint(50, 50), wxSize(100, 35));

    // Store the reference to wxTextCtrl as a member variable
    textCtrl = new wxTextCtrl(panel, TEXT_ID, "Codigo para ser compilado", wxPoint(400, 145), wxSize(400, 400), wxTE_MULTILINE);

    textCtrl2 = new wxTextCtrl(panel, TEXT_ID, "Codigo para ser compilado", wxPoint(0, 145), wxSize(400, 400), wxTE_MULTILINE);


    CreateStatusBar();
}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {
    // Capture the text from wxTextCtrl
    wxString code = textCtrl->GetValue();

    FILE* file;
    //file = fopen("C:/CodigoParaCompilador.txt", "r");
    file = fopen("D:/Users/Home/Desktop/CodigoCompilador.txt", "w");

    if (file == NULL) {
        wxLogStatus("Nao foi possivel abrir o arquivo");
        wxLogStatus("Nao foi possivel abrir o arquivo");
    }
    else {
        wxLogStatus("Foi possivel abrir o arquivo");
        fprintf(file, code);
    }

    fclose(file);
    
    std::string compileCommand = "g++ d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler.cpp -o d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler";
    int compileResult = system(compileCommand.c_str());
    if (compileResult != 0) {
        wxLogStatus("Deu merda na compilacao");
    }    
    std::string command = "d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler"; // Certifique-se de compilar Compiler.cpp em um execut�vel chamado 'Compiler'
    int execReturn = system(command.c_str());
    if (execReturn != 0) {
        wxLogStatus("Deu merda na execucao");
    }

     FILE* file2;

    file2 = fopen("d:/Users/Home/Documents/GitHub/demo/Compilador/log.txt", "r");

    if (file2 == NULL) {
        wxLogStatus("Erro ao abrir o arquivo de log!");
        return;
    }

    char buffer[256]; // Buffer para armazenar cada linha lida do arquivo
    wxString output;
    while (fgets(buffer, sizeof(buffer), file2) != NULL) {
        output += wxString(buffer);
    }

    // Fecha o arquivo
    fclose(file2);

    // Exibe o conte�do no painel
    textCtrl2->SetValue(output);
    

}