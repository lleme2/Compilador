#include <stdio.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;

int contador = 1;

struct ListaDeTokens {
    queue<string> lexema;
    queue<string> simbolo;
};

void imprime_lista_token(ListaDeTokens lista_de_tokens) {

    cout << endl << endl << endl;

    // Obtendo os tamanhos das filas
    int tamanho_lexema = lista_de_tokens.lexema.size();
    int tamanho_simbolo = lista_de_tokens.simbolo.size();

    // Garantindo que as filas t�m o mesmo tamanho
    if (tamanho_lexema != tamanho_simbolo) {
        cerr << "Erro: Tamanho de 'lexema' e 'simbolo' s�o diferentes!" << endl;
        return;
    }

    cout << "| " << setw(20) << left << "Lexema" << " | " << setw(20) << left << "Simbolo" << " |" << endl;
    cout << "|----------------------|----------------------|" << endl;

    while (!lista_de_tokens.lexema.empty() && !lista_de_tokens.simbolo.empty()) {
        string lexema_elemento = lista_de_tokens.lexema.front(); // Pega o primeiro elemento de 'lexema'
        string simbolo_elemento = lista_de_tokens.simbolo.front(); // Pega o primeiro elemento de 'simbolo'

        cout << "| " << setw(20) << left << lexema_elemento << " | " << setw(20) << left << simbolo_elemento << " |" << endl;

        lista_de_tokens.lexema.pop(); // Remove o elemento de 'lexema'
        lista_de_tokens.simbolo.pop(); // Remove o elemento de 'simbolo'
    }
    cout << "|----------------------|----------------------|" << endl;
}




void TrataDigito(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

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

    lista_de_tokens.simbolo.push(simbolo);
    lista_de_tokens.lexema.push(num);

}

