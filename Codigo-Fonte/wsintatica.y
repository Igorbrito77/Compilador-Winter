%{
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>    
#include <tuple>
#include <stdlib.h>
#include <stack>
#include "cabecalhos.h"   

#define YYDEBUG 1



vector < vector<tuple<string, string, string, string>> > pilha_blocos;
vector <tuple<string, string>> terms;
vector <label_loop> pilha_label;
vector <funcao> pilha_funcoes;
vector <string> pilha_retornos;
vector <string> params_chamada;

int n_linha = 1;
int cont_temp = 1;
int cont_var = 1;
int cont_label = 1;
int cont_loop = 1;
int cont_switch = 1;
int string_on = 0;
int cont_terms = 0;

string str_var_temporarias = "";
string str_var_declaradas = "";
string str_var_globais = "";
string str_imports = "#include <iostream>\n#include <sstream>\n#include <string>\n#include<stdlib.h>\n";

int num; 
using namespace std;

%}


%token TK_MAIN TK_ID TK_TERMINAL TK_GLOBAL TK_STATIC
%token TK_TIPO_INT TK_TIPO TK_OPERACAO TK_RELACIONAL TK_LOGICO TK_COMPOSTO TK_SUB_UN TK_ADD_UN
%token TK_IF TK_ELSE TK_FOR TK_WHILE TK_DO TK_SWITCH TK_CASE TK_DEFAULT TK_BREAK TK_BREAK_ALL TK_CONTINUE TK_INPUT TK_PRINT TK_BREAK_N TK_FUNCTION TK_RETURN
%token TK_FACTORIAL TK_POW 
%start S

%%

S           :BLOCO_PRINCIPAL
            {
                cout<<"//COMPILADOR WINTER\n\n" + str_imports;

                if(string_on== 1)
                    cout<< "#include <cstring>\n#include <stdio.h>\n";
                
                cout<<"using namespace std;\n\n"<<endl;

                if(string_on== 1)
                {
                   contaString(cont_loop); 
                }

                for(int i = 0; i < pilha_funcoes.size(); i++){
                    cout<<pilha_funcoes[i].cabecalho<<endl;
                }

                cout<<"\n\n"<<str_var_globais<<endl;

                for(int i = 0; i < pilha_funcoes.size(); i++){
                    cout<<pilha_funcoes[i].corpo<<endl;
                }


                cout<<"\n\nint main()\n{\n";
                
                if(string_on == 1)
                    cout<<"\tchar *TEMP_aux = (char*)malloc(sizeof(char)*999);\n\n";
                
                cout<<str_var_temporarias + "\n\n"<< str_var_declaradas+ "\n" <<endl;
                cout<<$1.traducao <<" return 0;\n}"<<endl;
            }


BLOCO_PRINCIPAL: BP  BLOCO_PRINCIPAL
            {
                $$.traducao = $1.traducao +  $2.traducao;
            }
            |{$$.traducao = "";}
            ;

BP         : TK_GLOBAL DECLARACAO 
            {
                $$.traducao = $2.traducao;
            }
            
            | FUNCAO {$$.traducao = " ";}

            |TK_STATIC TK_FUNCTION TK_MAIN '(' ')' BLOCO                    { $$.traducao = $6.traducao; } 
            ;       

X           :                                                   { pilha_blocos.push_back(vector<tuple<string, string, string, string>>()); }
            ;   

FUNCAO      :TK_FUNCTION TK_ID '(' DECS_FUN ')' ':' TK_TIPO BLOCO_SX
            {
                cria_funcao($$, $7, $2, $4, $8);
                $$.traducao = " ";
                pilha_blocos.pop_back();
                params_chamada.clear();
            }
            ;
 
DECS_FUN    : DEC_FUN ',' DECS_FUN  { $$.traducao = $1.traducao + ", " + $3.traducao;}
            | DEC_FUN               { $$.traducao = $1.traducao;}
            |
            ;

DEC_FUN     : TK_TIPO TK_ID
            {
                if(verifica_declaracao($2.label) == pilha_blocos.size()-1) // se alguma variável de mesmo nome já foi declarada no mesmo bloco, se gera um erro.
                    erro_declaracao($2.label);

                string nome = "VAR_" + $1.tipo +"_" + to_string(cont_var);
                auto tupla = std::make_tuple($1.tipo, $2.label, nome, "0");
                pilha_blocos[pilha_blocos.size()-1].push_back(tupla);
                $$.traducao = $1.tipo + " " + nome;
                cont_var++;
                params_chamada.push_back($1.tipo);
                // gera_codigo_declaracao($$,$1, $2);
            } 
            ;

