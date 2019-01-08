#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>    
#include <tuple>
#include  "cabecalhos.h"
#include "tabela_tipos.h"

string cria_variavel(string tipo_var, string label, string valor){
    
    string nome;
    if(tipo_var.compare("string") == 0)
    {
        nome = "VAR_char_" + to_string(cont_var);
        valor = conversao_explicita(tipo_var, valor);

        auto tupla = std::make_tuple(tipo_var, label, nome, valor);
        pilha_blocos[pilha_blocos.size()-1].push_back(tupla);
        
        if(pilha_blocos.size()-1 == 0)
            str_var_globais += "char *" + nome + ";\n";
        else
            str_var_declaradas += "\tchar *" + nome + ";\n";

        cont_var++; 
        return nome; 
    }
    if(valor.compare("vetor") == 0)
    {
        nome = "VAR_" + tipo_var +"_" + to_string(cont_var);

        //valor = conversao_explicita(tipo_var, valor);

        auto tupla = std::make_tuple(tipo_var, label, nome, valor);
        pilha_blocos[pilha_blocos.size()-1].push_back(tupla);
        
        if(pilha_blocos.size()-1 == 0)
            str_var_globais += tipo_var + " *" + nome + ";\n";
        else
            str_var_declaradas += "\t" + tipo_var + " *" + nome + ";\n";

        cont_var++; 
        return nome; 
    }
    if(valor.compare("matriz") == 0)
    {
        nome = "VAR_" + tipo_var +"_" + to_string(cont_var);

        //valor = conversao_explicita(tipo_var, valor);

        auto tupla = std::make_tuple(tipo_var, label, nome, valor);
        pilha_blocos[pilha_blocos.size()-1].push_back(tupla);
        
        if(pilha_blocos.size()-1 == 0)
            str_var_globais += tipo_var + " **" + nome + ";\n";
        else
            str_var_declaradas += "\t" + tipo_var + " **" + nome + ";\n";

        cont_var++; 
        return nome; 
    }

    nome = "VAR_" + tipo_var +"_" + to_string(cont_var);

    valor = conversao_explicita(tipo_var, valor);

    auto tupla = std::make_tuple(tipo_var, label, nome, valor);
    pilha_blocos[pilha_blocos.size()-1].push_back(tupla);
    
    if(pilha_blocos.size()-1 == 0)
        str_var_globais += tipo_var + " " + nome + ";\n";
    else
        str_var_declaradas += "\t" + tipo_var + " " + nome + ";\n";

    cont_var++; 
    return nome; 
}

string cria_temporaria(string valor, string tipo){

    string nome_temp;
    if(tipo.compare("string") == 0)
    {
        nome_temp = "TEMP_char_" + to_string(cont_temp);
        str_var_temporarias += "\tchar *" + nome_temp + ";\n";
    }
    else{
        nome_temp = "TEMP_" + tipo + "_" + to_string(cont_temp);
        str_var_temporarias += "\t"+ tipo + " " + nome_temp + ";\n";
    }

    string aux = "\t" + nome_temp + " = "+ valor + ";\n\n";
    cont_temp++;
    return aux;
}

string cria_label(string prefixo){
    
    prefixo += "_" + to_string(cont_label); 
    cont_label++;
    return prefixo; 
}
