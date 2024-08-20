#include <stdio.h>
#include <iostream>
#include <queue>

using namespace std;

struct ListaDeTokens {
    queue<char> lexema;
    queue<char> simbolo;
};

void AnalisadorLexical(FILE *file) {
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
            cout << caractere;
        }

        // Ler próximo caractere
        caractere = fgetc(file);
    }

    // Fecha arquivo fonte (fechar arquivo fora da função)
}

int main() {
    FILE *file;
    file = fopen("C:/Faculdade/Compiladores/CodigoParaCompilador.txt", "r");

    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    AnalisadorLexical(file);

    fclose(file);
    return 0;
}
