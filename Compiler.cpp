#include <stdio.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include <iomanip>  // Para usar setw

using namespace std;

struct ListaDeTokens {
    queue<string> lexema;
    queue<string> simbolo;
};

void imprime_lista_token(ListaDeTokens lista_de_tokens) {
    // Obtendo os tamanhos das filas
    int tamanho_lexema = lista_de_tokens.lexema.size();
    int tamanho_simbolo = lista_de_tokens.simbolo.size();

    // Garantindo que as filas têm o mesmo tamanho
    if (tamanho_lexema != tamanho_simbolo) {
        cerr << "Erro: Tamanho de 'lexema' e 'simbolo' são diferentes!" << endl;
        return;
    }

    cout << "| " << setw(10) << left << "Lexema" << " | " << setw(10) << left << "Simbolo" << " |" << endl;
    cout << "|------------|------------|" << endl;

    while (!lista_de_tokens.lexema.empty() && !lista_de_tokens.simbolo.empty()) {
        string lexema_elemento = lista_de_tokens.lexema.front(); // Pega o primeiro elemento de 'lexema'
        string simbolo_elemento = lista_de_tokens.simbolo.front(); // Pega o primeiro elemento de 'simbolo'

        cout << "| " << setw(10) << left << lexema_elemento << " | " << setw(10) << left << simbolo_elemento << " |" << endl;

        lista_de_tokens.lexema.pop(); // Remove o elemento de 'lexema'
        lista_de_tokens.simbolo.pop(); // Remove o elemento de 'simbolo'
    }
}




void TrataDigito(FILE *file, char caractere, ListaDeTokens &lista_de_tokens){

    //cria a variavel de controle "num"
    string num = "";
    string simbolo = "snumero";

    //"concatena com num o primeiro digito do numero lido"
    num += caractere;

    //LER(CARACTERE)
    caractere = fgetc(file);

    //enquanto caractere for um digito
    while((caractere > 47) && (caractere < 58)){
        //concatenar o digito em num e ir para o proximo digito
        num += caractere;
        caractere = fgetc(file);
    }

    lista_de_tokens.simbolo.push(simbolo);
    lista_de_tokens.lexema.push(num);

}

void TrataIDePalavraReservada(FILE *file, char caractere){

}

void TrataAtribuicao(FILE *file, char caractere){

}

void TrataOPAritmetico(FILE *file, char caractere){

}

void TrataOPRelacional(FILE *file, char caractere){

}

void TrataPontuacao(FILE *file, char caractere){

}

void PegaToken(FILE *file, char caractere, ListaDeTokens &lista_de_tokens){

    //testa se o caractere esta entre '0' e '9'
    if(caractere > 47 && caractere < 58)
        TrataDigito(file, caractere, lista_de_tokens);

    //testa se o caractere e uma letra entre 'A' e 'Z' ou entre 'a' e 'z'
    else if((caractere > 64 && caractere < 91) || (caractere > 96 && caractere < 123))
        TrataIDePalavraReservada(file, caractere);

    //testa se o caractere e igual a ':'
    else if(caractere == 58)
        TrataAtribuicao(file, caractere);

    //testa se o caractere pertence a {+,-,*}
    else if((caractere == 43) || (caractere == 45) || (caractere == 42))
        TrataOPAritmetico(file, caractere);

    //testa se o caractere pertence a {!, <, >, =}
    else if((caractere == 33) || (caractere == 60) || (caractere == 62) || (caractere == 61))
        TrataOPRelacional(file, caractere);

    //testa se o caractere pertence a {; , ( ) .}
    else if((caractere == 59) || (caractere == 44) || (caractere == 40) || (caractere == 41) || (caractere == 46))
        TrataPontuacao(file, caractere);

    else
        cout << ' ' << "|ERRO: CARACTERE INVALIDO|" << ' ';


}

void TrataEspacoComentario(FILE *file, ListaDeTokens &lista_de_tokens){
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
            PegaToken(file, caractere, lista_de_tokens);
        }

        // Ler próximo caractere
        caractere = fgetc(file);
    }
}


void AnalisadorLexical(FILE *file) {

    ListaDeTokens lista_de_tokens;
    TrataEspacoComentario(file, lista_de_tokens);

    imprime_lista_token(lista_de_tokens);

}

int main() {
    FILE *file;
    file = fopen("C:/CodigoParaCompilador.txt", "r");

    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    AnalisadorLexical(file);


    fclose(file);
    return 0;
}
