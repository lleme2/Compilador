#include <stdio.h>
#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

struct ListaDeTokens {
    queue<char> lexema;
    queue<char> simbolo;
};

void imprime_lista_token (){

    const int columnWidth = 25;  // Largura das colunas

    // Imprimindo o cabeçalho
    cout << "|" << setw(columnWidth) << left << "Lexema" << " | "
         << setw(columnWidth) << left << "Simbolo" << " |" << endl;

    // Imprimindo separador
    cout << "|" << setw(columnWidth + 1) << setfill('-') << "|"
         << setw(columnWidth + 2) << "|" << setfill(' ') << endl;

    // Imprimindo os dados
    for (int i = 0; i < 5; ++i) {
        cout << "|" << setw(columnWidth) << left << "Teste A: " + to_string(i) << " | "
             << setw(columnWidth) << left << "Teste B:" + to_string(i) << " |" << endl;
    }

    // Imprimindo o rodapé
    cout << "|" << setw(columnWidth + 1) << setfill('-') << "|"
         << setw(columnWidth + 2) << "|" << setfill(' ') << endl;
}

void PegaToken(FILE *file, char caractere){

    while(caractere != EOF){
        cout << caractere;
        if(caractere != '{' || caractere != ' '){
            break;
        }
        caractere = fgetc(file);
    }

}

void TrataEspacoComentario(FILE *file){
    char caractere;

    // Ler o primeiro caractere do arquivo
    caractere = fgetc(file);

    // Enquanto nao acabou o arquivo fonte
    while (caractere != EOF) {
        //elimina comentarios e espacos
        while ((caractere == '{' || caractere == ' ') && (caractere != EOF)) {
            if (caractere == '{') {
                // espera caractere de fechamento de comentario
                while ((caractere = fgetc(file)) != '}' && caractere != EOF) {
                    // Ler próximo caractere
                }
            }
            // Ler próximo caractere
            caractere = fgetc(file);
        }

        // Se caractere não for fim de arquivo
        if (caractere != EOF) {
        //Logica de montegem de token - IMPLEMENTAR
            PegaToken(file, caractere);
        }

        // Ler próximo caractere
        caractere = fgetc(file);
    }
}


void AnalisadorLexical(FILE *file) {

    TrataEspacoComentario(file);

}

int main() {
    FILE *file;
    file = fopen("C:/CodigoParaCompilador.txt", "r");

    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    AnalisadorLexical(file);
    //imprime_lista_token();


    fclose(file);
    return 0;
}
