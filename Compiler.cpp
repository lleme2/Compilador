/*#define PATH_COMPILER "C:/-----PUCCAMPINAS----/8 SEMESTRE/Compiladores/Compilador/Compiler.cpp"
#define PATH_LOG "C:/-----PUCCAMPINAS----/8 SEMESTRE/Compiladores/Compilador/log.txt"
#define PATH_TXT "C:/CodigoParaCompilador.txt"
#define PATH_GERA "C:\\-----PUCCAMPINAS----\\8 SEMESTRE\\Compiladores\\Compilador\\gera.txt"*/

#define PATH_COMPILER "d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler.cpp"
#define PATH_OUTPUT "d:/Users/Home/Documents/GitHub/demo/Compilador/Compiler"
#define PATH_LOG "d:/Users/Home/Documents/GitHub/demo/Compilador/log.txt"
#define PATH_TXT "D:/Users/Home/Desktop/CodigoCompilador.txt"
#define PATH_GERA "d:/Users/Home/Documents/GitHub/demo/Compilador/gera.txt"

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <queue>
#include <fstream>
#include <list>
#include <vector>
#include <deque>
#include <typeinfo>
#include <stack>
#include <cctype>

using namespace std;
vector<string> saida_pos_fixa;
//string saida_pos_fixa.clear();
deque<string> pilha_pos_fixa;
string tipo_posfixa = "";

int rotulo = 1;
int s = 1; // pensar sobre comando START

string nivel;
int contador = 1;
bool primeiro_token = true;

struct Simbolos {
    string lexema;
    string tipo;
    string nivel;
    int endereco;
};


queue<string> erros;
vector<Simbolos> tabela;
string msg_erro;

struct Token {
    string lexema;
    string simbolo;
};

void Gera_Expressao();
string Pesquisa_tipo(string lexema);
void Coloca_tipo(string tipo);
void ImprimirTabela(const vector<Simbolos>& tabela);
bool Pesquisa_declaracao_variavel(string lexema);
bool Pesquisa_declaracao_func(string lexema);
void Desempilhar();
bool Pesquisa_declaracao_proc(string lexema);
void Insere_tabela(string lexema, string tipo);
bool Pesquisa_Variavel_Duplicada(string lexema);
void imprime_codigo_com_linhas();
void imprime_erros();
void Analisa_comando_simples(FILE *file, char *caractere, Token &token);
void Analisa_expressao(FILE *file, char *caractere, Token &token);
void AnalisaBloco(FILE *file, char *caractere, Token &token);
void Analisa_comandos(FILE *file, char *caractere, Token &token);
string TipoPosFixa();
string verificarTipoExpressao();


void printVector() {
    for(int i=0;i<saida_pos_fixa.size();i++){
        cout << saida_pos_fixa[i]<< " ";
    }
    cout << endl;
}