BLOCO       :X'{' COMANDOS '}'                                  { $$.traducao = $3.traducao; pilha_blocos.pop_back();}
           // |X '{' '}'                                           {$$.traducao = " ";}
            ;

BLOCO_SX:  '{' COMANDOS '}'                                      { $$.traducao = $2.traducao;}
            ;

COMANDOS    : COMANDO COMANDOS                                  { $$.traducao = $1.traducao + $2.traducao; }
            |                                                   { $$.traducao = "";} 
            ;

COMANDO     : ATRIBUICAO        { $$.traducao = $1.traducao;}
            | DECLARACAO        { $$.traducao = $1.traducao;}
            | IF_ELSE           { $$.traducao = $1.traducao;}
            | WHILE             { $$.traducao = $1.traducao;}
            | DO_WHILE          { $$.traducao = $1.traducao;}
            | FOR               { $$.traducao = $1.traducao;}
            | FOREACH           { $$.traducao = $1.traducao;}
            | BREAK             { $$.traducao = $1.traducao;}
            | BREAK_ALL         { $$.traducao = $1.traducao;}
            | BREAK_N           { $$.traducao = $1.traducao;}
            | CONTINUE          { $$.traducao = $1.traducao;}
            | SWITCH            { $$.traducao = $1.traducao;}
            | INPUT             { $$.traducao = $1.traducao;}
            | PRINT             { $$.traducao = $1.traducao;}
            | RETURN            { $$.traducao = $1.traducao;}
            | CHAMADA_FUN ';'   { $$.traducao = $1.traducao; $$.tipo = $1.tipo; $$.label = $1.label;}
 

            ;
CHAMADA_FUN : TK_ID '(' PARAMETROS ')'
            { 
                verifica_funcao($1.label);

                $$.tipo = retorna_tipo_funcao($1.label);
                $$.label = retorna_nome_temp($$.tipo, 0);
                $$.amostra = $$.label;

                if($$.tipo.compare("void") == 0)
                    $$.traducao = "\t"+$1.label + "( " + $3.traducao + " );\n";
                else
                     $$.traducao =  cria_temporaria( $1.label + "( " + $3.traducao + " )", $$.tipo);
                
                params_chamada.clear();
                //$$.traducao = "\t" + $1.label + "( " + $3.traducao + " )";
            }
            ;

PARAMETROS  : PARAMETRO ',' PARAMETROS { $$.traducao = $1.traducao + "," + $3.traducao;}
            | PARAMETRO                { $$.traducao = $1.traducao;}
            | {$$.traducao = " ";}
            ;

PARAMETRO   : F {$$.traducao = $1.amostra; params_chamada.push_back($1.tipo);} /////corrigir
            ;


RETURN      : TK_RETURN E ';'
            {

                if($2.traducao.compare(" ") == 0)
                    $$.traducao = "\treturn " + $2.amostra + ";\n";
                else{
                    $$.traducao =  $2.traducao + "\treturn " + $2.amostra + ";\n";
                }
                pilha_retornos.push_back($2.tipo);
            }
            | TK_RETURN ';'
            {
                $$.traducao = "\treturn;\n";
                pilha_retornos.push_back("void");
            }
            ;
          
INPUT       : TK_TIPO TK_ID '=' TK_INPUT '(' MSG_INPUT
            
            {   
                gera_codigo_declaracao($$, $1, $2);

                if($1.tipo.compare("string")==0)    { gera_codigo_cin_string($$, $2, $6.traducao);}
                else{
                    $$.traducao = $6.traducao + "\tcin>>" + retorna_nome_var($2.label)+ ";\n";
                    $$.tipo = $1.tipo;
                }
            }  

            |TK_ID '=' TK_INPUT '(' MSG_INPUT
            {   
                 gera_codigo_declaracao($$, $1, $2);

                if($1.tipo.compare("string")==0)    { gera_codigo_cin_string($$, $1, $5.traducao);}
                else
                    $$.traducao = $5.traducao +"\tcin>>" + retorna_nome_var($1.label)+ ";\n";
            } 
            ;
           
MSG_INPUT   : TK_TERMINAL ')'';'
            {
                if($1.tipo.compare("string") != 0)
                    yyerror("Terminal não é uma string para ser parâmetro do input.");
                else
                    $$.traducao = "\tcout<<" + $1.valor + "<<endl;\n";
            }
            | ')'';'
            {
                $$.traducao = "";
            }
            ;
            

