#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>    
#include <tuple>
#include  "cabecalhos.h"
#include "tabela_tipos.h"

using namespace std;


int verifica_declaracao(string nome){

    for( int i = pilha_blocos.size()-1;  i >= 0; i--){

        for(auto& tuple: pilha_blocos[i]) {
            if(nome.compare(get<1>(tuple)) == 0)
            {
                    //printf("retotno =========%d\n", i);
                    return i;  
            }
        }
    }

    return nao_declarada;
}


string retorna_valor(string label){

    int pos = verifica_declaracao(label);
    if(pos == nao_declarada)
        erro_nao_declaracao(label);

    for(auto& tuple: pilha_blocos[pos]) {
        if(label.compare(get<1>(tuple)) == 0)
        {
            //cout<<"valor retornado =="<<endl;
            return get<3>(tuple);                 
        }
    }
}

string retorna_tipo(string label){

    int verifica = verifica_declaracao(label);
 
    if(verifica == nao_declarada){
        erro_nao_declaracao(label);
    }

    for(auto& tuple: pilha_blocos[verifica]) {
        if(label.compare(get<1>(tuple)) == 0)
            return get<0>(tuple);                 
    }    
}

string retorna_nome_var(string label){
   
    int pos = verifica_declaracao(label);
    
    for(auto& tuple: pilha_blocos[pos]) {
        if(label.compare(get<1>(tuple)) == 0)
        {
            //cout<<"no retorna_nome_var , RETORNANDO "<<get<2>(tuple)<<endl;
          return get<2>(tuple);                 
        }
    }

    return "";
}

string retorna_temp_tamanho_string(string label)
{
    int pos ;

    for(auto& tuple: string_tamanho) {
        if(label.compare(get<0>(tuple)) == 0)
        {
            cout<<"achei"<<endl;
            return get<1>(tuple); 
        }
    }

    return "";
}

void verifica_compatibilidade(string tipo, string tipo1){

    for(int i = 0; i < NUM_LINHAS; i++){

            if(tipo.compare(matriz_tipos[i][0]) == 0){

                if( (tipo1.compare(matriz_tipos[i][1]) == 0 || (tipo1.compare(matriz_tipos[i][2]) == 0))){
                    return;
                }

            }
    }    

    yyerror("Tipos incompatíveis para operação. "+tipo+" != "+tipo1);
    exit(0);
    
}

int tipos_iguais(string tipo1, string tipo2){
    if(tipo1.compare(tipo2) == 0)
        return 1;
    return 0;
}


string prioridade(string tipo, string tipo1){

    for(int i = 0; i < NUM_LINHAS; i++){

            if(tipo.compare(matriz_tipos[i][0]) == 0){
                if( (tipo1.compare(matriz_tipos[i][1]) == 0)){
                    return matriz_tipos[i][2];
                }
            }
    }   

    yyerror("Tipos incompatíveis para operação. "+tipo+" != "+tipo1);
    exit(0);
    
}

int verifica_declaracao_funcao(string label){

     for(int i = pilha_funcoes.size()-1; i >= 0; i--){
        if( pilha_funcoes[i].nome.compare(label) == 0)
            return i;
    }

    yyerror("A função "+ label + " não foi declarada.");
    exit(0);
}

void verifica_funcao(string label){

    int pos = verifica_declaracao_funcao(label); // verifica a declaração da função

    if(pilha_funcoes[pos].params_funcao.size() != params_chamada.size()){ // verifica o número de parâmetros passados 

        if(pilha_funcoes[pos].params_funcao.size() > params_chamada.size())
             yyerror("Poucos parâmetros passados para função.");
        else if(pilha_funcoes[pos].params_funcao.size() < params_chamada.size())
            yyerror("Parâmetros a mais enviados para a função chamada.");
        
        cout<<"\t>> "<<pilha_funcoes[pos].cabecalho<<endl;
        exit(0);
    }

    
    for(int i = 0; i < pilha_funcoes[pos].params_funcao.size(); i++){

        if(pilha_funcoes[pos].params_funcao[i].compare(params_chamada[i]) != 0){
            yyerror(" Tipo "+ params_chamada[i] +" não compatível com o tipo "+ pilha_funcoes[pos].params_funcao[i] +" do parâmetro da função.");
            exit(0);
        }
    }
}


string retorna_tipo_funcao(string label){

     for(int i = pilha_funcoes.size()-1; i >= 0; i--){
        if( pilha_funcoes[i].nome.compare(label) == 0)
            return pilha_funcoes[i].tipo_retorno;
    }

}

string getVetorTamanho(string label)
{
    int pos ;

    for(auto& tuple: vetor_tamanho) {
        if(label.compare(get<0>(tuple)) == 0)
        {
            return get<1>(tuple); 
        }
    }

    return "";
}

void erro_declaracao(string var){
    yyerror(" UMA VARIÁVEL " + var + " JÀ FOI DECLARADA.");
}

void erro_nao_declaracao(string var){
    yyerror(" VARIÁVEL " + var + " NÃO FOI DECLARADA.");   
}

void erro_desvio(string var){
     if(pilha_label.size() == 0)
        yyerror(var + " UTILIZADO FORA DE LOOP OU SWITCH.");
}

void erro_continue_fora_loop(){
    if(pilha_label.back().tipo.compare("switch")==0)
        yyerror(" continue UTILIZADO FORA DE LOOP.");
}