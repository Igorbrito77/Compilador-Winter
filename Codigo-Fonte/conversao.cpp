#include <iostream>
#include <sstream>
#include <iomanip>
#include  "cabecalhos.h"
#include "conversao_precisa.h"

string conversao(atributos s1,atributos s3, string tipo_prioridade, string operacao){

    string traducao;

        if(s1.tipo.compare(tipo_prioridade) == 0)
            traducao =  s1.traducao + s3.traducao + converte(s1,s3, operacao);
        else
            traducao =  s1.traducao + s3.traducao + converte(s3,s1, operacao);

    return traducao;
}


string converte(atributos s1,atributos s3, string operacao){

    string str_temp;

    str_temp += cria_temporaria(s3.valor, s3.tipo);
    str_temp += cria_temporaria(s1.tipo + " (" + retorna_nome_temp(s3.tipo, -1) + ")", s1.tipo);
    str_temp += cria_temporaria(s1.amostra + operacao + retorna_nome_temp(s1.tipo, -1), s1.tipo);

    return str_temp;
}

string conversao_explicita(string tipo_var, string valor){

    if(tipo_var.compare("int") == 0)
        valor = to_string(atoi((valor).c_str()) );   
    else if (tipo_var.compare("float") == 0)   
        valor = to_string_with_precision( (strtof((valor).c_str(),0)));
    return valor;
}


string conversao_atribuicao(string tipo_var, string label){

    return cria_temporaria(tipo_var + "(" + label + ")", tipo_var);
}


string retorna_nome_temp(string tipo, int num){
    
    return "TEMP_" + tipo + "_" +to_string(cont_temp + num);
}