PRINT       : TK_PRINT '(' MAISP
            {
                $$.traducao = $3.amostra + "\tcout" + $3.traducao;
            }
            ;

MAISP       : E ',' MAISP {$$.traducao = "<<" + $1.amostra + $3.traducao; $$.amostra = $1.traducao + $3.amostra;}
            | E ')'';'   {$$.traducao = "<<" + $1.amostra + "<<endl;\n" ; $$.amostra = $1.traducao;}
            ;


BREAK       :TK_BREAK ';'
            {  
                erro_desvio("BREAK");    
                $$.traducao = "\tgoto "+ pilha_label.back().destino + ";\n";
            }   
            ;   

BREAK_ALL   : TK_BREAK_ALL ';'
            {
                erro_desvio("BREAK");
                $$.traducao = "\tgoto "+ pilha_label[0].destino + ";\n";

            }
            ;

BREAK_N     : TK_BREAK_N ';'
            {
                erro_desvio("BREAK");
                int num =  atoi(($1.traducao.substr(6,7).c_str()) );
                cout<<"------->"<< num<<endl;
                
                $$.traducao = "\tgoto "+ pilha_label[pilha_label.size() -num].destino + ";\n";
            }
            ;


CONTINUE    :   TK_CONTINUE ';'
            {
                erro_desvio("CONTINUE");
                erro_continue_fora_loop();

                if(pilha_label.back().tipo.compare("switch") != 0)
                    $$.traducao = "\tgoto " + pilha_label.back().inicio + ";\n";

            }
            ; 

IF_ELSE:    TK_IF '(' E ')' BLOCO
            {
                string varTeste = retorna_nome_temp("bool", 0);
                string labelFim = cria_label("fim_if");
                $$.traducao = $3.traducao  + cria_temporaria(" !"+ $3.amostra, "bool") + "\tif( " + varTeste + " ) goto " + labelFim + ";\n" +
                $5.traducao +"\t" + labelFim +":\n";
                $$.tipo = " ";
                $$.valor = " ";
                $$.label = " "; 
            }
            
            | TK_IF '(' E ')' BLOCO TK_ELSE BLOCO
            {
                string varTeste = retorna_nome_temp("bool", 0);
                string labelFim = cria_label("fim_if");
                string labelElse = cria_label("else");

                $$.traducao = $3.traducao  + cria_temporaria(" !"+ $3.amostra, "bool") + "\tif( " + varTeste + " ) goto " + labelElse + ";\n" +
                $5.traducao + "\t\tgoto " + labelFim + ";\n\t" + labelElse + ":\n" +  $7.traducao +  "\t" + labelFim + ":\n"; 
                
            }
            ;

WHILE:      TK_WHILE '(' L ')' BLOCO
            {
               
                string varTeste = retorna_nome_temp("bool", 0);
                string labelInicio = pilha_label[pilha_label.size()-1].inicio;
                string labelFim = pilha_label[pilha_label.size()-1].destino;

                $$.traducao = "\t"+ labelInicio + ":\n" + $3.traducao + cria_temporaria(" !"+ $3.label, "bool") + "\tif( " + varTeste + " ) goto " + labelFim
                + ";\n" + $5.traducao + "\tgoto " + labelInicio + ";\n\t" + labelFim + ":\n";
                
                $$.tipo = " ";
                $$.valor = " ";
                $$.label = " "; 
                
                pilha_label.pop_back();
            }
            ;

DO_WHILE:   TK_DO BLOCO TK_WHILE '(' L ')' ';' 
            {
                string varTeste = retorna_nome_temp("bool", 0);
                string labelInicio = pilha_label[pilha_label.size()-1].inicio;
                string labelFim = pilha_label[pilha_label.size()-1].destino;

                $$.traducao = "\t" + labelInicio + ":\n" + $2.traducao + $5.traducao + cria_temporaria(" !"+ $5.label, "bool") + "\tif( " + varTeste 
                + " ) goto " + labelFim + ";\n\t" + labelFim + ":\n";
            
                pilha_label.pop_back();
            }
            ;

FOR:        X TK_FOR '(' ATRIBUICAO L ';' OPERACAO ')' BLOCO_SX
            {

                string varTeste = retorna_nome_temp("bool", 0);
                string labelInicio = pilha_label[pilha_label.size()-1].inicio;
                string labelFim = pilha_label[pilha_label.size()-1].destino;


                $$.traducao = "\t" + $4.traducao + labelInicio + ": " + $5.traducao + cria_temporaria(" !" + $5.label, "bool") + "\tif( " + varTeste 
                + " ) goto " + labelFim + ";\n" + $9.traducao+ "\t" + $7.traducao  + "\n\n\tgoto "
                + labelInicio + ";\n\n\t" + labelFim + ":\n\n";


               pilha_label.pop_back();
               pilha_blocos.pop_back();
            } 
            ;

