#ifndef CABECALHOS_H
#define CABECALHOS_H

#include <iostream>
#include <sstream>
#include <string> 
#include <vector>
#include <tuple>
#include <stack>

#define YYSTYPE atributos //assegura o tipo...


using namespace std;

extern int yy_flex_debug;


extern int n_linha;
extern int cont_temp;
extern int cont_var;
extern int cont_label;
extern int cont_loop;
extern int cont_switch;
extern int string_on;
extern int cont_terms;
extern vector<tuple<string, string>> terms;
extern vector < vector<tuple<string, string, string, string>> > pilha_blocos;
extern string str_var_temporarias;
extern string str_var_declaradas;
extern string str_var_globais;
extern string conta_String;
extern vector<tuple<string,string>> string_tamanho;
extern vector<tuple<string,string>> vetor_tamanho;
extern vector<tuple<string,string,string>> matriz_tamanho;
extern string somaString;



struct atributos{
    string label;
    string traducao;
    string tipo;
    string valor;
    string amostra;
};

enum Declaracao{
	nao_declarada = -1
};

struct label_loop 
{
	string tipo;
	string inicio;
	string destino;
};
extern vector<label_loop> pilha_label;


struct funcao
{
	string nome;
	string tipo_retorno;
	string nome_retorno;
	string corpo;
	string cabecalho;
	vector <string> params_funcao;
};
extern vector<funcao> pilha_funcoes;
extern vector<string> pilha_retornos;
extern vector<string> params_chamada;

void yyerror(string);
void erro_declaracao(string);
void erro_nao_declaracao(string);
void erro_desvio(string);
void erro_continue_fora_loop();

int yylex(void);
int yyparse();
string cria_variavel(string, string, string);
string cria_temporaria(string, string);
int verifica_declaracao(string nome);
void atualiza_valor(string, string, string);
string calc(string, string, string);
void verifica_compatibilidade(string, string);
string retorna_tipo(string);
string retorna_tipo_funcao(string);
string retorna_valor(string);
string retorna_nome_var(string);
string retorna_temp_tamanho_string(string);
string prioridade(string, string);
string conversao_atribuicao(string, string);
string conversao_explicita(string, string);
string conversao(atributos,atributos, string, string);
string converte(atributos,atributos, string);
int tipos_iguais(string, string);
string retorna_nome_temp(string, int);
string cria_label(string);


void gera_codigo_operacao(atributos&, atributos, string, atributos);
void gera_codigo_operacao_unarios(atributos &, atributos , string );
void gera_codigo_operacao_composta(atributos &, atributos s1, string operacao,atributos );
string gera_codigo_soma_string(atributos &);
void gera_codigo_declaracao(atributos&, atributos, atributos);
void gera_codigo_atribuicao(atributos&ss, atributos s1, atributos s3);
void gera_codigo_declaracao_atribuicao(atributos&, atributos, atributos, atributos);
void sobe_valor(atributos &, atributos);
void sobe_valor_terminal(atributos &, atributos);
void sobe_valor_id(atributos &, atributos);
void gera_codigo_operacao_logica(atributos &, atributos, atributos, atributos);
void gera_codigo_conversao(atributos &, atributos, atributos);
void gera_codigo_operacao_logica2(atributos &, atributos, atributos);
void sobe_valor_vetor(atributos &, atributos, atributos);
void gera_codigo_declaracao_vetor(atributos &, atributos, atributos, atributos);
void gera_codigo_atribuicao_vetor(atributos &, atributos, atributos, atributos);
void gera_codigo_declaracao_atribuicao_vetor(atributos &, atributos, atributos, atributos, atributos);
void gera_codigo_declaracao_atribuicao_matriz(atributos &, atributos, atributos, atributos, atributos, atributos);
void gera_codigo_atribuicao_matriz(atributos &, atributos, atributos, atributos, atributos);
void sobe_valor_matriz(atributos &, atributos, atributos, atributos);
void gera_codigo_declaracao_matriz(atributos &, atributos, atributos, atributos, atributos);

string gera_codigo_cout(atributos &ss);

void empilha_label(string);

void gera_codigo_atribuicao_string(atributos& ss, atributos s2, atributos s4);
//void gera_codigo_cin_string(atributos& ss, atributos s1, atributos s2, atributos s3);
void gera_codigo_cin_string(atributos&,atributos,string);


string cria_funcao(atributos &, atributos, atributos, atributos, atributos);
void verifica_funcao(string);
int verifica_declaracao_funcao(string);

void contaString(int);


void gera_codigo_pow(atributos&,atributos,atributos);
void gera_codigo_fatorial(atributos&, atributos);

string getVetorTamanho(string);

#endif