template <typename T>
void ImprimeDeque(const std::deque<T>& dq) {
    std::cout << "Conteúdo do deque (reverso): [";
    for (size_t i = dq.size(); i > 0; --i) {
        std::cout << dq[i - 1];
        if (i > 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void ImprimirTabela() {
    cout << "Tabela de Símbolos:\n";
    cout << "------------------------\n";
    cout << "Lexema\tTipo\tNível\tEndereco\n";
    cout << "------------------------\n";
    
    for (const auto& simbolo : tabela) {
        cout << simbolo.lexema << "\t" << simbolo.tipo << "\t" << simbolo.nivel << "\t" << simbolo.endereco << "\n";
    }
    
    cout << "------------------------\n";
}

void Coloca_tipo(string tipo_var){
    
    int iterador = tabela.size() - 1;\
    while (tabela[iterador].tipo == "variavel"){
        tabela[iterador].tipo = "variavel " + tipo_var;
        iterador--;
    }
}

int precedencia(string op) {
    if (op == "positivo ou negativo") {
        return 7;
    } else if (op == "*" || op == "div") {
        return 6;
    } else if (op == "+" || op == "-") {
        return 5;
    }
    else if(op == ">" || op == ">=" || op == "=" || op == "<" || op == "<=" || op == "!="){
        return 4;
    }
    else if (op == "nao") {
        return 3;
    } else if (op == "e") {
        return 2;
    }
    else if (op == "ou") {
        return 1;
    }
    else{
        return -1;
    }
}


void Desempilhar(){
    cout << "Estou na funcao desempilhar" << endl;
    cout << "\n\n\n";
    ImprimirTabela(); 
    //cout << "Estou na funcao desempilhar" << endl;
    int iterador = tabela.size() - 1;
    //cout << "Ultimo item tabela: " + tabela.back().lexema << endl;
    while (tabela[iterador].nivel == "" )
    {
        tabela.pop_back();
        iterador--;
        //cout << "Ultimo item tabela: " + tabela.back().lexema << endl;      
        //ImprimirTabela(); 
    }
    //cout << "Ta saindo do while" << endl;
    tabela[iterador].nivel = "";
    ImprimirTabela(); 
    //cout << "Ultimo item tabela: " + tabela.back().tipo << endl;
}

void Desempilha_posfixa(){
    while(!pilha_pos_fixa.empty()){
        string controle_pilha_pos_fixa = "";
        controle_pilha_pos_fixa = pilha_pos_fixa.front();
        pilha_pos_fixa.pop_front();
        saida_pos_fixa.push_back(controle_pilha_pos_fixa);
    }
    //cout << saida_pos_fixa << endl;.push_back
    cout << "Tipo da expressao: " << TipoPosFixa() << endl;
    cout << "Expressao pos fixa: " << endl;
    for(int i=0;i<saida_pos_fixa.size();i++){
        cout << saida_pos_fixa[i]<< " ";
    }
    cout << endl;
    Gera_Expressao();
    saida_pos_fixa.clear();
}

bool IsDigit(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Função para verificar se um elemento é uma letra (variável)
bool IsLetter(const string& s) {
    for (char c : s) {
        if (!isalpha(c)) return false;
    }
    return true;
}

string TipoPosFixa() {
    stack<string> tipos; // Pilha para armazenar os tipos ("int" ou "bool")
    
    for (const string& c : saida_pos_fixa) {
        if (IsDigit(c) && c != "div" && c != "e" && c != "ou" && c != "nao" && c != "verdadeiro" && c != "falso") {
            //cout << "Digito expressao: " << c << endl;
            // Digito é tratado como inteiro
            tipos.push("int");
        } else if (c == "verdadeiro" || c == "falso") {
            //cout << "Booleano expressao: " << c << endl;
            // Literais booleanos
            tipos.push("bool");
        } else if (IsLetter(c) && c != "div" && c != "e" && c != "ou" && c != "nao" && c != "verdadeiro" && c != "falso") {
            //cout << "Variavel expressao: " << c << endl;
            // Variável (precisa ser inferida, assumimos "int" como exemplo)
            if(Pesquisa_tipo(c) == "variavel sbooleano"){
                tipos.push("bool");
            }
            else if(Pesquisa_tipo(c) == "variavel sinteiro"){
                tipos.push("int");
            }
            else{
                return "Erro: Deu errado no tipo";
            }
             // Você pode adicionar lógica para variáveis booleanas
        } else {
            // Tratar operadores
            if (c == "+" || c == "-" || c == "*" || c == "div") {
                // Operadores aritméticos exigem dois operandos inteiros
                if (tipos.size() < 2 || tipos.top() != "int") return "Erro de tipo1 ";
                tipos.pop();
                if (tipos.top() != "int") return "Erro de tipo 2";
                tipos.pop();
                tipos.push("int");
            } else if (c == ">" || c == "<" || c == ">=" || c == "<=" || c == "!=" || c == "=") {
                // Operadores relacionais exigem dois operandos inteiros e retornam booleanos
                if (tipos.size() < 2 || tipos.top() != "int") return "Erro de tipo 3";
                tipos.pop();
                if (tipos.top() != "int") return "Erro de tipo 4";
                tipos.pop();
                tipos.push("bool");
            } else if (c == "e" || c == "ou") {
                // Operadores lógicos exigem dois operandos booleanos e retornam booleanos
                if (tipos.size() < 2 || tipos.top() != "bool") return "Erro de tipo 5";
                tipos.pop();
                if (tipos.top() != "bool") return "Erro de tipo 6";
                tipos.pop();
                tipos.push("bool");
            } else if (c == "nao") {
                // Operador lógico unário exige um operando booleano e retorna booleano
                if (tipos.empty() || tipos.top() != "bool") return "Erro de tipo 7";
                tipos.pop();
                tipos.push("bool");
            } else {
                // Caso de operador desconhecido
                return "Erro: operador desconhecido";
            }
        }
    }
    
    // Após processar toda a expressão, a pilha deve conter exatamente um tipo
    if (tipos.size() == 1) {
        return tipos.top();
    } else {
        return "Erro de tipo 8";
    }
}

void Insere_tabela(string lexema, string tipo, string nivel, int endereco){
    Simbolos simboloPrograma;
    simboloPrograma.tipo = tipo;
    simboloPrograma.lexema = lexema;
    simboloPrograma.nivel = nivel;
    simboloPrograma.endereco = endereco;
    tabela.push_back(simboloPrograma);
    return;
}

string Pesquisa_tipo(string lexema){
    int controlador = tabela.size()-1;
    while(tabela[controlador].tipo != "nomedoprograma"){
        if(tabela[controlador].lexema == lexema){
            return tabela[controlador].tipo;
        }
        controlador--;
    }
    return "NaN";
}

int Pesquisa_endereco(string lexema){
    int controlador = tabela.size()-1;
    while(tabela[controlador].tipo != "nomedoprograma"){
        if(tabela[controlador].lexema == lexema){
            return tabela[controlador].endereco;
        }
        controlador--;
    }
    return -1;
}

bool Pesquisa_declaracao_func(string lexema){
    //ImprimirTabela();
    //TODO: procedimento tem q ir ate o final da pilha? Pode ter procedimento e funcao com o msm nome?
    for(int iterador=tabela.size()-1 ; iterador>0;iterador--){
        //cout << "Declaracao funcao tipo: " + tabela[iterador].tipo << endl;
        //cout << "Declaracao funcao lexema: " + tabela[iterador].lexema << endl;
        if(tabela[iterador].lexema == lexema){
            if(tabela[iterador].tipo == "funcao booleana" || tabela[iterador].tipo == "funcao inteiro" || tabela[iterador].tipo == "procedimento"){
                return true;
            }
            else{
                continue;
            }     
        }
    }
    return false; 
}

bool Pesquisa_declaracao_proc(string lexema){
    //TODO: procedimento tem q ir ate o final da pilha? Pode ter procedimento e funcao com o msm nome?
    for(int iterador=tabela.size()-1 ; iterador>0;iterador--){
        if(tabela[iterador].lexema == lexema && tabela[iterador].tipo == "procedimento"){
            return true;
        }
    }
    return false; 
}

bool Pesquisa_Variavel_Duplicada(string lexema){
    int iterador = tabela.size() - 1;
    cout << "Iterador: "<< iterador << endl;
    while(tabela[iterador].nivel != "X" && iterador > 0){
        cout << "Pesquisa var duplicada: " + tabela[iterador].lexema << endl;
        if(tabela[iterador].lexema == lexema){
            if(tabela[iterador].tipo == "variavel sinteiro" || tabela[iterador].tipo == "variavel sbooleano"){
                return true;
            }
        }
        iterador--;
    }
    return false;
}

bool Pesquisa_declaracao_variavel(string lexema){
    //cout << "Lexema a ser verificado no escreva: " + lexema << endl;
    //ImprimirTabela();
    int controlador = tabela.size()-1;
    if(controlador > 0){
        while(tabela[controlador].tipo != "nomedoprograma"){
            //cout << "Lexema ESCREVA: " + tabela[controlador].lexema << endl;
            //cout << "Tipo ESCREVA: " + tabela[controlador].tipo << endl;
            if(tabela[controlador].lexema == lexema){
                if(tabela[controlador].tipo == "variavel sinteiro" || tabela[controlador].tipo == "variavel sbooleano"){
                return true;
                }
                else{
                    continue;
                }
            }
            controlador--;
        }
    }
    return false;
}

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

    //"concatena com id o primeiro caractere lido"
    id += *caractere;

    //LER(CARACTERE)
    *caractere = fgetc(file);

    //enquanto caractere for uma letra ou _ ou digito
    while((*caractere > 64 && *caractere < 91) || (*caractere > 96 && *caractere < 123) || (*caractere == 95) || (*caractere > 47 && *caractere < 58)){
        //concatenar o caractere em id e ir para o proximo caractere
        id += *caractere;
        *caractere = fgetc(file);
    }

    // Loop para converter cada caractere para minuscula
    for (char& c : id) {
        c = tolower(static_cast<unsigned char>(c));
    }
    //salva o lexema lido na lista de tokens
    token.lexema = id;

    //testa oq aquela palavra criada representa e salva o simbolo
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
    else if(id == "e"){
        simbolo = "se";
        token.simbolo = simbolo;
    }
    else{
        simbolo = "sidentificador";
        token.simbolo = simbolo;
    }


}

void TrataAtribuicao(FILE *file, char *caractere, Token &token){

    //tendo em vista que ja foi lido o ":", vemos qual eh o proximo caractere
    *caractere = fgetc(file);

    string lexema, simbolo;
    //se for um igual, trata-se de uma atribuicao ":=", portanto salva-se isso como token
    if(*caractere == '='){
        lexema = ":=";
        simbolo = "satribuicao";
        token.lexema = lexema;
        token.simbolo = simbolo;
        *caractere = fgetc(file);
    }
    //caso contrario, eh apenas um ":", portanto salva-se como token dois pontos
    else{
        lexema = ":";
        simbolo = "sdoispontos";
        token.lexema = lexema;
        token.simbolo = simbolo;
    }
}

void TrataOPAritmetico(FILE *file, char *caractere, Token &token){

    string lexema, simbolo;

    //Testa para ver se o caractere lido eh um "+", "-" ou "*" e salvando seus respectivos tokens
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

    //se o caractere lido for um "!", testa para ver se o proximo eh um "=" para formar o simbolo de diferente
    if(*caractere == 33){
        if ((*caractere = fgetc(file)) == 61) {
            lexema = "!=";
            simbolo = "sdif";
            token.lexema = lexema;
            token.simbolo = simbolo;
        }
        *caractere = fgetc(file);
    }

    //se o caractere lido for um "<", pega o proximo caractere para ver se eh um "<" ou um "<="
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
            //*caractere = fgetc(file);
        }
    }

    //se o caractere lido for um ">", pega o proximo caractere para ver se eh um ">" ou um ">="
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
            //*caractere = fgetc(file);
        }
    }

    //se o caractere lido for um "=", cria seu token
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

    //testa para ver se eh um ";", ".", ",", ")" ou "(" e cria seu respectivo token
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

    //se o caractere nao reconhecido pela linguagem for um fechamento de comentario, quer dizer que ele nao tem par de abertura, logo, eh um erro que vai para a fila
    if(*caractere == '}'){
        msg_erro = "";
        string msg_erro = "ERRO LEXICAL NA LINHA " + to_string(contador) + ": '" + string(1, *caractere) + "' tentativa de fechar comentario sem par de abertura";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }

    // se o erro for qualquer outro caractere, ele manda como erro para a fila de erros
    else{
        msg_erro = "";
        msg_erro = "ERRO LEXICAL NA LINHA " + to_string(contador) + ": caractere '" + string(1, *caractere) + "' nao reconhecido pela linguagem";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }

    //salva como token o caractere de erro juntamente com o simbolo "serro"
    string lexema, simbolo;
    lexema = *caractere;
    simbolo = "serro";
    token.lexema = lexema;
    token.simbolo = simbolo;

    *caractere = fgetc(file);
}