void TrataIDePalavraReservada(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){
    //cria a variavel de controle "id"
    string id = "";
    string simbolo;

    //"concatena com num o primeiro digito do numero lido"
    id += *caractere;

    //LER(CARACTERE)
    *caractere = fgetc(file);

    //enquanto caractere for uma letra ou _
    while((*caractere > 64 && *caractere < 91) || (*caractere > 96 && *caractere < 123) || (*caractere == 95)){
        //concatenar o digito em num e ir para o proximo digito
        id += *caractere;
        *caractere = fgetc(file);
    }

    // Loop para converter cada caractere para minuscula
    for (char& c : id) {
        c = tolower(static_cast<unsigned char>(c));
    }
    //salva o lexema lido na lista de tokens
    lista_de_tokens.lexema.push(id);

    if(id == "programa"){
        simbolo = "sprograma";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "se"){
        simbolo = "sse";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "entao"){
        simbolo = "sentao";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "senao"){
        simbolo = "ssenao";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "enquanto"){
        simbolo = "senquanto";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "faca"){
        simbolo = "sfaca";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "inicio"){
        simbolo = "sinicio";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "fim"){
        simbolo = "sfim";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "escreva"){
        simbolo = "sescreva";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "leia"){
        simbolo = "sleia";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "var"){
        simbolo = "svar";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "inteiro"){
        simbolo = "sinteiro";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "booleano"){
        simbolo = "sbooleano";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "verdadeiro"){
        simbolo = "sverdadeiro";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "falso"){
        simbolo = "sfalso";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "procedimento"){
        simbolo = "sprocedimento";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "funcao"){
        simbolo = "sfuncao";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "div"){
        simbolo = "sdiv";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "ou"){
        simbolo = "sou";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(id == "nao"){
        simbolo = "snao";
        lista_de_tokens.simbolo.push(simbolo);
    }
    else{
        simbolo = "sidentificador";
        lista_de_tokens.simbolo.push(simbolo);
    }


}

void TrataAtribuicao(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){
    *caractere = fgetc(file);
    string lexema, simbolo;
    if(*caractere == '='){
        lexema = ":=";
        simbolo = "satribuicao";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
        *caractere = fgetc(file);
    }
    else{
        lexema = ":";
        simbolo = "sdoispontos";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
}

void TrataOPAritmetico(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    string lexema, simbolo;
    // 43,45,42
    if(*caractere == 43){
        lexema = "+";
        simbolo = "smais";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(*caractere == 45){
        lexema = "-";
        simbolo = "smenos";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(*caractere == 42){
        lexema = "*";
        simbolo = "smult";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }

    *caractere = fgetc(file);

}

void TrataOPRelacional(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    string lexema, simbolo;
    // 33, 60, 62, 61
    if(*caractere == 33){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = "!=";
            simbolo = "sdif";
            lista_de_tokens.lexema.push(lexema);
            lista_de_tokens.simbolo.push(simbolo);
        }
        *caractere = fgetc(file);
    }
    else if(*caractere == 60){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = "<=";
            simbolo = "smenorig";
            lista_de_tokens.lexema.push(lexema);
            lista_de_tokens.simbolo.push(simbolo);
            *caractere = fgetc(file);
        }
        else {
            lexema = "<";
            simbolo = "smenor";
            lista_de_tokens.lexema.push(lexema);
            lista_de_tokens.simbolo.push(simbolo);
            *caractere = fgetc(file);
        }
    }
    else if(*caractere == 62){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = ">=";
            simbolo = "smaiorig";
            lista_de_tokens.lexema.push(lexema);
            lista_de_tokens.simbolo.push(simbolo);
            *caractere = fgetc(file);
        }
        else {
            lexema = ">";
            simbolo = "smaior";
            lista_de_tokens.lexema.push(lexema);
            lista_de_tokens.simbolo.push(simbolo);
            *caractere = fgetc(file);
        }
    }
    else if(*caractere == 61){
        lexema = "=";
        simbolo = "sig";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
        *caractere = fgetc(file);
    }

}

void TrataPontuacao(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    string lexema, simbolo;
    if(*caractere == 59){
        lexema = ";";
        simbolo = "spontoevirgula";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(*caractere == 46){
        lexema = ".";
        simbolo = "sponto";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(*caractere == 44){
        lexema = ",";
        simbolo = "svirgula";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else if(*caractere == 41){
        lexema = ")";
        simbolo = "sfechaparenteses";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }
    else{
        lexema = "=";
        simbolo = "sabreparenteses";
        lista_de_tokens.lexema.push(lexema);
        lista_de_tokens.simbolo.push(simbolo);
    }

    *caractere = fgetc(file);
}

void TrataErro(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    if(*caractere == '}'){
        cout << endl << "ERRO LEXICAL NA LINHA " << contador << ": uso de simbolo de fechamento de comentario sem um simbolo de abertura de comentario '}'";
    }
    else{
        cout << endl << "ERRO LEXICAL NA LINHA " << contador << ": caractere nao reconhecido pela linguagem";
    }

    string lexema, simbolo;
    lexema = *caractere;
    simbolo = "serro";
    lista_de_tokens.lexema.push(lexema);
    lista_de_tokens.simbolo.push(simbolo);

    *caractere = fgetc(file);
}

void PegaToken(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    //testa se o caractere esta entre '0' e '9'
    if(*caractere > 47 && *caractere < 58)
        TrataDigito(file, caractere, lista_de_tokens);

    //testa se o caractere e uma letra entre 'A' e 'Z' ou entre 'a' e 'z'
    else if((*caractere > 64 && *caractere < 91) || (*caractere > 96 && *caractere < 123) || (*caractere == 95))
        TrataIDePalavraReservada(file, caractere, lista_de_tokens);

    //testa se o caractere e igual a ':'
    else if(*caractere == 58)
        TrataAtribuicao(file, caractere, lista_de_tokens);

    //testa se o caractere pertence a {+,-,*}
    else if((*caractere == 43) || (*caractere == 45) || (*caractere == 42))
        TrataOPAritmetico(file, caractere, lista_de_tokens);

    //testa se o caractere pertence a {!, <, >, =}
    else if((*caractere == 33) || (*caractere == 60) || (*caractere == 62) || (*caractere == 61))
        TrataOPRelacional(file, caractere, lista_de_tokens);

    //testa se o caractere pertence a {; , ( ) .}
    else if((*caractere == 59) || (*caractere == 44) || (*caractere == 40) || (*caractere == 41) || (*caractere == 46))
        TrataPontuacao(file, caractere, lista_de_tokens);

    else
        TrataErro(file, caractere, lista_de_tokens);


}

void TrataEspacoComentario(FILE *file, char *caractere, ListaDeTokens &lista_de_tokens){

    // Enquanto nao acabou o arquivo fonte
    while (*caractere != EOF) {
        //elimina comentarios e espacos
        while ((*caractere == '{' || *caractere == ' ' || *caractere == '\n') && (*caractere != EOF)) {
            if (*caractere == '{') {
                // espera caractere de fechamento de comentario
                while ((*caractere = fgetc(file)) != '}' && *caractere != EOF) {
                    // Ler pr�ximo caractere
                }
            }
            else if(*caractere == '\n'){
                contador++;
            }
            // Ler pr�ximo caractere
            *caractere = fgetc(file);
        }

        // Se caractere n�o for fim de arquivo
        if (*caractere != EOF) {
        //Logica de montegem de token - IMPLEMENTAR
            PegaToken(file, caractere, lista_de_tokens);
        }

    }
}


void AnalisadorLexical(FILE *file) {

    char caractere;
    ListaDeTokens lista_de_tokens;

    caractere = fgetc(file);
    TrataEspacoComentario(file, &caractere, lista_de_tokens);

    imprime_lista_token(lista_de_tokens);

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
}

int main() {
    FILE *file;
    file = fopen("C:/CodigoParaCompilador.txt", "r");

    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    AnalisadorLexical(file);
    imprime_codigo_com_linhas();


    fclose(file);
    return 0;
}
