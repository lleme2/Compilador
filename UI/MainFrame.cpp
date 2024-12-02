

#define PATH_LOG "d:/Users/Home/Documents/GitHub/demo/Compilador/log.txt"

#include "MainFrame.h"
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <string>  
#include <vector>
#include <wx/filedlg.h> 

void DataMemory(std::string instruction);

struct rotulos {
    int rotulo;
    int linha;
};

std::string PATH_OUTPUT = "d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler.exe";
std::string PATH_COMPILER = "d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler.cpp";
std::vector<std::string> instructions;
std::vector<std::string> memory(50, " ");
std::vector<rotulos> rotulosVec;
wxString filePathCode;
std::string saida_values;

int pc = 0;
int s;

enum IDs {
    BUTTON_ID = 2,
    BUTTON_DATA_MEMORY_ID = 5,
    TEXT_ID = 3,
    STATIC_TEXT_ID = 4,
    ID_MENU_NEW = 101,
    ID_MENU_OPEN = 102,
    ID_MENU_OPEN2 = 104,
    ID_MENU_EXIT = 103,
    ID_MENU_PREFERENCES = 104
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_ID, MainFrame::OnButtonClicked)
EVT_BUTTON(BUTTON_DATA_MEMORY_ID, MainFrame::OnDataMemoryButtonClicked)
EVT_MENU(ID_MENU_NEW, MainFrame::OnMenuNew)
EVT_MENU(ID_MENU_OPEN, MainFrame::OnMenuOpen)
EVT_MENU(ID_MENU_OPEN2, MainFrame::OnMenuOpen2)
EVT_MENU(ID_MENU_EXIT, MainFrame::OnMenuExit)
EVT_MENU(ID_MENU_PREFERENCES, MainFrame::OnMenuPreferences)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(192, 192, 192));

    wxButton* button = new wxButton(panel, BUTTON_ID, "Compilar", wxPoint(50, 50), wxSize(100, 35));

    wxButton* dataMemoryButton = new wxButton(panel, BUTTON_DATA_MEMORY_ID, "Executar Memoria", wxPoint(200, 50), wxSize(150, 35));

    textCtrl = new wxTextCtrl(panel, TEXT_ID, "Memoria de dados", wxPoint(450, 145), wxSize(200, 400), wxTE_MULTILINE);
    textCtrl->SetEditable(false);
    textCtrl2 = new wxTextCtrl(panel, TEXT_ID, "Codigo de maquina", wxPoint(0, 145), wxSize(400, 400), wxTE_MULTILINE);
    textCtrl2->SetEditable(false);
    saida_dados = new wxTextCtrl(panel, TEXT_ID, "Saida de dados", wxPoint(100, 600), wxSize(200, 100), wxTE_MULTILINE);
    saida_dados->SetEditable(false);
    codigo = new wxTextCtrl(panel, TEXT_ID, "Codigo", wxPoint(700, 145), wxSize(500, 400), wxTE_MULTILINE);
    Erros = new wxTextCtrl(panel, TEXT_ID, "Saida de Erros", wxPoint(700, 600), wxSize(500, 100), wxTE_MULTILINE);
    Erros->SetEditable(false);
    // Criação da barra de menus
    wxMenuBar* menuBar = new wxMenuBar;

    // Menu "Arquivo"
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_MENU_NEW, "&Novo\tCtrl-N");
    fileMenu->Append(ID_MENU_NEW, "&Novo\tCtrl-N");
    fileMenu->Append(ID_MENU_OPEN2, "&Abrir Codigo\tCtrl-O");
    fileMenu->Append(ID_MENU_OPEN, "&Abrir Codigo Maquina\tCtrl-O");
    fileMenu->Append(ID_MENU_EXIT, "E&xit\tAlt-X");
    menuBar->Append(fileMenu, "&Arquivo");

    // Menu "Opções"
    wxMenu* optionsMenu = new wxMenu;
    optionsMenu->Append(ID_MENU_PREFERENCES, "&Preferencias");
    menuBar->Append(optionsMenu, "&Opcoes");

    // Define a barra de menu
    SetMenuBar(menuBar);

    CreateStatusBar();
}