void PegaToken(FILE *file, char *caractere, Token &token){

    //cria variavel de controle para saber em qual linha um comentario foi aberto
    int save_linha_abre_comentario;

    //se for a primeira vez que a funcao pega token eh chamada, ele pega o primeiro token, caso contrario, o proximo token ja estara em *caractere
    if(primeiro_token){
        *caractere = fgetc(file);
        primeiro_token = false;
    }

    //elimina comentarios, espacos e pula linhas
    while ((*caractere == '{' || *caractere == ' ' || *caractere == '\n') && (*caractere != EOF)) {
        if (*caractere == '{') {
            //salva linha em que se abriu um comentario
            save_linha_abre_comentario = contador;
            // espera caractere de fechamento de comentario
            while ((*caractere = fgetc(file)) != '}') {
                //se um comentario aberto nao foi fechado, coloca na fila erro juntamente com a linha salva de abertura de comentario
                if(*caractere == EOF){
                    msg_erro = "";
                    msg_erro = "ERRO LEXICAL NA LINHA " + to_string(save_linha_abre_comentario) + ": comentario aberto nao foi fechado";
                    erros.push(msg_erro);
                    imprime_codigo_com_linhas();
                    imprime_erros();
                    exit(1);
                    break;
                }
            }
        }
        //se o codigo pula uma linha, a variavel global "contador" que conta as linhas eh incrementada
        else if(*caractere == '\n'){
            contador++;
        }
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
    //testa se o caractere eh o final do arquivo
    else if(*caractere == EOF){
        token.lexema = "EOF";
        token.simbolo = "sfimarquivo";
    }
    //se o caractere nao esta entre as opcoes anteriores, se trata de um caractere nao reconhecido pela linguagem, logo, vai para o trata erro
    else
        TrataErro(file, caractere, token);


}

void GERA(string rotulo_escrita, string instrucao, int parametro1, int parametro2){

    cout << "ENTROU NO GERA COM A INSTRUCAO: " << instrucao << endl;

    ofstream arquivo_geracao(PATH_GERA, ios::app); // Modo append
    if (!arquivo_geracao.is_open()) {
        cerr << "Não foi possível abrir o arquivo!" << endl;
        return;
    }
    // alloc 1,2

    if(rotulo_escrita != ""){
        arquivo_geracao << rotulo_escrita << " ";
    }
    if(instrucao != ""){
        arquivo_geracao << instrucao << " ";
    }
    if(parametro1 != NULL){
        arquivo_geracao << parametro1 << " ";
    }
    if(parametro2 != NULL){
        arquivo_geracao << ", " << parametro2;
    }

    arquivo_geracao << endl;

    arquivo_geracao.close();

}

void Gera_Expressao(){
    cout << endl << endl << endl << endl << "CHEGUEI NO GERA EXPRESSAO";
    string controle;
        for (int i = 0; i < saida_pos_fixa.size(); i++)
        {
            controle = saida_pos_fixa[i];
            saida_pos_fixa.pop_back();
            cout << controle << " ";
            if (controle == ">")
            {
                GERA("", "CMA", NULL, NULL);
            }
            else if (controle == ">=")
            {
                GERA("", "CMAQ", NULL, NULL);
            }
            else if (controle == "=")
            {
                GERA("", "CEQ", NULL, NULL);
            }
            else if (controle == "<")
            {
                GERA("", "CME", NULL, NULL);
            }
            else if (controle == "<=")
            {
                GERA("", "CMEQ", NULL, NULL);
            }
            else if (controle == "!=")
            {
                GERA("", "CDIF", NULL, NULL);
            }
            else if (controle == "+")
            {
                GERA("", "ADD", NULL, NULL);
            }
            else if (controle == "-")
            {
                GERA("", "SUB", NULL, NULL);
            }
            else if (controle == "*")
            {
                GERA("", "MULT", NULL, NULL);
            }
            else if (controle == "div")
            {
                GERA("", "DIVI", NULL, NULL);
            }
            else if (controle == "e")
            {
                GERA("", "AND", NULL, NULL);
            }
            else if (controle == "ou")
            {
                GERA("", "OR", NULL, NULL);
            }
            else if (controle == "nao")
            {
                GERA("", "NEG", NULL, NULL);
            }
            else
            {
                if (IsDigit(controle))
                {
                    GERA("", "LDC", stoi(controle), NULL);
                }
                else if (IsLetter(controle))
                {
                    GERA("", "LDV", Pesquisa_endereco(controle), NULL);
                    /*Na hora de mostrar na UI, n é pra aparecer o endereco da variavel e sim a variavel
                    errado: LDV 5 --> o codigo gerado
                    certo: LDV x --> é o q tem q ser mostrado no UI
                    */
                }
            }
        }
    cout << endl << endl << endl << endl;
}

void Analisa_tipo(FILE *file, char *caractere, Token &token){
    if(token.simbolo != "sinteiro" && token.simbolo != "sbooleano"){
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": tipo de variavel invalido";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
    else{
        //TODO: insere tipo na tabela
        Coloca_tipo(token.simbolo);
        PegaToken(file, caractere, token);
    }
}

void Analisa_variaveis(FILE *file, char *caractere, Token &token){
    int qtd_variaveis = 0;
    int start_point = s;
    do{
        if(!Pesquisa_Variavel_Duplicada(token.lexema)){
            Insere_tabela(token.lexema, "variavel","", s);
            qtd_variaveis++;
            s++;
            PegaToken(file, caractere, token);
            if(token.simbolo == "svirgula" || token.simbolo == "sdoispontos"){
                if(token.simbolo == "svirgula"){
                    PegaToken(file, caractere, token);
                    if(token.simbolo == "sdoispontos"){
                        msg_erro = "";
                        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": esperando identificador de variavel apos ','";
                        erros.push(msg_erro);
                        imprime_codigo_com_linhas();
                        imprime_erros();
                        exit(1);
                        return;
                    }
                }
            }
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": esperando ',' ou ':' apos declaracao de variavel";
                erros.push(msg_erro);
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
                return;
            }
        }
        else{
            cout << "ERRO: Variavel duplicada!" << endl;
            //PegaToken(file, caractere, token); Se n tiver isso tem loop ininito, falta tratar o erro decentemente
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }
    }while(token.simbolo != "sdoispontos");
    GERA("", "ALLOC", start_point, qtd_variaveis);
    PegaToken(file, caractere, token);
    Analisa_tipo(file, caractere, token);
}

void Analisa_et_variaveis(FILE *file, char *caractere, Token &token){
    if(token.simbolo == "svar"){
        PegaToken(file, caractere, token);
        while(token.simbolo == "sidentificador"){
            cout << token.simbolo << endl;
            if(token.simbolo == "sidentificador"){
                    Analisa_variaveis(file, caractere, token);
                    if(token.simbolo == "spontoevirgula"){
                        PegaToken(file, caractere, token);
                    }
                    else{
                        msg_erro = "";
                        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": Faltou ';' apos declaracao de variavel";
                        erros.push(msg_erro);
                        imprime_codigo_com_linhas();
                        imprime_erros();
                        exit(1);
                        return;

                    }
            }
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": a variavel declarada nao tem identificador correspondente";
                erros.push(msg_erro);
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
                return;
            }
        }
    }
}

