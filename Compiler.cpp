#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>

using namespace std;

int contador = 1;

struct Token {
    string lexema;
    string simbolo;
};

void TrataDigito(FILE *file, char *caractere, Token &token){

    //cria a variavel de controle "num"
    string num = "";
    string simbolo = "snumero";

    //"concatena com num o primeiro digito do numero lido"
    num += *caractere;

    //LER(CARACTERE)
    *caractere = fgetc(file);

    //enquanto caractere for um digito
    while((*caractere > 47) && (*caractere < 58)){
        //concatenar o digito em num e ir para o proximo digito
        num += *caractere;
        *caractere = fgetc(file);
    }

    token.simbolo = simbolo;
    token.lexema = num;

}

void TrataIDePalavraReservada(FILE *file, char *caractere, Token &token){
    //cria a variavel de controle "id"
    string id = "";
    string simbolo;

    //"concatena com num o primeiro digito do numero lido"
    id += *caractere;

    //LER(CARACTERE)
    *caractere = fgetc(file);

    //enquanto caractere for uma letra ou _ ou digito
    while((*caractere > 64 && *caractere < 91) || (*caractere > 96 && *caractere < 123) || (*caractere == 95) || (*caractere > 47 && *caractere < 58)){
        //concatenar o digito em num e ir para o proximo digito
        id += *caractere;
        *caractere = fgetc(file);
    }

    // Loop para converter cada caractere para minuscula
    for (char& c : id) {
        c = tolower(static_cast<unsigned char>(c));
    }
    //salva o lexema lido na lista de tokens
    token.lexema = id;

    if(id == "programa"){
        simbolo = "sprograma";
        token.simbolo = simbolo;
    }
    else if(id == "se"){
        simbolo = "sse";
        token.simbolo = simbolo;
    }
    else if(id == "entao"){
        simbolo = "sentao";
        token.simbolo = simbolo;
    }
    else if(id == "senao"){
        simbolo = "ssenao";
        token.simbolo = simbolo;
    }
    else if(id == "enquanto"){
        simbolo = "senquanto";
        token.simbolo = simbolo;
    }
    else if(id == "faca"){
        simbolo = "sfaca";
        token.simbolo = simbolo;
    }
    else if(id == "inicio"){
        simbolo = "sinicio";
        token.simbolo = simbolo;
    }
    else if(id == "fim"){
        simbolo = "sfim";
        token.simbolo = simbolo;
    }
    else if(id == "escreva"){
        simbolo = "sescreva";
        token.simbolo = simbolo;
    }
    else if(id == "leia"){
        simbolo = "sleia";
        token.simbolo = simbolo;
    }
    else if(id == "var"){
        simbolo = "svar";
        token.simbolo = simbolo;
    }
    else if(id == "inteiro"){
        simbolo = "sinteiro";
        token.simbolo = simbolo;
    }
    else if(id == "booleano"){
        simbolo = "sbooleano";
        token.simbolo = simbolo;
    }
    else if(id == "verdadeiro"){
        simbolo = "sverdadeiro";
        token.simbolo = simbolo;
    }
    else if(id == "falso"){
        simbolo = "sfalso";
        token.simbolo = simbolo;
    }
    else if(id == "procedimento"){
        simbolo = "sprocedimento";
        token.simbolo = simbolo;
    }
    else if(id == "funcao"){
        simbolo = "sfuncao";
        token.simbolo = simbolo;
    }
    else if(id == "div"){
        simbolo = "sdiv";
        token.simbolo = simbolo;
    }
    else if(id == "ou"){
        simbolo = "sou";
        token.simbolo = simbolo;
    }
    else if(id == "nao"){
        simbolo = "snao";
        token.simbolo = simbolo;
    }
    else{
        simbolo = "sidentificador";
        token.simbolo = simbolo;
    }


}

void TrataAtribuicao(FILE *file, char *caractere, Token &token){
    *caractere = fgetc(file);
    string lexema, simbolo;
    if(*caractere == '='){
        lexema = ":=";
        simbolo = "satribuicao";
        token.lexema = lexema;
        token.simbolo = simbolo;
        *caractere = fgetc(file);
    }
    else{
        lexema = ":";
        simbolo = "sdoispontos";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
}

void TrataOPAritmetico(FILE *file, char *caractere, Token &token){

    string lexema, simbolo;
    // 43,45,42
    if(*caractere == 43){
        lexema = "+";
        simbolo = "smais";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else if(*caractere == 45){
        lexema = "-";
        simbolo = "smenos";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else if(*caractere == 42){
        lexema = "*";
        simbolo = "smult";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }

    *caractere = fgetc(file);

}

void TrataOPRelacional(FILE *file, char *caractere, Token &token){

    string lexema, simbolo;
    // 33, 60, 62, 61
    if(*caractere == 33){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = "!=";
            simbolo = "sdif";
            token.lexema = lexema;
            token.simbolo = simbolo;
        }
        *caractere = fgetc(file);
    }
    else if(*caractere == 60){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = "<=";
            simbolo = "smenorig";
            token.lexema = lexema;
            token.simbolo = simbolo;
            *caractere = fgetc(file);
        }
        else {
            lexema = "<";
            simbolo = "smenor";
            token.lexema = lexema;
            token.simbolo = simbolo;
            *caractere = fgetc(file);
        }
    }
    else if(*caractere == 62){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = ">=";
            simbolo = "smaiorig";
            token.lexema = lexema;
            token.simbolo = simbolo;
            *caractere = fgetc(file);
        }
        else {
            lexema = ">";
            simbolo = "smaior";
            token.lexema = lexema;
            token.simbolo = simbolo;
            *caractere = fgetc(file);
        }
    }
    else if(*caractere == 61){
        lexema = "=";
        simbolo = "sig";
        token.lexema = lexema;
        token.simbolo = simbolo;
        *caractere = fgetc(file);
    }

}