FOREACH:    X TK_FOR '(' TK_ID ':' TK_ID ')' BLOCO_SX
            {

                string labelInicio = pilha_label[pilha_label.size()-1].inicio;
                string labelFim = pilha_label[pilha_label.size()-1].destino;


                string tamanho = getVetorTamanho(retorna_nome_var($6.label));

                if(tamanho.compare("")==0)
                {
                    yyerror("Vetor "+$6.traducao+" não foi declarado");
                }

                if(retorna_tipo($4.label).compare(retorna_tipo($6.label))!=0)
                {
                    yyerror("Tipos incompatíveis. "+retorna_tipo($4.label)+" != "+retorna_tipo($6.label));
                }

                $$.traducao = cria_temporaria("0",$4.tipo) + labelInicio + ": \n\n" + cria_temporaria(retorna_nome_temp($4.tipo,1)+"<"+tamanho,"bool")
                 + "\n"+cria_temporaria(" !" + retorna_nome_temp("bool",1), "bool") + "\n\tif( " + retorna_nome_temp("bool",0) 
                + " ) goto " + labelFim + ";\n\n\t" + retorna_nome_var($4.label) + " = "+retorna_nome_var($7.label)+"["+retorna_nome_temp("int",2)+"]\n\n"+
                cria_temporaria(retorna_nome_temp("int",3)+" + 1","int")+"\t"+retorna_nome_temp("int",3) + " = "+retorna_nome_temp("int",0)+"\n\n\t" + $8.traducao  
                + "\n\t"+retorna_nome_var($6.label)+"["+retorna_nome_temp("int",3)+"]" + " = "+retorna_nome_var($4.label)+"\n\n""\n\tgoto "
                + labelInicio + ";\n\n\t" + labelFim + ":\n\n";


               pilha_label.pop_back();
               pilha_blocos.pop_back();
            } 
            ;


SWITCH      :TK_SWITCH '(' E ')' '{' CASES  DEFAULT'}' 
            {
                int swt = cont_switch-1;
                string labelInicio = pilha_label[pilha_label.size()-1].inicio + ":";
                string labelFim = pilha_label[pilha_label.size()-1].destino + ":";
               

                $$.traducao = "\t"+ labelInicio + "\n\t"+ $3.tipo+ " switch_" + to_string(swt)+ " = " + retorna_nome_var($2.label) +";\n\n"+ $6.traducao + $7.traducao 
                + "\n\t"+labelFim + "\n";
                pilha_label.pop_back();
                pilha_blocos.pop_back();
            }
            ;
 
CASES       : CASE CASES       {$$.traducao = $1.traducao + $2.traducao;}
            |{$$.traducao = " ";}
            ;

CASE        : TK_CASE F ':' COMANDOS
            {
                 string varTeste = retorna_nome_temp("bool", 0);
                 int swt = cont_switch-1;
                 string label = cria_label("case");
                 string prox_label = "case_" + to_string(cont_label);

                 //verifica_compatibilidade($2.tipo, );

                 $$.traducao =  cria_temporaria(" switch_" + to_string(swt) + " == "+ $2.valor, "bool") + "\tif(!" + varTeste + ") goto " 
                 + prox_label+ ";\n" + $4.traducao + "\t\n" + prox_label + ":\n"; 

                 $2.traducao = "";
            }
            ;

DEFAULT     : TK_DEFAULT ':' COMANDOS  
            {
                $$.traducao = $3.traducao;
            }
            | {$$.traducao = " ";}
            ;

DECLARACAO:    TK_TIPO TK_ID ';'                                        { gera_codigo_declaracao($$,$1, $2); }       // float media;
            |  TK_TIPO TK_ID '=' E ';'                                  { gera_codigo_declaracao_atribuicao($$,$1, $2, $4); } // int contador = 1;
            |  TK_TIPO TK_ID '[' A ']' ';'       		  				{ gera_codigo_declaracao_vetor($$,$1, $2,$4); }
			|  TK_TIPO TK_ID '[' A ']' '[' A ']' ';'       				{ gera_codigo_declaracao_matriz($$,$1, $2,$4,$7); }
			|  TK_TIPO TK_ID '[' A ']' '=' '{' TERMS '}' ';'       		{ gera_codigo_declaracao_atribuicao_vetor($$,$1, $2,$4,$8);cont_terms = 0;terms.clear(); }
			|  TK_TIPO TK_ID '[' A ']' '[' A ']' '=' '{' TERMS '}' ';'  { gera_codigo_declaracao_atribuicao_matriz($$,$1, $2,$4,$7,$11);cont_terms = 0;terms.clear(); }
            ;