void Analisa_declaracao_procedimento(FILE *file, char *caractere, Token &token){
    PegaToken(file, caractere, token);
    string nivel = "X"; // colocar X ao inves de utlima posicao
    if(token.simbolo == "sidentificador"){
        //marquinha
        if(!Pesquisa_declaracao_proc(token.lexema)){
            cout << "Procedimento: " + token.lexema << endl;
            Insere_tabela(token.lexema,"procedimento",nivel,rotulo);
            rotulo++;
            PegaToken(file, caractere, token);
            if(token.simbolo == "spontoevirgula"){
                PegaToken(file, caractere, token);
                AnalisaBloco(file, caractere, token);
            }
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ';' na declaracao do procedimento";
                erros.push(msg_erro);     
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);            
            }
        }
        else{
            cout << "\nErro: na declaracao de procedimento";
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }   
    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou identificador na declaracao do procedimento";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);       
    }
    // funcao desempilha
    Desempilhar();
}

void Analisa_declaracao_funcao(FILE *file, char *caractere, Token &token){
    PegaToken(file, caractere, token);
    string nivel = "X";
    if(token.simbolo == "sidentificador"){
        // pesquisa declara func
        if(!Pesquisa_declaracao_func(token.lexema)){
            cout << "\nAnalisa Declaracao Func: Passou pela procura\n";
            Insere_tabela(token.lexema,"",nivel,rotulo);
            rotulo++;
            PegaToken(file, caractere, token);
            if(token.simbolo == "sdoispontos"){
                PegaToken(file, caractere, token);
                if(token.simbolo == "sinteiro" || token.simbolo == "sbooleano"){
                    if(token.simbolo == "sinteiro"){
                        tabela[tabela.size() - 1].tipo = "funcao inteiro";
                    }
                    else{
                        tabela[tabela.size() - 1].tipo = "funcao booleana";
                    }
                    PegaToken(file, caractere, token);
                    if(token.simbolo == "spontoevirgula"){
                        cout << "\nChamada analisa bloco declaracao de funcao";
                        PegaToken(file, caractere, token);
                        AnalisaBloco(file, caractere, token);
                        cout << "Volta do analisa bloco do analisa declaracao de func: " + token.simbolo << endl;
                    }
                    else{
                        msg_erro = "";
                        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ';' na declaracao da funcao";
                        erros.push(msg_erro);
                        imprime_codigo_com_linhas();
                        imprime_erros();
                        exit(1); 
                    }
                }
                else{
                    msg_erro = "";
                    msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": parametro da funcao de tipo invalido";
                    erros.push(msg_erro);
                    imprime_codigo_com_linhas();
                    imprime_erros();
                    exit(1);                  
                }
            }
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ':' apos identificador de funcao";
                erros.push(msg_erro);
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);             
            }
            }
            else{
                cout << "Erro: Essa funcao ja existe!" << endl;
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou identificador na declaracao da funcao";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1); 
    }
    Desempilhar();
    cout << "Volta da funcao desempilhar do analisa declara func" << endl;
}

void Analisa_subrotinas(FILE *file, char *caractere, Token &token){
    while(token.simbolo == "sprocedimento" || token.simbolo == "sfuncao"){
        if(token.simbolo == "sprocedimento"){
            Analisa_declaracao_procedimento(file, caractere, token);
        }
        else{
            cout << "\n Chegou no analisa declaracao funcao";
            Analisa_declaracao_funcao(file, caractere, token);
        }
        cout << "if ; do analisa subrotinas: " + token.simbolo << endl;
        if(token.simbolo == "spontoevirgula"){
            PegaToken(file, caractere, token);
            cout << " Pos if ; do analisa subrotinas: " + token.simbolo << endl;
        }
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ';'";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);            
        }
    }
}

