#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include  "cabecalhos.h"
#include "conversao_precisa.h"


string calc(string valor1, string valor2, string operacao){

    string resultado;

    if(operacao.compare("+") == 0){
        resultado = to_string_with_precision( (strtof((valor1).c_str(),0) + strtof((valor2).c_str(),0)) );
    }

    else if(operacao.compare("-") == 0){
        resultado = to_string_with_precision( (strtof((valor1).c_str(),0) - strtof((valor2).c_str(),0)) );
    }

    else if(operacao.compare("*") == 0){
        resultado = to_string_with_precision( (strtof((valor1).c_str(),0) * strtof((valor2).c_str(),0)) );
    }

    else if(operacao.compare("/") == 0){
        resultado = to_string_with_precision((strtof((valor1).c_str(),0) / strtof((valor2).c_str(),0))  );
    }

     else if(operacao.compare(">") == 0){

            if((strtof((valor1).c_str(),0) > (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }

     else if(operacao.compare("<") == 0){

            if((strtof((valor1).c_str(),0) < (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }
    else if(operacao.compare("==") == 0){

            if((strtof((valor1).c_str(),0) == (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }
    else if(operacao.compare("!=") == 0){

            if((strtof((valor1).c_str(),0) != (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }
    else if(operacao.compare(">=") == 0){

            if((strtof((valor1).c_str(),0) >= (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }
    else if(operacao.compare("<=") == 0){

            if((strtof((valor1).c_str(),0) <= (strtof((valor2).c_str(),0))))
                return "true";
            return "false";
    }
    else if(operacao.compare("&") == 0){

            if(valor1.compare("true")==0 && valor2.compare("true")==0)
                return "true";
            return "false";
    }
    else if(operacao.compare("|") == 0){

            if(valor1.compare("false")==0 && valor2.compare("false")==0)
                return "false";
            return "true";
    }
    else if(operacao.compare("!") == 0){

            if(valor1.compare("true")==0 )
                return "false";
            return "true";
    }

    return resultado;
}