void MainFrame::OnMenuNew(wxCommandEvent& evt) {
    wxMessageBox("Novo arquivo criado!", "Novo", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnMenuOpen(wxCommandEvent& evt) {
    //wxMessageBox("Abrir arquivo selecionado!", "Abrir", wxOK | wxICON_INFORMATION);
    // Cria o diálogo de seleção de arquivo
    wxFileDialog openFileDialog(this, _("Abrir Arquivo"), "", "",
        "Arquivos de Texto (*.txt)|*.txt|Todos os Arquivos (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    // Se o usuário selecionou um arquivo
    if (openFileDialog.ShowModal() == wxID_OK) {
        // Obtém o caminho completo do arquivo selecionado
        wxString filePath = openFileDialog.GetPath();
        wxLogStatus("Arquivo selecionado: %s", filePath);

        // Abrir o arquivo
        FILE* file2 = fopen(filePath.ToStdString().c_str(), "r");

        if (file2 == NULL) {
            wxLogError("Erro ao abrir o arquivo!");
            return;
        }
        else {
            wxLogStatus("Arquivo aberto com sucesso!");
        }

        char buffer[256];
        wxString output;
        int line = 0;
        while (fgets(buffer, sizeof(buffer), file2) != NULL) {
            instructions.push_back(buffer);
            if (std::isdigit(buffer[0])) {
                rotulos rotulo;
                std::string end(1, buffer[0]);
                rotulo.linha = line;
                rotulo.rotulo = stoi(end);
                rotulosVec.push_back(rotulo);
            }
            line++;
            output += wxString(buffer);
        }

        fclose(file2);

        // Exibe o conteúdo no painel
        textCtrl2->SetValue(output);
    }
}

void MainFrame::OnMenuOpen2(wxCommandEvent& evt) {
    //wxMessageBox("Abrir arquivo selecionado!", "Abrir", wxOK | wxICON_INFORMATION);
    // Cria o diálogo de seleção de arquivo
    wxFileDialog openFileDialog(this, _("Abrir Arquivo"), "", "",
        "Arquivos de Texto (*.txt)|*.txt|Todos os Arquivos (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    // Se o usuário selecionou um arquivo
    if (openFileDialog.ShowModal() == wxID_OK) {
        // Obtém o caminho completo do arquivo selecionado
        filePathCode = openFileDialog.GetPath();
        wxLogStatus("Arquivo selecionado: %s", filePathCode);

        // Abrir o arquivo
        FILE* file2 = fopen(filePathCode.ToStdString().c_str(), "r");

        if (file2 == NULL) {
            wxLogError("Erro ao abrir o arquivo!");
            return;
        }
        else {
            wxLogStatus("Arquivo aberto com sucesso!");
        }

        char buffer[256];
        int contador = 0;
        wxString output;
        while (fgets(buffer, sizeof(buffer), file2) != NULL) {
            output += wxString(buffer);
            contador++;
        }

        fclose(file2);

        // Exibe o conteúdo no painel
        codigo->SetValue(output);
    }
}

void MainFrame::OnMenuExit(wxCommandEvent& evt) {
    Close(true);  // Fecha o aplicativo
}

void MainFrame::OnMenuPreferences(wxCommandEvent& evt) {
    wxMessageBox("Preferências selecionadas!", "Preferências", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {
    Erros->Clear();
    wxString code = codigo->GetValue();
    wxLogStatus("Arquivo aberto com sucesso! %s", code);
    FILE* file_codeTXT = fopen("D:/Users/Home/Desktop/CodigoCompilador.txt", "w");
    std::string codeStd = code.ToStdString();
    fprintf(file_codeTXT, "%s", codeStd.c_str());
    fclose(file_codeTXT);
    std::string compileCommand = "g++ " + PATH_COMPILER + " -o " + PATH_OUTPUT;
    int compileResult = system(compileCommand.c_str());
    if (compileResult != 0) {
       wxLogStatus("Deu merda na compilacao");
    }
    std::string command = PATH_OUTPUT;
    int execReturn = system(PATH_OUTPUT.c_str());
    if (execReturn != 0) {
       wxLogStatus("Deu merda na execucao %d", execReturn);
       wxMilliSleep(2000);
    }
    

    wxMilliSleep(2000);  // Espera 2000 milissegundos (2 segundos)

    // Nome do arquivo a ser aberto
    std::string filename = "d:/Users/Home/Documents/GitHub/demo/Compilador/log.txt";

    // Cria um objeto ifstream para abrir o arquivo
    std::ifstream file(filename);

    // Verifica se o arquivo foi aberto com sucesso
    if (!file.is_open()) {
        wxLogStatus("Nao abriu o arquivo");
    }

    // Ler linha por linha até o final do arquivo
    std::string line;
    std::string saida = "";
    while (std::getline(file, line)) {
        // Exibe cada linha lida
        saida += line;
    }

    // Fecha o arquivo
    file.close();

    Erros->SetValue(line);
}

void MainFrame::OnDataMemoryButtonClicked(wxCommandEvent& evt) {
    // Função chamada ao clicar no novo botão
    //wxLogStatus("Executando DataMemory...");
    DataMemory(instructions[pc]);
    saida_dados->SetValue(saida_values);
    // Chama a função que processa as instruções na memória
    if (pc == instructions.size()) {
        pc = 0;
    }
    wxString memoryContent;
    memoryContent << "Topo da pilha: " << s << " Memoria de Dados:\n";
    for (size_t i = 0; i < memory.size(); ++i) {
        if (i == s) {
            memoryContent << "Posicao [" << i << "]: " << wxString(memory[i]) << " <--" << "\n";
        }
        else {
            memoryContent << "Posicao [" << i << "]: " << wxString(memory[i]) << "\n";
        }
       
    }
    

    // Exibe o conteúdo da memória no textCtrl2
    textCtrl->SetValue(memoryContent);
}

bool Pesquisa_rotulo(int rotulo) {
    if (rotulosVec.empty()) {
        return false;
    }
    for (int k = 0; k < rotulosVec.size(); k++) {
        if (rotulosVec[k].rotulo == rotulo) {
            return true;
        }
    }
    return false;
}

int Pesquisa_linha(int rotulo) {
    if (rotulosVec.empty()) {
        return -1;
    }
    for (int k = 0; k < rotulosVec.size(); k++) {
        if (rotulosVec[k].rotulo == rotulo) {
            return rotulosVec[k].linha;
        }
    }
    return -1;
}

void DataMemory(std::string instruction) {
    
        if (instruction.find("START") == 0) {
            s = 0;
            wxLogStatus("START - Comeco do codigo");
            pc++;
        }
        else if (instruction.find("ALLOC") == 0) {
            // Tratamento do ALLOC
            std::string params1 (1, instruction[6]); // Extrai os parâmetros após "ALLOC "
            std::string params2 (1, instruction[10]);
            wxLogStatus("ALLOC %s,%s - Topo: %d", params1, params2,s);
            int start_point = stoi(params1);
            int num_variables = stoi(params2);
            for (int i = 0; i < num_variables; i++) {
                s++;
                memory[s] = memory[start_point + i];
            }
            wxLogStatus("Topo: %d", s);
            pc++;
        }
        else if (instruction.find("DALLOC") == 0) {
            // Tratamento do DALLOC
            std::string params1(1, instruction[7]);
            std::string params2(1, instruction[11]);// Extrai os parâmetros após "DALLOC "
            wxLogStatus("DALLOC %s,%s - Desalocacao variaveis", params1, params2);
            int start_point = stoi(params1);
            int num_variables = stoi(params2);
            for (int i = num_variables-1; i >= 0; i--) {
                memory[start_point +i] = memory[s];
                s--;
            }
            pc++;
        }
        else if (instruction.find("JMP") == 0) {
            // Tratamento do JMP
            char jpmf = instruction[3];
            if (jpmf == 'F') {
                std::string aux(1, instruction[5]);
                int endf = Pesquisa_linha(stoi(aux));
                wxLogStatus("JMPF - Label: %d", endf);
                if (memory[s] == "0") {
                    pc = endf;
                }
                else {
                    pc++;
                }
                s--;
            }
            else {
                std::string aux(1, instruction[4]);; // Extrai o label após "JMP "
                int end = Pesquisa_linha(stoi(aux));
                wxLogStatus("JMP - Label: %d", end);
                pc = end;
            }
        }
        else if (instruction.find("STR") == 0) {
            std::string label = instruction.substr(4);
            if (instruction[4] == 10) {
                memory[0] = memory[s];
                s--;
                pc++;
            }
            else {
                memory[stoi(label)] = memory[s];
                s--;
                pc++;
            }
            wxLogStatus("STR %s - Store realizado.", label);
            
        }
        else if (instruction.find("CMP") == 0) {
            // Tratamento do CMP
            wxLogStatus("CMP - Comparação realizada.");
            pc++;
        }
        else if (instruction.find("LDC") == 0) {
            // Carregar constante
            std::string constant = instruction.substr(4); // Extrai o valor após "LDC "
            wxLogStatus("LDC - Constante: %s", constant);
            s++;
            memory[s] = constant;
            pc++;
        }
        else if (instruction.find("LDV") == 0) {
            // Carregar valor
            std::string address = instruction.substr(4); // Extrai o endereço após "LDV "
            if (instruction[4] == 10) {
                s++;
                memory[s] = memory[0];
                pc++;
            }
            else {
                wxLogStatus("LDV - Endereço: %s", address);
                s++;
                memory[s] = memory[stoi(address)];
                pc++;
            }
            
        }
        else if (instruction.find("ADD") == 0) {
            // Soma
            wxLogStatus("ADD - Soma realizada.");
            int op = stoi(memory[s - 1]) + stoi(memory[s]);
            memory[s - 1] = std::to_string(op);
            s--;
            pc++;
        }
        else if (instruction.find("SUB") == 0) {
            // Subtração
            wxLogStatus("SUB - Subtração realizada.");
            int op = stoi(memory[s - 1]) - stoi(memory[s]);
            memory[s - 1] = std::to_string(op);
            s--;
            pc++;
        }
        else if (instruction.find("MULT") == 0 ) {
            // Multiplicação
            wxLogStatus("MULT - Multiplicação realizada.");
            int op = stoi(memory[s - 1]) * stoi(memory[s]);
            memory[s - 1] = std::to_string(op);
            s--;
            pc++;
        }
        else if (instruction.find("DIV") == 0) {
            // Divisão
            wxLogStatus("DIVI - Divisão realizada.");
            int op = stoi(memory[s - 1]) / stoi(memory[s]);
            memory[s - 1] = std::to_string(op);
            s--;
            pc++;
        }
        else if (instruction.find("INV") == 0) {
            // Inverter sinal
            wxLogStatus("INV - Sinal invertido.");
            std::string aux = "-" + memory[s];
            memory[s] = aux;
            pc++;
        }
        else if (instruction.find("AND") == 0) {
            // Conjunção
            wxLogStatus("AND - Operação lógica AND realizada.");
            if (memory[s - 1] == "1" && memory[s] == "1") {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++; 
        }
        else if (instruction.find("OR") == 0) {
            // Disjunção
            wxLogStatus("OR - Operação lógica OR realizada.");
            if (memory[s - 1] == "1" || memory[s] == "1") {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("NEG") == 0) {
            // Negação
            wxLogStatus("NEG - Negação realizada.");
            int aux = stoi(memory[s]);
            aux = 1 - aux;
            memory[s] = std::to_string(aux);
            pc++;
        }
        else if (instruction.find("CME") == 0) {
            // Comparar menor
            
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 < num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            wxLogStatus("CME %d < %d %d- Comparação de menor realizada.",num1,num2,s);
            pc++;
        }
        else if (instruction.find("CMA") == 0) {
            // Comparar maior
            wxLogStatus("CMA - Comparação de maior realizada.");
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 > num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("CEQ") == 0) {
            // Comparar igual
            wxLogStatus("CEQ - Comparação de igualdade realizada.");
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 == num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("CDIF") == 0) {
            // Comparar desigual
            wxLogStatus("CDIF - Comparação de desigualdade realizada.");
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 != num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("CMEQ") == 0) {
            // Comparar menor ou igual
            wxLogStatus("CMEQ - Comparação de menor ou igual realizada.");
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 <= num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("CMAQ") == 0) {
            // Comparar maior ou igual
            wxLogStatus("CMAQ - Comparação de maior ou igual realizada.");
            int num1 = stoi(memory[s - 1]);
            int num2 = stoi(memory[s]);
            if (num1 >= num2) {
                memory[s - 1] = "1";
            }
            else {
                memory[s - 1] = "0";
            }
            s--;
            pc++;
        }
        else if (instruction.find("RD") == 0) {
            wxTextEntryDialog dialog(nullptr, "Digite um valor:", "Entrada de Dados");
            if (dialog.ShowModal() == wxID_OK) {
                std::string userInput = dialog.GetValue().ToStdString();
                wxLogStatus("RD - Valor lido: %s", userInput);
                s++;
                memory[s] = userInput; // Armazena o valor lido na memória
            }
            else {
                wxLogStatus("RD - Entrada cancelada.");
            }
            pc++;
        }
        else if (instruction.find("PRN") == 0) {
            // Exibição do valor atual (print interativo)
            std::string value = memory[s];
            //wxMessageBox("Valor: " + value, "Imprimir", wxOK | wxICON_INFORMATION);
            saida_values.append(value);
            wxLogStatus("PRN - Valor impresso: %s", value);
            s--;
            pc++;
        }
        else if (instruction.find("RETURN") == 0) {
            pc = stoi(memory[s]);
            s--;
            // Comparar maior ou igual
            wxLogStatus("RETURN - Comando return.");
        }
        else if (instruction.find("HLT") == 0) {
            wxLogStatus("HLT - Comando final de codigo.");
        }
        else if (instruction.find("CALL") == 0) {
            int end;
            std::string rot(1, instruction[5]);
            end = Pesquisa_linha(stoi(rot));
            wxLogStatus("CALL %d- Comando chamada de funcao.", end);
            if (end == -1) {
                exit(-1);
            }
            s++;
            memory[s] = std::to_string(pc + 1);
            pc = end;
        }
        else {
            /*rotulos rotulo;
            std::string end(1,instruction[0]);
            rotulo.linha = pc;
            rotulo.rotulo = stoi(end);
            if (!Pesquisa_rotulo(stoi(end))) {
                rotulosVec.push_back(rotulo);
            }*/
            pc++;
            wxLogStatus("ROTULO %s - Comando final de codigo.", instruction);
        }
        //pc++;
}