//void Analisa_chamada_funcao(FILE *file, char *caractere, Token &token){}

void Chamada_procedimento(FILE *file, char *caractere, Token &token){
    if(!Pesquisa_declaracao_proc(token.lexema)){
        cout << "ERRO: Procedimento não declarado!";
    }
}

void Analisa_fator(FILE *file, char *caractere, Token &token){
    if(token.simbolo == "sidentificador"){
        if(Pesquisa_declaracao_func(token.lexema) || Pesquisa_declaracao_variavel(token.lexema)){
            saida_pos_fixa.push_back(token.lexema);
            //cout << saida_pos_fixa << endl;.push_back
            ImprimeDeque(pilha_pos_fixa);
//cout << saida_pos_fixa << endl;.push_back
            PegaToken(file, caractere, token);
            cout << "Analisa fator indentificador: " + token.simbolo << endl;
        }
        else{
            cout << "Erro: Uso de variavel ou funcao nao declarada na expressao" << endl;
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }
        //Analisa_chamada_funcao(file, caractere, token);
    }
    else if(token.simbolo == "snumero"){
        saida_pos_fixa.push_back(token.lexema);
        cout << "Numero do fator: " << token.lexema << "\n";
        PegaToken(file, caractere, token);
        cout << "Numero do fator 2: " << token.simbolo << "\n";
    }
    else if (token.simbolo == "snao"){
        if(!pilha_pos_fixa.empty()){
            //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
            ImprimeDeque(pilha_pos_fixa);
            int preced = precedencia(token.lexema);
            string controle_pilha_pos_fixa = "";
            controle_pilha_pos_fixa = pilha_pos_fixa.front();
            int preced_controle = precedencia(controle_pilha_pos_fixa);
            cout << "precedencia lexema: " << preced << endl;
            cout << "precedencia topo da pilha: " << preced_controle << endl;
            while(precedencia(controle_pilha_pos_fixa) >= preced && !pilha_pos_fixa.empty() && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                cout << "VO DAR POP HEIN 1" << endl;
                pilha_pos_fixa.pop_front();
                saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
            }
        }
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        pilha_pos_fixa.push_front(token.lexema);
        PegaToken(file, caractere, token);
        Analisa_fator(file, caractere, token);
        cout << "Saida do analisa_fator snao: " << token.lexema << endl;
    }
    else if(token.simbolo == "sabreparenteses"){
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        pilha_pos_fixa.push_front(token.lexema);
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        PegaToken(file, caractere, token);
        cout << "Antes do analisa expressao do ( : " << token.lexema << "\n";
        Analisa_expressao(file, caractere, token);
        cout << "Voltando do analisa expressao do ( : " << token.lexema << "\n";
        if(token.simbolo == "sfechaparenteses"){
            string controle_pilha_pos_fixa = "";
            while(controle_pilha_pos_fixa != "(" && !pilha_pos_fixa.empty()){
                cout << "Pilha dentro do fecha parenteses: " << endl;
                ImprimeDeque(pilha_pos_fixa);
                //cout << saida_pos_fixa << endl;.push_back
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
                pilha_pos_fixa.pop_front();
                if(controle_pilha_pos_fixa != "(" && !pilha_pos_fixa.empty()){
                    saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                }
            }
            PegaToken(file, caractere, token);
            //cout << saida_pos_fixa << endl;.push_back
            cout << "Voltando do if fecha parent : " << token.lexema << "\n";
        }
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": parenteses aberto nao foi fechado";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }
    }
    else if(token.lexema == "verdadeiro" || token.lexema == "falso"){
        PegaToken(file, caractere, token);
    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": termo usado '" + token.lexema + "' invalido";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
    cout << "Saida do analisa fator: " << token.lexema << endl;
    return;
}

void Analisa_termo(FILE *file, char *caractere, Token &token){
    cout << "Analisa termo : " << token.lexema << "\n";
    for(int i=0;i<saida_pos_fixa.size();i++){
        cout << saida_pos_fixa[i]<< " ";
    }
    cout << endl;
    ImprimeDeque(pilha_pos_fixa);
//cout << saida_pos_fixa << endl;.push_back
    Analisa_fator(file, caractere, token);
    cout << "Depois do analisa fator : " << token.simbolo << "\n";
    ImprimeDeque(pilha_pos_fixa);
//cout << saida_pos_fixa << endl;.push_back
    while(token.simbolo == "smult" || token.simbolo == "sdiv" || token.simbolo == "se"){
        if(token.simbolo == "smult" || token.simbolo == "sdiv"){
            if(!pilha_pos_fixa.empty()){
                //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
                ImprimeDeque(pilha_pos_fixa);
                int preced = precedencia(token.lexema);
                string controle_pilha_pos_fixa = "";
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
                cout << "precedencia lexema: " << preced << endl;
                cout << "precedencia topo da pilha: " << precedencia(controle_pilha_pos_fixa) << endl;
                while(precedencia(controle_pilha_pos_fixa) >= preced && !pilha_pos_fixa.empty()){
                    cout << "VO DAR POP HEIN 2" << endl;
                    pilha_pos_fixa.pop_front();
                    saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                    controle_pilha_pos_fixa = pilha_pos_fixa.front();
                }
            }
            ImprimeDeque(pilha_pos_fixa);
            //cout << saida_pos_fixa << endl;.push_back
            pilha_pos_fixa.push_front(token.lexema);
        }
        else if(token.simbolo == "se"){
            //cout << "Saida da pilha se: " << saida_pos_fixa << endl;
            if(!pilha_pos_fixa.empty()){
                //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
                ImprimeDeque(pilha_pos_fixa);
                int preced = precedencia(token.lexema);
                string controle_pilha_pos_fixa = "";
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
                int preced_controle = precedencia(controle_pilha_pos_fixa);
                cout << "precedencia lexema: " << preced << endl;
                cout << "precedencia topo da pilha: " << preced_controle << endl;
                while(precedencia(controle_pilha_pos_fixa) >= preced && !pilha_pos_fixa.empty() && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                    cout << "VO DAR POP HEIN 3" << endl;
                    pilha_pos_fixa.pop_front();
                    saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                    controle_pilha_pos_fixa = pilha_pos_fixa.front();
                }
            }
            ImprimeDeque(pilha_pos_fixa);
            //cout << saida_pos_fixa << endl;.push_back
            pilha_pos_fixa.push_front(token.lexema);
        }


        cout << "Analisa termo com e 1: " << token.simbolo << "\n";
        PegaToken(file, caractere, token);
        cout << "Analisa termo com e 2: " << token.lexema << "\n";
        for(int i=0;i<saida_pos_fixa.size();i++){
        cout << saida_pos_fixa[i]<< " ";
        }
        cout << endl;
        Analisa_fator(file, caractere, token);
        cout << "Analisa termo com e 3: " << token.lexema << "\n";
        ImprimeDeque(pilha_pos_fixa);
    } 
    cout << "saiu plmds saiu " << token.lexema << "\n";
    ImprimeDeque(pilha_pos_fixa);
    return;
}