void TrataPontuacao(FILE *file, char *caractere, Token &token){

    string lexema, simbolo;
    if(*caractere == 59){
        lexema = ";";
        simbolo = "spontoevirgula";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else if(*caractere == 46){
        lexema = ".";
        simbolo = "sponto";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else if(*caractere == 44){
        lexema = ",";
        simbolo = "svirgula";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else if(*caractere == 41){
        lexema = ")";
        simbolo = "sfechaparenteses";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
    else{
        lexema = "(";
        simbolo = "sabreparenteses";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }

    *caractere = fgetc(file);
}

void TrataErro(FILE *file, char *caractere, Token &token){

    if(*caractere == '}'){
        cout << endl << "ERRO LEXICAL NA LINHA " << contador << ": '"<< *caractere << "' tentativa de fechar comentario sem par de abertura";
    }

    else{
        cout << endl << "ERRO LEXICAL NA LINHA " << contador << ": caractere '"<< *caractere << "' nao reconhecido pela linguagem";
    }

    string lexema, simbolo;
    lexema = *caractere;
    simbolo = "serro";
    token.lexema = lexema;
    token.simbolo = simbolo;

    *caractere = fgetc(file);
}

void PegaToken(FILE *file, char *caractere, Token &token){

    int save_linha_abre_comentario;


    //elimina comentarios e espacos
    while ((*caractere == '{' || *caractere == ' ' || *caractere == '\n') && (*caractere != EOF)) {
        if (*caractere == '{') {
            save_linha_abre_comentario = contador;
            // espera caractere de fechamento de comentario
            while ((*caractere = fgetc(file)) != '}') {
                if(*caractere == EOF){
                    cout << endl << "ERRO LEXICAL NA LINHA " << save_linha_abre_comentario << ": comentario aberto nao foi fechado";
                    break;
                }
            }
        }
        else if(*caractere == '\n'){
            contador++;
        }
        // Ler pr�ximo caractere
        *caractere = fgetc(file);
    }


    //testa se o caractere esta entre '0' e '9'
    if(*caractere > 47 && *caractere < 58)
        TrataDigito(file, caractere, token);

    //testa se o caractere e uma letra entre 'A' e 'Z' ou entre 'a' e 'z'
    else if((*caractere > 64 && *caractere < 91) || (*caractere > 96 && *caractere < 123) || (*caractere == 95))
        TrataIDePalavraReservada(file, caractere, token);

    //testa se o caractere e igual a ':'
    else if(*caractere == 58)
        TrataAtribuicao(file, caractere, token);

    //testa se o caractere pertence a {+,-,*}
    else if((*caractere == 43) || (*caractere == 45) || (*caractere == 42))
        TrataOPAritmetico(file, caractere, token);

    //testa se o caractere pertence a {!, <, >, =}
    else if((*caractere == 33) || (*caractere == 60) || (*caractere == 62) || (*caractere == 61))
        TrataOPRelacional(file, caractere, token);

    //testa se o caractere pertence a {; , ( ) .}
    else if((*caractere == 59) || (*caractere == 44) || (*caractere == 40) || (*caractere == 41) || (*caractere == 46))
        TrataPontuacao(file, caractere, token);
    else if(*caractere == EOF){
        token.lexema = "EOF";
        token.simbolo = "sfimarquivo";
    }
    else
        TrataErro(file, caractere, token);


}

void AnalisadorSintatico(FILE *file) {

    char caractere;
    Token token;

    caractere = fgetc(file);

    do{
        PegaToken(file, &caractere, token);
        cout << endl << "lexema: " << token.lexema << " | Simbolo: " << token.simbolo;
    }while(token.simbolo != "sfimarquivo");

}


void imprime_codigo_com_linhas(){
    FILE *arquivo_para_imprimir;
    arquivo_para_imprimir = fopen("C:/CodigoParaCompilador.txt", "r");
    char caractere_impresso;
    int conta_linhas_impressao = 1;

    cout << endl << endl << endl;

    caractere_impresso = fgetc(arquivo_para_imprimir);
    cout  << conta_linhas_impressao << ' ' << '|';

    while(caractere_impresso != EOF){
        if (caractere_impresso == '\n'){
            conta_linhas_impressao++;
            cout << caractere_impresso;
            caractere_impresso = fgetc(arquivo_para_imprimir);
            cout  << conta_linhas_impressao << ' ' << '|';
        }
        cout << caractere_impresso;
        caractere_impresso = fgetc(arquivo_para_imprimir);
    }
    cout << endl << endl << endl;
    fclose(arquivo_para_imprimir);
}


int main() {

    FILE *file;
    file = fopen("C:/CodigoParaCompilador.txt", "r");

    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    AnalisadorSintatico(file);
    imprime_codigo_com_linhas();


    fclose(file);
    return 0;
}