TERMS	:	F ',' TERMS {auto tupla = std::make_tuple( $1.amostra,$1.tipo);terms.push_back(tupla); cont_terms++;}
		|	F {auto tupla = std::make_tuple($1.amostra,$1.tipo);terms.push_back(tupla);cont_terms++;}
		;
		

ATRIBUICAO  : OPERACAO ';'                          { sobe_valor($$, $1);}
            ;

OPERACAO    : TK_ID '=' E                           { gera_codigo_atribuicao($$,$1, $3); }              // a = 7.5;
			| TK_ID TK_ADD_UN                       { gera_codigo_operacao_unarios($$, $1, "+"); }
            | TK_ID TK_SUB_UN                       { gera_codigo_operacao_unarios($$, $1, "-"); }
			| TK_ID TK_COMPOSTO F                   { gera_codigo_operacao_composta($$, $1, $2.traducao, $3); }
			| TK_ID '[' A ']' '=' E                 { gera_codigo_atribuicao_vetor($$, $1, $3, $6);} 
            | TK_ID '[' A ']' '[' A ']' '=' E       { gera_codigo_atribuicao_matriz($$, $1, $3, $6, $9);} 
            ;

POW         : TK_POW '(' F ',' F')'          { gera_codigo_pow($$, $3, $5);}
            ;

FACTORIAL   : TK_FACTORIAL '(' F ')'          { gera_codigo_fatorial($$, $3);} 
            ;

E           : A                         {  sobe_valor($$, $1); }
            | L                         {  sobe_valor($$, $1); }
            | TK_TIPO '(' F ')'         {  gera_codigo_conversao($$, $1, $3);}   //Converção explícita
            ;

A           : A '+' T                   { gera_codigo_operacao($$, $1, "+", $3); }
            | A '-' T                   { gera_codigo_operacao($$, $1, "-", $3); }
            | T                         { sobe_valor($$, $1); }
            ;

T           :  T '*' F          { gera_codigo_operacao($$, $1, "*", $3); }
            |  T '/' F          { gera_codigo_operacao($$, $1, "/", $3); }
            |  F                { $$.valor = $1.valor; $$.tipo = $1.tipo; $$.label = $1.label; $$.traducao = $1.traducao; }
            ;


L           :L TK_LOGICO R      { gera_codigo_operacao_logica($$, $1, $2, $3); }    // a || b
            |TK_LOGICO F        { gera_codigo_operacao_logica2($$, $1, $2);    }    // !a
            |R                  { sobe_valor($$, $1); }
            ;   


R           :A TK_RELACIONAL A      {    gera_codigo_operacao_logica($$, $1, $2, $3); }     // 2 > 3
            |TK_LOGICO '(' L ')'    {    gera_codigo_operacao_logica2($$, $1, $3); }        // !( a > b )
            ;

F           : TK_TERMINAL               { sobe_valor_terminal($$, $1); }
            | TK_ID                     { sobe_valor_id($$, $1); }
            //| '(' F ')'                 { }
            | CHAMADA_FUN               { $$.traducao = $1.traducao; $$.tipo = $1.tipo; $$.label = $1.label;}
            | TK_ID '[' A ']'           {  sobe_valor_vetor($$, $1, $3);} 
            | TK_ID '[' A ']' '[' A ']' {  sobe_valor_matriz($$, $1, $3, $6);}
            | POW                       {  $$.traducao = $1.traducao; }
            | FACTORIAL                 {  $$.traducao = $1.traducao; }
            ;
            
%%

#include "lex.yy.c"

int main( int argc, char* argv[] )
{
    // #if YYDEBUG
    //     yydebug = 1;
    // #endif

    pilha_blocos.push_back(vector<tuple<string, string, string, string>>());
    yyparse();
    pilha_blocos.pop_back();
    pilha_funcoes.clear();

    return 0;
}

void yyerror( string erro )
{
   cout<<"\nErro semântico: " <<erro<<endl<< "Linha: "<< n_linha <<endl<< "Próximo ao token '"<< yytext<<"'"<<endl;
    exit(0);
}                