void Analisa_expressao_simples(FILE *file, char *caractere, Token &token){
    cout << "Simbolo expressao simples: " << token.lexema << "\n";
    ImprimeDeque(pilha_pos_fixa);
    //cout << saida_pos_fixa << endl;.push_back
    if(token.simbolo == "smais" || token.simbolo == "smenos"){
        // positivo ou negativo tratar na pos fixa: precedencia("GUITOS NOSSO REI")
        if(!pilha_pos_fixa.empty()){
            //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
            ImprimeDeque(pilha_pos_fixa);
            int preced = precedencia("positivo ou negativo");
            string controle_pilha_pos_fixa = "";
            controle_pilha_pos_fixa = pilha_pos_fixa.front();
            cout << "precedencia lexema: " << preced << endl;
            int preced_controle = precedencia(controle_pilha_pos_fixa);
            cout << "precedencia topo da pilha: " << preced_controle << endl;
            while(precedencia(controle_pilha_pos_fixa) >= preced && !pilha_pos_fixa.empty() && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                cout << "VO DAR POP HEIN 4" << endl;
                pilha_pos_fixa.pop_front();
                saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
            }
        }
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        pilha_pos_fixa.push_front(token.lexema);
        PegaToken(file, caractere, token);
    }
    Analisa_termo(file, caractere, token);
    cout << "saiu plmds saiu 2 " << token.lexema << "\n";
    ImprimeDeque(pilha_pos_fixa);
    //cout << saida_pos_fixa << endl;.push_back
    for(int i=0;i<saida_pos_fixa.size();i++){
        cout << saida_pos_fixa[i]<< " ";
    }
    cout << endl;
    while(token.simbolo == "smais" || token.simbolo == "smenos" || token.simbolo == "sou"){
        if(token.simbolo == "smais" || token.simbolo == "smenos"){
            if(!pilha_pos_fixa.empty()){
                //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
                ImprimeDeque(pilha_pos_fixa);
                int preced = precedencia(token.lexema);
                string controle_pilha_pos_fixa = "";
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
                int preced_controle = precedencia(controle_pilha_pos_fixa);
                cout << "precedencia lexema " << token.lexema << ": " << preced << endl;
                cout << "precedencia topo da pilha: " << preced_controle << endl;
                while(preced_controle >= preced && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                            cout << "VO DAR POP HEIN 5" << endl;
                            pilha_pos_fixa.pop_front();
                            saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                            if (pilha_pos_fixa.empty()){
                                break;
                            }
                            controle_pilha_pos_fixa = pilha_pos_fixa.front();
                }
            }
            pilha_pos_fixa.push_front(token.lexema);
            ImprimeDeque(pilha_pos_fixa);
            //cout << saida_pos_fixa << endl;.push_back
        }
        else if(token.simbolo == "sou"){
            if(!pilha_pos_fixa.empty()){
                //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
                ImprimeDeque(pilha_pos_fixa);
                int preced = precedencia(token.lexema);
                string controle_pilha_pos_fixa = "";
                controle_pilha_pos_fixa = pilha_pos_fixa.front();
                int preced_controle = precedencia(controle_pilha_pos_fixa);
                cout << "precedencia lexema: " << preced << endl;
                cout << "precedencia topo da pilha: " << preced_controle << endl;
                while(preced_controle >= preced && !pilha_pos_fixa.empty() && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                    cout << "VO DAR POP HEIN 6" << endl;
                    pilha_pos_fixa.pop_front();
                    saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                    if (pilha_pos_fixa.empty()){
                                break;
                            }
                    controle_pilha_pos_fixa = pilha_pos_fixa.front();
                }
            }
            pilha_pos_fixa.push_front(token.lexema);
            ImprimeDeque(pilha_pos_fixa);
            //cout << saida_pos_fixa << endl;.push_back
        }

        PegaToken(file, caractere, token);
        cout << "saiu plmds saiu 3 " << token.lexema << endl;
        Analisa_termo(file, caractere, token);
        cout << "saiu plmds saiu 4 " << token.lexema << endl;
    }
    return;
}

void Analisa_expressao(FILE *file, char *caractere, Token &token){
    //saida_pos_fixa.clear();
    cout << "Token da expressao normal 1: " << token.lexema << "\n";
    Analisa_expressao_simples(file, caractere, token);
    cout << "Token da expressao normal 2: " << token.lexema << "\n";
    ImprimeDeque(pilha_pos_fixa);
//cout << saida_pos_fixa << endl;.push_back
    while(token.simbolo == "smaior" || token.simbolo == "smaiorig" || token.simbolo == "sig" || token.simbolo == "smenor" || token.simbolo == "smenorig" || token.simbolo == "sdif"){
        if(!pilha_pos_fixa.empty()){
            //cout << "Saida da pilha sou: " << saida_pos_fixa << endl;
            ImprimeDeque(pilha_pos_fixa);
            int preced = precedencia(token.lexema);
            string controle_pilha_pos_fixa = "";
            controle_pilha_pos_fixa = pilha_pos_fixa.front();
            int preced_controle = precedencia(controle_pilha_pos_fixa);
            cout << "precedencia lexema: " << preced << endl;
            cout << "precedencia topo da pilha: " << preced_controle << endl;
            while(preced_controle >= preced && !pilha_pos_fixa.empty() && preced != -1 && preced_controle != -1 && controle_pilha_pos_fixa != "("){
                cout << "VO DAR POP HEIN 7" << endl;
                pilha_pos_fixa.pop_front();
                ImprimeDeque(pilha_pos_fixa);
                saida_pos_fixa.push_back(controle_pilha_pos_fixa);
                printVector();
                if(!pilha_pos_fixa.empty()){
                    controle_pilha_pos_fixa = pilha_pos_fixa.front();
                }
                
            }
        }
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        pilha_pos_fixa.push_front(token.lexema);
        ImprimeDeque(pilha_pos_fixa);
        //cout << saida_pos_fixa << endl;.push_back
        cout << "Token TESTE: " << token.simbolo << "\n";
        PegaToken(file, caractere, token);
        cout << "Token TESTE: " << token.lexema << "\n";
        Analisa_expressao_simples(file, caractere, token);
    }
    cout << "Token da expressao normal 3: " << token.lexema << "\n";
    ImprimeDeque(pilha_pos_fixa);
    //cout << saida_pos_fixa << endl;.push_back
    //PegaToken(file, caractere, token);
}

void Analisa_atrib_chprocedimento(FILE *file, char *caractere, Token &token){
    cout << "Entrada analisa atrb ou procedimento: " + token.lexema << endl;
    string tipo_entrada = Pesquisa_tipo(token.lexema);
    PegaToken(file, caractere, token);
    cout << "Tipo entrada: " << tipo_entrada << endl;
    if (token.simbolo == "satribuicao")
    {
        PegaToken(file, caractere, token);
        string tipo_saida = Pesquisa_tipo(token.lexema);
        cout << "Tipo saida: " << tipo_saida << endl;
        if (tipo_entrada != tipo_saida && tipo_saida != "NaN")
        {
            cout << "Token da possivel atribuicao 1: " << token.lexema << endl;
            if ((tipo_entrada == "variavel sinteiro" && tipo_saida == "funcao inteiro") || (tipo_entrada == "variavel sbooleano" && tipo_saida == "funcao booleana"))
            {
                cout << "Token da possivel atribuicao 2: " << token.lexema << endl;
                Analisa_expressao(file, caractere, token);
                Desempilha_posfixa();
            }
            else if((tipo_entrada == "funcao inteiro" && tipo_saida == "variavel sinteiro") || (tipo_entrada == "funcao booleana" && tipo_saida == "variavel sbooleano")){
                cout << contador << " Erro: Tentando atribuir variaveis a uma funcao" << endl;
            }
            else
            {
                cout << contador << " Erro: Tipos diferentes em uma atribuicao!" << endl;
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        else
        {
            cout << "Token da possivel atribuicao: " << token.lexema << endl;
            Analisa_expressao(file, caractere, token);
            Desempilha_posfixa();
        }
    }

    else{
        Chamada_procedimento(file, caractere, token);
    }
}

void Analisa_se(FILE *file, char *caractere, Token &token){
    int salva_rotulo;
    PegaToken(file, caractere, token);
    Analisa_expressao(file, caractere, token);
    Desempilha_posfixa();
    salva_rotulo = rotulo;
    cout << "Saida do analisa expresao do se: " << token.lexema << endl;
    GERA("","JMPF",salva_rotulo,NULL);
    rotulo++;
    if(token.simbolo == "sentao"){
        PegaToken(file, caractere, token);
        Analisa_comando_simples(file, caractere, token);
        cout << "Token dps do analisa comando simples ENTAO: " << token.lexema << "\n";
        if(token.simbolo == "ssenao"){
            int salva_rotulo_entao = rotulo;
            GERA("","JMP",salva_rotulo_entao,NULL);
            GERA(to_string(salva_rotulo),"NULL",NULL,NULL);
            PegaToken(file, caractere, token);
            Analisa_comando_simples(file, caractere, token);
            GERA(to_string(salva_rotulo_entao),"NULL",NULL,NULL);
            cout << "Token dps do analisa comando simples SENAO: " << token.lexema << "\n";
        }
        else{
            GERA(to_string(salva_rotulo),"NULL",NULL,NULL);
        }
    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": falta um 'entao' apos o se";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
        //return;
    }
}

void Analisa_enquanto(FILE *file, char *caractere, Token &token){
    int salva_rotulo_enquanto = rotulo;
    rotulo++;
    GERA(to_string(salva_rotulo_enquanto),"NULL",NULL,NULL);
    PegaToken(file, caractere, token);
    Analisa_expressao(file, caractere, token);
    Desempilha_posfixa();
    int salva_rotulo_saida_enquanto = rotulo;
    rotulo++;
    GERA("","JMPF",salva_rotulo_saida_enquanto,NULL);
    //cout << endl << endl << saida_pos_fixa;
    if(token.simbolo == "sfaca"){
        PegaToken(file, caractere, token);
        Analisa_comando_simples(file, caractere, token);
        GERA("","JMP",salva_rotulo_enquanto,NULL);
        GERA(to_string(salva_rotulo_saida_enquanto),"NULL",NULL,NULL);
    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": falta um 'faca' apos o enquanto";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
}

void Analisa_leia(FILE *file, char *caractere, Token &token){
    GERA("", "RD", NULL, NULL);
    PegaToken(file, caractere, token);
    if(token.simbolo == "sabreparenteses"){
        PegaToken(file, caractere, token);
        if(token.simbolo == "sidentificador"){
            GERA("","STR",Pesquisa_endereco(token.lexema),NULL);
            if(Pesquisa_declaracao_variavel(token.lexema)){
                if(Pesquisa_tipo(token.lexema) == "variavel sinteiro" || Pesquisa_tipo(token.lexema) == "variavel sbooleano"){
                    PegaToken(file, caractere, token);
                    if(token.simbolo == "sfechaparenteses"){
                        PegaToken(file, caractere, token);
                    }
                    else{
                        msg_erro = "";
                        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou fechamento de parenteses ')' para o leia";
                        erros.push(msg_erro);
                        imprime_codigo_com_linhas();
                        imprime_erros();
                        exit(1);
                    }
                }
                else{
                    cout << "Erro: Tipo de variavel para leitura incorreto!" << endl;
                    imprime_codigo_com_linhas();
                    imprime_erros();
                    exit(1);
                }
            }
            else{
                cout << contador << "Erro: Nao existe essa variavel para leitura!" << endl;
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou identificacao da variavel para leitura";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }
    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou abertura de parenteses '(' para o leia";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
}

void Analisa_escreva(FILE *file, char *caractere, Token &token){
    PegaToken(file, caractere, token);
    if(token.simbolo == "sabreparenteses"){
        PegaToken(file, caractere, token);
        if(token.simbolo == "sidentificador"){
            GERA("","LDV",Pesquisa_endereco(token.lexema),NULL);
            if(Pesquisa_declaracao_variavel(token.lexema)){
                if(Pesquisa_tipo(token.lexema) == "variavel sinteiro"){
                    PegaToken(file, caractere, token);
                    if(token.simbolo == "sfechaparenteses"){
                        PegaToken(file, caractere, token);
                        cout << "Pos pega token do IF fehca parenteses escreva: " << token.lexema << endl;
                        /*if(token.simbolo == "sdoispontos"){
                            PegaToken(file, caractere, token);
                        }*/
                    }
                    else{
                        msg_erro = "";
                        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou fechamento de parenteses ')' para o escreva";
                        erros.push(msg_erro);
                        imprime_codigo_com_linhas();
                        imprime_erros();
                        exit(1);
                    }
                }
                else{
                    cout << "Erro: Tipo de variavel para leitura incorreto!" << endl;
                    imprime_codigo_com_linhas();
                    imprime_erros();
                    exit(1);
                }
            }
            else{
                cout << "ERRO: Variavel nao declarada no comando escreva!" << endl;
                //PegaToken(file, caractere, token);
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou identificacao da variavel para o escreva";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }

    }
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou abertura de parenteses '(' para o escreva";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
    GERA("","PRN",NULL,NULL); 
}

void Analisa_comando_simples(FILE *file, char *caractere, Token &token){
    cout << "Entrada analisa comando simples: " + token.lexema << endl;
    if (token.simbolo == "sidentificador") {
        Analisa_atrib_chprocedimento(file, caractere, token);
    }
    else if (token.simbolo == "sse") {
        Analisa_se(file, caractere, token);
    }
    else if (token.simbolo == "senquanto") {
        Analisa_enquanto(file, caractere, token);
    }
    else if (token.simbolo == "sleia") {
        Analisa_leia(file, caractere, token);
    }
    else if (token.simbolo == "sescreva") {
        Analisa_escreva(file, caractere, token);
    }
    /*else if(token.simbolo == "ssenao"){
        PegaToken(file, caractere, token);
        Analisa_comando_simples(file, caractere, token);
    }*/
    else {
        Analisa_comandos(file, caractere, token);
        cout << "Voltou do analisa comandos do analisa comandos simples" << endl;
    }
}

void Analisa_comandos(FILE *file, char *caractere, Token &token){
    cout << "\nAnalisa Comandos: " << token.simbolo << endl;
    //PegaToken(file, caractere, token);
    if(token.simbolo == "sinicio"){
        PegaToken(file, caractere, token);
        cout << "Analisa Comandos pos inicio: " + token.lexema << endl;
        Analisa_comando_simples(file, caractere, token);
        cout << "Volta do comando simples do analisa_comandos: " + token.lexema << endl;
        while(token.simbolo != "sfim"){
            cout << "While do analisa comandos: "+ token.lexema << endl;
            if(token.simbolo == "spontoevirgula"){
                PegaToken(file, caractere, token);
                cout << "While do analisa comandos pos if: "+ token.simbolo << endl;
                if(token.simbolo != "sfim"){
                    Analisa_comando_simples(file, caractere, token);
                }
            }
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ';' apos comando";
                erros.push(msg_erro);
                imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        cout << "Pos while do analisa comandos 1: "+ token.simbolo << endl;
        PegaToken(file, caractere, token);
        cout << "Pos while do analisa comandos 2: "+ token.simbolo << endl;
    }
    else{
        
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou usar 'inicio' para iniciar o programa declarado";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }
    return;
}

void AnalisaBloco(FILE *file, char *caractere, Token &token){

    Analisa_et_variaveis(file, caractere, token);
    Analisa_subrotinas(file, caractere, token);
    cout << "Voltou do analisa sub rotinas do analisa bloco: " + token.simbolo << endl;
    Analisa_comandos(file, caractere, token);
    cout << "Voltou do analisa comandos do analisa bloco" << endl;
}

void AnalisadorSintatico(FILE *file) {

    //cria uma estrutura de dados do tipo token para receber os tokens do lexico
    Token token;
    char caractere;

    //pega tokens e analisa se ele eh o "sprograma" que a linguagem exige
    PegaToken(file, &caractere, token);
    if(token.simbolo == "sprograma"){
        //se o arquivo de fato comecar com "sprograma", pega mais um token e testa para ver se a variavel programa eh identificada com um identificador
        PegaToken(file, &caractere, token);
        if(token.simbolo == "sidentificador"){
            Insere_tabela(token.lexema,"nomedoprograma","",NULL);
            //se a variavel for identificada com um identificador, testa se a declaracao eh fechada com um ";"
            PegaToken(file, &caractere, token);
            if(token.simbolo == "spontoevirgula"){
                GERA("", "START", NULL, NULL);
                //se o programa se iniciou com a declaracao correta do programa, passa entao para a analise do bloco de comandos
                PegaToken(file, &caractere, token);
                cout << "Anlisador Sintatico: " + token.simbolo << "\n";
                AnalisaBloco(file, &caractere, token);
                GERA("", "HLT", NULL, NULL);
            }

            //se faltou ";" na declaracao do programa, coloca na fila uma mensagem de erro
            else{
                msg_erro = "";
                msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": faltou ';' apos declaracao do programa";
                erros.push(msg_erro);
                //imprime_codigo_com_linhas();
                imprime_erros();
                exit(1);
            }
        }
        //se o programa foi declarado mas nao identificado com identificador, coloca erro na fila
        else{
            msg_erro = "";
            msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": o nome de identificacao do programa nao foi inserido";
            erros.push(msg_erro);
            imprime_codigo_com_linhas();
            imprime_erros();
            exit(1);
        }
    }
    //se o programa nao iniciou declarando o "programa", coloca erro na fila
    else{
        msg_erro = "";
        msg_erro = "ERRO SINTATICO NA LINHA " + to_string(contador) + ": A linguagem exige que o inicio do codigo seja a declaracao do programa usando 'programa'";
        erros.push(msg_erro);
        imprime_codigo_com_linhas();
        imprime_erros();
        exit(1);
    }

}

void imprime_codigo_com_linhas(){
    FILE *arquivo_para_imprimir;
    arquivo_para_imprimir = fopen(PATH_TXT, "r");
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

void imprime_erros(){
    while (!erros.empty()) {
        // Acessa o primeiro erro da fila e o imprime
        cout << erros.front() << endl;
        // Remove o erro da fila
        erros.pop();
    }
}

int main() {
    cout << PATH_TXT << "\n";
     ofstream APAGA_GERA(PATH_GERA, ios::out); // Abre no modo de escrita padrão (apaga todo o conteúdo)
    if (!APAGA_GERA.is_open()) {
        cerr << "Não foi possível abrir o arquivo!" << endl;
        return 0;
    }
    // Como o arquivo foi aberto no modo de escrita, ele já está vazio
    APAGA_GERA.close(); // Fecha o arquivo

    std::ofstream logFile(PATH_LOG);
    if (!logFile) {
        std::cerr << "Erro ao abrir o arquivo de log!" << std::endl;
        return 1;
    }
    std::streambuf* coutBuffer = std::cout.rdbuf(); // Salva o buffer padrão
    std::cout.rdbuf(logFile.rdbuf()); // Redireciona para o arquivo
    
    //abertura do arquivo fonte
    FILE *file;
    //file = fopen("C:/CodigoParaCompilador.txt", "r");
    file = fopen(PATH_TXT, "r");
    
    //tratamento de erro na abertura do arquivo
    if (file == NULL) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    

    //chama o analisador sintatico
    AnalisadorSintatico(file);
    //Desempilhar();

    imprime_codigo_com_linhas();
    imprime_erros();
    //ImprimirTabela();

    //cout << endl << endl << saida_pos_fixa;


    fclose(file);
    std::cout.rdbuf(coutBuffer); // Restaura o buffer padrão
    logFile.close();
    return 0;
}