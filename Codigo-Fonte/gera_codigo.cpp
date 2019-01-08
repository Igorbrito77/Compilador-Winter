#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>    
#include <tuple>
#include  "cabecalhos.h"
#include "tabela_tipos.h"

vector<tuple<string,string>> string_tamanho;
vector<tuple<string,string>> vetor_tamanho;
vector<tuple<string,string,string>> matriz_tamanho;

string somaString = "";

void gera_codigo_declaracao_atribuicao(atributos& ss, atributos s1, atributos s2, atributos s4){

        ss.amostra = " ";
        if(s1.tipo.compare("string") == 0)
        {

            if(verifica_declaracao(s2.label) == nao_declarada)
            {
                if(s1.tipo != s4.tipo)                
                    yyerror("Tipos incompatíveis para operação. "+s1.tipo+" != "+s4.tipo);
                
                else
                {
                    if(s4.label.compare(" ")==0)
                    {
                        cria_variavel("string", s2.label,string("0"));
                        gera_codigo_atribuicao_string(ss,s2,s4);  
                    }
                    else
                    {
                        cria_variavel("string", s2.label,string("0"));
                        ss.traducao = somaString  + "\t" + retorna_nome_var(s2.label) + " = " + s4.label + ";\n\n";
                        somaString = "";
                    }  
                    
                }
            }
            else 
                erro_declaracao(s2.label);
        }
        else
        { 
          
            string valor;
            verifica_compatibilidade(s1.tipo, s4.tipo);

            if(verifica_declaracao(s2.label) != pilha_blocos.size()-1){

                    if(s1.tipo != s4.tipo){
                        
                        valor = conversao_atribuicao(s1.tipo, s4.amostra); // cria uma nova temp pra salvar a converte do valor
                        ss.traducao = "\t" + s4.traducao + valor +"\t"+ cria_variavel(s1.tipo, s2.label, s4.valor) + " = " + retorna_nome_temp(s1.tipo, -1) +";\n\n";
                    }
                    else{
                        // cria_variavel(s1.tipo, s2.label,s4.valor);                      
                        ss.traducao = "\t"+ s4.traducao + "\t" + cria_variavel(s1.tipo, s2.label, s4.valor) + " = " + s4.amostra+ ";\n";
                    }
            }
            else 
                  erro_declaracao(s2.label);
        }
}

void gera_codigo_atribuicao_string(atributos& ss, atributos s2, atributos s4)
{

    string aux,temp;

    aux = cria_temporaria(to_string(s4.valor.length()) + " - 1 " ,"int");
    temp = retorna_nome_temp("int", -1);

    aux = aux + cria_temporaria("(char*)malloc(sizeof(char)*"+temp+")","string") + "\t" + "strcpy("+retorna_nome_temp("char", 0)+","+s4.valor+");"
    + "\n\n\t" + retorna_nome_var(s2.label) + " = (char*)malloc(sizeof(char)*"+temp+");" 
    + "\n\n\t" + "strcpy("+ retorna_nome_var(s2.label) +","+retorna_nome_temp("char", 0)+");" + "\n\n";
    ss.traducao = aux;

    auto tupla = std::make_tuple(retorna_nome_var(s2.label),temp);
    string_tamanho.push_back(tupla);
}


void gera_codigo_cin_string(atributos& ss, atributos s3, string str_msg)
{
    string aux,temp;
    ss.amostra = " ";
    string_on = 1;

    aux ="\tcin>>TEMP_aux;\n\n" + cria_temporaria("conta(TEMP_aux) " ,"int");
    temp = retorna_nome_temp("int", -1);

    aux = str_msg + aux + cria_temporaria("(char*)malloc(sizeof(char)*"+temp+")","string") + "\t" + "strcpy("+retorna_nome_temp("char", 0)+
    ",const_cast<char*>(TEMP_aux.c_str()));"+ "\n\n\t" + retorna_nome_var(s3.label) + " = (char*)malloc(sizeof(char)*"+temp+");" 
    + "\n\n\t" + "strcpy("+ retorna_nome_var(s3.label) +","+retorna_nome_temp("char", 0)+");" + "\n\n";
    ss.traducao = aux;

    auto tupla = std::make_tuple(retorna_nome_var(s3.label),temp);
    string_tamanho.push_back(tupla);   
}


void gera_codigo_atribuicao(atributos &ss, atributos s1, atributos s3){
        
        // if(!verifica_declaracao(s1.label) == nao_declarada)
           // erro_nao_declaracao(s1.label);
        string valor, tipo1 = retorna_tipo(s1.label);
        ss.amostra = " ";

        if(tipo1.compare("string") == 0)
        {
            if(tipo1 != s3.tipo)            
                yyerror("Tipos incompatíveis para operação. "+s1.tipo+" != "+s3.tipo);

            else
            {
                if(s3.label.compare(" ")==0)
                {
                    gera_codigo_atribuicao_string(ss,s1,s3);  
                }
                else
                {
                   ss.traducao = somaString + "\t" + retorna_nome_var(s1.label) + " = " + s3.label + ";\n\n";
                   somaString = ""; 
                }
                
            }
        }

        else{

        verifica_compatibilidade(tipo1, s3.tipo);
        if(tipo1 != s3.tipo){
            
            valor = conversao_atribuicao(tipo1, s3.label); // cria uma nova temp pra salvar a converte do valor
            ss.traducao = s3.traducao + "\n" + valor +"\t" + retorna_nome_var(s1.label) + " = " + retorna_nome_temp(tipo1, -1) +";\n\n";
        }
        else{
            ss.traducao = s3.traducao + "\t" + retorna_nome_var(s1.label)+ " = " + s3.amostra + ";\n";
        }  

    }
}


void gera_codigo_declaracao(atributos &ss, atributos s1, atributos s2){

    if(verifica_declaracao(s2.label) == pilha_blocos.size()-1) // se alguma variável de mesmo nome já foi declarada no mesmo bloco, se gera um erro.
        erro_declaracao(s2.label);
    
   // ss.traducao = "\t" + s1.tipo +  " "+ cria_variavel(s1.tipo, s2.label, string("0")) + ";\n";
    cria_variavel(s1.tipo, s2.label, string("0"));
    ss.traducao = "";
    ss.amostra = "";
}
string gera_codigo_soma_string(atributos &s1)
{
    string aux,temp;

    aux = cria_temporaria(to_string(s1.valor.length()) + " - 1 " ,"int");
    temp = retorna_nome_temp("int", -1);
    aux = aux + cria_temporaria("(char*)malloc(sizeof(char)*"+temp+")","string") + "\t" + "strcpy("+retorna_nome_temp("char", 0)+","+s1.valor+");"
    + "\n\n";
    s1.label = retorna_nome_temp("char", 0);

    s1.traducao = temp;
    return aux;
}
void gera_codigo_operacao(atributos &ss, atributos s1, string operacao, atributos s3){
    
        verifica_compatibilidade(s1.tipo, s3.tipo);

        if(s1.tipo.compare("string") == 0)
        {
            
            string temp,aux,traducao = "";
            if(operacao.compare("+")!=0)            
                yyerror("operação "+operacao+" não é contemplada para o tipo string. ");

            if(s1.label.compare(" ") == 0 & s3.label.compare(" ") == 0)
            {
                traducao = traducao + gera_codigo_soma_string(s1);
                traducao = traducao + gera_codigo_soma_string(s3);
                traducao = traducao + cria_temporaria(s1.traducao+" + "+s3.traducao ,"int") +"\t"+ 
                cria_temporaria(retorna_nome_temp("int", 1) + " - 1","int")+ "\n\n"+
                cria_temporaria("(char*)malloc(sizeof(char)*"+retorna_nome_temp("int", 1)+")","string")+
                "\tstrcpy("+s3.label+","+s1.label+")\n\n\tstrcat(" + s3.label+","+retorna_nome_temp("char", -1)+");\n\n";

                somaString = somaString + traducao;
                ss.tipo = "string";
                ss.label = s3.label;
                ss.amostra = ss.label;
                traducao = "";
                return;
            }
            if(s1.label.compare(" ") != 0 & s3.label.compare(" ") == 0)
            {
                traducao = traducao + gera_codigo_soma_string(s3);
                traducao = traducao + cria_temporaria(retorna_temp_tamanho_string(s1.label)+" + "+s3.traducao ,"int") +"\t"+ 
                cria_temporaria(retorna_nome_temp("int", 1) + " - 1","int")+ "\n\n"+
                cria_temporaria("(char*)malloc(sizeof(char)*"+retorna_nome_temp("int", 1)+")","string")+
                "\tstrcpy("+s3.label+","+s1.label+")\n\n\tstrcat(" + s3.label+","+retorna_nome_temp("char", -1)+");\n\n";
                somaString = somaString + traducao;
                ss.tipo = "string";
                ss.label = s3.label;
                ss.amostra = ss.label;
                traducao = "";
                return;

            }
            if(s1.label.compare(" ") == 0 & s3.label.compare(" ") != 0)
            {
                traducao = traducao + gera_codigo_soma_string(s1);
                traducao = traducao + cria_temporaria(retorna_temp_tamanho_string(s3.label)+" + "+s1.traducao ,"int") +"\t"+ 
                cria_temporaria(retorna_nome_temp("int", 1) + " - 1","int")+ "\n\n"+
                cria_temporaria("(char*)malloc(sizeof(char)*"+retorna_nome_temp("int", 1)+")","string")+
                "\tstrcpy("+s1.label+","+retorna_nome_temp("char", -1)+")\n\n\tstrcat(" + s1.label+","+s3.label+");\n\n";
                somaString = somaString + traducao;
                ss.tipo = "string";
                ss.label = s1.label;
                ss.amostra = ss.label;
                traducao = "";
                return;
            }
            if(s1.label.compare(" ") != 0 & s3.label.compare(" ") != 0)
            {
                traducao = traducao + cria_temporaria(retorna_temp_tamanho_string(s1.label)+" + "+retorna_temp_tamanho_string(s3.label) ,"int") +"\t"+ 
                cria_temporaria(retorna_nome_temp("int", 1) + " - 1","int")+ "\n\n"+
                cria_temporaria("(char*)malloc(sizeof(char)*"+retorna_nome_temp("int", 1)+")","string")+
                "\tstrcpy("+retorna_nome_temp("char", 0)+","+s1.label+")\n\n\tstrcat(" + retorna_nome_temp("char", 0)+","+s3.label+");\n\n";
                somaString = somaString + traducao;
                ss.tipo = "string";
                ss.label = retorna_nome_temp("char", -2);
                ss.amostra = ss.label;
                traducao = "";
                return;
            }
        }


        ss.valor = calc(s1.valor, s3.valor, operacao);
        ss.label = retorna_nome_temp(s1.tipo, 0);
        ss.amostra = ss.label;
        ss.tipo = s1.tipo; 
        
        if(!tipos_iguais(s1.tipo, s3.tipo)){

            string aux = prioridade(s1.tipo, s3.tipo);         
            ss.tipo = aux;
            ss.traducao = conversao(s1,s3, aux, operacao);

            //ss.traducao = conversao(s1.tipo, s1.label, s1.valor, s1.traducao, s3.tipo, s3.label, s3.valor, s3.traducao, aux, operacao);
            ss.label = retorna_nome_temp(aux, -1);
            ss.amostra = ss.label;
        }
        else
            ss.traducao = s1.traducao +  "\n" + s3.traducao + "\n" + cria_temporaria(s1.amostra +" "+ operacao + " " +s3.amostra, s1.tipo);    
}

void gera_codigo_operacao_unarios(atributos &ss, atributos s1, string operacao)
{
        if(verifica_declaracao(s1.label) == nao_declarada)
            yyerror("Variavel" +s1.label + " não foi declarada ");

        if(s1.tipo.compare("int") != 0 && s1.tipo.compare("float") != 0)
            yyerror("Tipo "+ s1.tipo +" incompatível para operação. ");

        ss.tipo = s1.tipo; 
        ss.traducao = cria_temporaria(retorna_nome_var(s1.label) +" "+ operacao + " 1",s1.tipo) + "\n\t" + 
        retorna_nome_var(s1.label) +" = "+ retorna_nome_temp(s1.tipo,0)+";\n\n";
}

void gera_codigo_operacao_composta(atributos &ss, atributos s1, string operacao,atributos s3)
{
        if(verifica_declaracao(s1.label) == nao_declarada)        
            yyerror("Variável "+s1.label+" não foi declarada. ");

        if(s1.tipo.compare("int") != 0 && s1.tipo.compare("float") != 0)
            yyerror("Tipo "+ s1.tipo +" incompatível para operação. ");

        ss.tipo = s1.tipo; 
        ss.traducao = cria_temporaria(retorna_nome_var(s1.label) +" "+ operacao[0] + s3.amostra,s1.tipo)+ "\n\t"
         + retorna_nome_var(s1.label) +" = "+ retorna_nome_temp(s1.tipo,0)+";\n\n"; 
}

void sobe_valor_vetor(atributos &ss,atributos s1,atributos s3)
{
    if(s3.tipo.compare("int") != 0 )
        yyerror("O índice de acesso do vetor tem que ser do tipo int.");

    if(verifica_declaracao(s1.label) == nao_declarada)
        yyerror("Variável "+s1.label+" não foi declarada ");

    ss.tipo = s1.tipo;
    ss.traducao = s3.traducao;
    ss.label = "*("+retorna_nome_var(s1.label)+" + "+ s3.amostra+")";
    ss.amostra = ss.label;
}

void gera_codigo_declaracao_vetor(atributos &ss,atributos s1,atributos s2,atributos s3)
{
    int t1 = atoi(s3.valor.c_str());
    if(t1<0)
    {
      yyerror("O indice de acesso do vetor tem que ser positivo.");  
    }
    if(s1.tipo.compare("string")==0)
        yyerror("Arrays nao sao contemplados para string");
    if(s3.tipo.compare("int") != 0)    
        yyerror("O indice de acesso do vetor tem que ser do tipo int.");

    if(verifica_declaracao(s2.label) == pilha_blocos.size()-1) // se alguma variável de mesmo nome já foi declarada no mesmo bloco, se gera um erro.
        erro_declaracao(s2.label);

    cria_variavel(s1.tipo, s2.label, string("vetor"));

    auto tupla = std::make_tuple(retorna_nome_var(s2.label),s3.amostra);
    vetor_tamanho.push_back(tupla);

    ss.traducao = s3.traducao + "\t"+ retorna_nome_var(s2.label)+" = "+"("+s1.tipo+"*)malloc(sizeof("+s1.tipo+")*"+s3.amostra+");\n\n" ;
}

void gera_codigo_atribuicao_vetor(atributos &ss,atributos s1,atributos s3,atributos s6)
{
    string valor, tipo1 = retorna_tipo(s1.label);
    if(s3.tipo.compare("int") != 0)    
        yyerror("O indice de acesso do vetor tem que ser do tipo int.");

    if(verifica_declaracao(s1.label) == nao_declarada)    
        yyerror("Variável "+s1.label+" não foi declarada ");

    verifica_compatibilidade(tipo1, s6.tipo);
    if(tipo1 != s6.tipo){
            
        valor = conversao_atribuicao(tipo1, s6.amostra); // cria uma nova temp pra salvar a converte do valor
        ss.traducao = s6.traducao + "\n" + s3.traducao + valor +"\t" + "*("+retorna_nome_var(s1.label)+" + "+s3.amostra+")" + " = " + retorna_nome_temp(tipo1, -1) +";\n\n";
    }
    else{
        ss.traducao = s6.traducao + s3.traducao + "\t" + "*("+retorna_nome_var(s1.label)+" + "+s3.amostra+")"+ " = " + s6.amostra + ";\n";
    }  
}

void gera_codigo_declaracao_atribuicao_vetor(atributos &ss,atributos s1,atributos s2,atributos s3,atributos s5)
{
    int t1 = atoi(s3.valor.c_str());
    if(t1<0)
    {
      yyerror("O indice de acesso do vetor tem que ser positivo.");  
    }
    if(s1.tipo.compare("string")==0)
        yyerror("Arrays nao sao contemplados para string");
    if(s3.tipo.compare("int") != 0)    
        yyerror("O indice de acesso do vetor tem que ser do tipo int.");
    if(s3.valor.compare(to_string(cont_terms)))
        yyerror("Era esperado " + s3.valor + " termos para atribuir ao vetor , porem tem " + to_string(cont_terms) + " termos");

    string aux = "",aux2 = "";
    int i,j = cont_terms-1;
    gera_codigo_declaracao_vetor(ss,s1, s2,s3);
    aux2 = ss.traducao;
    for(i=0;i<cont_terms;i++)
    {
        auto tupla = terms[j];
        verifica_compatibilidade(s1.tipo, get<1>(tupla));
        aux = aux + "\t" + retorna_nome_var(s2.label) + "[" + to_string(i) + "] = " + get<0>(tupla) + ";\n\n";
        j--;
    }
    auto tupla = std::make_tuple(retorna_nome_var(s2.label),s3.amostra);
    vetor_tamanho.push_back(tupla);
    ss.traducao = aux2 + aux;
}


void gera_codigo_declaracao_atribuicao_matriz(atributos &ss,atributos s1,atributos s2,atributos s4,atributos s7,atributos s11)
{ 
    if(s1.tipo.compare("string")==0)
        yyerror("Arrays nao sao contemplados para string");
    int t1 = atoi(s4.valor.c_str());
    int t2 = atoi(s7.valor.c_str());
    int tamanho = t1*t2,k;
    if(t1<0 | t2<0)
    {
        yyerror("O indice de acesso da matriz tem que ser positivo.");
    }
    if(s4.tipo.compare("int") != 0 | s7.tipo.compare("int") != 0)    
        yyerror("O indice de acesso da matriz tem que ser do tipo int.");

    if(tamanho != cont_terms)    
        yyerror("Era esperado " + to_string(tamanho) + " termos para atribuir a matriz , porem tem " + to_string(cont_terms) + " termos");

    string aux = "",aux2 = "";
    int i,j = cont_terms-1;
    gera_codigo_declaracao_matriz(ss,s1, s2,s4,s7);
    aux2 = ss.traducao;
    for(i=0;i<t1;i++)
    {
        for(k=0;k<t2;k++)
        {
            auto tupla = terms[j];
            verifica_compatibilidade(s1.tipo, get<1>(tupla));
            aux = aux + "\t" + retorna_nome_var(s2.label) + "[" + to_string(i) + "][" + to_string(k) + "]  = " + get<0>(tupla) + ";\n\n";
            j--; 
        }
        
    }
    ss.traducao = aux2 + aux;

}

void gera_codigo_atribuicao_matriz(atributos &ss,atributos s1,atributos s3,atributos s6,atributos s9)
{
     string valor, tipo1 = retorna_tipo(s1.label);
    if(s3.tipo.compare("int") != 0 | s6.tipo.compare("int") != 0)    
        yyerror("O indice de acesso da matriz tem que ser do tipo int.");

    if(verifica_declaracao(s1.label) == nao_declarada)    
        yyerror("Variável "+s1.label+" não foi declarada ");


    verifica_compatibilidade(tipo1, s9.tipo);
    if(tipo1 != s9.tipo){
            
        valor = conversao_atribuicao(tipo1, s9.amostra); // cria uma nova temp pra salvar a converte do valor
        ss.traducao = s9.traducao + "\n" + s3.traducao + s6.traducao + valor +"\t" + 
        "*(*("+retorna_nome_var(s1.label)+" + "+ s3.amostra+")+"+ s6.amostra+")" + " = " + retorna_nome_temp(tipo1, -1) +";\n\n";
    }
    else{
        ss.traducao = s9.traducao + s3.traducao + s6.traducao + 
        "\t" + "*(*("+retorna_nome_var(s1.label)+" + "+ s3.amostra+")+"+ s6.amostra+")"+ " = " + s9.amostra + ";\n";
    }  
}

void sobe_valor_matriz(atributos &ss,atributos s1,atributos s3,atributos s6)
{
    if(s3.tipo.compare("int") != 0 & s6.tipo.compare("int") != 0)
        yyerror("O indice de acesso da matriz tem que ser do tipo int.");

    if(verifica_declaracao(s1.label) == nao_declarada)
        yyerror("Variável "+s1.label+" não foi declarada ");

    ss.tipo = s1.tipo;
    ss.traducao = s3.traducao + s6.traducao;
    ss.label = "*(*("+retorna_nome_var(s1.label)+" + "+s3.amostra+")+"+ s6.amostra+")";
    ss.amostra = ss.label;
}

void gera_codigo_declaracao_matriz(atributos &ss,atributos s1,atributos s2,atributos s3,atributos s6)
{
    int t1 = atoi(s3.valor.c_str());
    int t2 = atoi(s6.valor.c_str());
    if(t1<0 | t2<0)
    {
        yyerror("O indice de acesso da matriz tem que ser positivo.");
    }
    if(s1.tipo.compare("string")==0)
        yyerror("Arrays nao sao contemplados para string");
    
    if(s3.tipo.compare("int") != 0 | s6.tipo.compare("int") != 0)    
        yyerror("O indice de acesso da matriz tem que ser do tipo int.");

    if(verifica_declaracao(s2.label) == pilha_blocos.size()-1) // se alguma variável de mesmo nome já foi declarada no mesmo bloco, se gera um erro.
        erro_declaracao(s2.label);

    cria_variavel(s1.tipo, s2.label, string("matriz"));

    auto tupla = std::make_tuple(retorna_nome_var(s2.label), s3.amostra);
    vetor_tamanho.push_back(tupla);

    string labelInicio = "inicio_loop_"+ to_string(cont_loop);
    string labelFim ="fim_loop_"+ to_string(cont_loop);
    cont_loop++;

    ss.traducao = s3.traducao + s6.traducao + "\t"+ retorna_nome_var(s2.label)+" = "+"("+s1.tipo+"**)malloc(sizeof("+s1.tipo+")*"+ s3.amostra+
    ");\n\n\t"+
    cria_temporaria("0","int") + labelInicio + ":\n\n" + cria_temporaria(retorna_nome_temp("int",1)+"<"+ s3.amostra,"bool") +
    cria_temporaria(" !" + retorna_nome_temp("bool",1), "bool") + "\tif( " + retorna_nome_temp("bool",0) 
    + " ) goto " + labelFim + ";\n\n"
    + "\t" + retorna_nome_var(s2.label)+"["+retorna_nome_temp("int",2)+"] = (int*)malloc("+ s6.amostra +"*sizeof("+s1.tipo+"));\n\n"
    +"\t" + cria_temporaria(retorna_nome_temp("int",3)+" + 1","int")+"\n\n\t"+retorna_nome_temp("int",3)+" = "+retorna_nome_temp("int",0) + ";\n\n\tgoto "
    + labelInicio + ";\n\n\t" + labelFim + ":\n\n";
}


void gera_codigo_operacao_logica(atributos &ss, atributos s1, atributos s2, atributos s3){
        verifica_compatibilidade(s1.tipo, s3.tipo);

        ss.label = retorna_nome_temp("bool", 0); 
        ss.valor = calc(s1.valor, s3.valor, s2.traducao);
        ss.tipo = "bool"; 
        ss.amostra =  retorna_nome_temp("bool", 0);

        ss.traducao = s1.traducao +  "\n" + s3.traducao + "\n" + cria_temporaria(s1.amostra + " " + s2.traducao + " " + s3.amostra, ss.tipo);
}

void gera_codigo_operacao_logica2(atributos &ss, atributos s1, atributos s3){

        ss.label = retorna_nome_temp("bool", 0); 
        ss.valor = calc(s3.valor,string(" "), s1.traducao);
        ss.tipo = "bool";
        ss.amostra =  retorna_nome_temp("bool", 0);
        ss.traducao = s3.traducao +"\n" + cria_temporaria(s1.traducao + s3.amostra, ss.tipo);
}


void gera_codigo_conversao(atributos &ss, atributos s1, atributos s3){
        
        verifica_compatibilidade(s1.tipo, s3.tipo);
        ss.label = retorna_nome_temp(s1.tipo, 0);
        ss.traducao = conversao_atribuicao(s1.tipo, s3.amostra);
        ss.tipo = s1.tipo;
        ss.valor = to_string(atoi((s3.valor).c_str()) );
        ss.amostra = ss.label;
}

string gera_codigo_cout(atributos &ss)
{
    if(ss.label.compare(" ") == 0)
    {
        return ss.valor;
    }
    return ss.label;
}

void sobe_valor(atributos &ss, atributos s1){
    
    ss.traducao = s1.traducao;
    ss.tipo = s1.tipo;
    ss.valor = s1.valor; 
    ss.label = s1.label;
    ss.amostra = s1.amostra;
}

void sobe_valor_terminal(atributos &ss, atributos s1){
    
    ss.tipo = s1.tipo;
    ss.valor = s1.valor; 
    ss.label = " ";
    ss.traducao = " ";
    ss.amostra = s1.valor;
}

void sobe_valor_id(atributos &ss, atributos s1){
   
    ss.tipo = retorna_tipo(s1.label);
    ss.valor = retorna_valor(s1.label); 
    ss.label = retorna_nome_var(s1.label);
    ss.traducao = " ";
    ss.amostra = ss.label;
}


void empilha_label(string tipo){

    label_loop l;
    l.tipo = tipo;

    if(tipo.compare("switch") == 0){
        l.inicio = "inicio_switch_" + to_string(cont_switch); 
        l.destino = "fim_switch_" + to_string(cont_switch);
        cont_switch++;
    }
    
    else{
        l.inicio = "inicio_loop_" + to_string(cont_loop);
        l.destino = "fim_loop_" + to_string(cont_loop);
        cont_loop++;
    }

    pilha_label.push_back(l);
}



// tipo de retorno , id, declarações, bloco
string cria_funcao(atributos &ss, atributos s7, atributos s2, atributos s4, atributos s8){
     
    funcao f;

    f.tipo_retorno = s7.tipo;
    f.nome = s2.label;
    f.cabecalho =  s7.tipo + " " + s2.label + "( " + s4.traducao + ");";
    f.corpo = s7.tipo + " " + s2.label + " ( " + s4.traducao + " )\n{\n" + str_var_temporarias + "\n" + str_var_declaradas + s8.traducao + "}\n";
    f.params_funcao = params_chamada;

    if(pilha_retornos.size() != 0){
      //  cout<<"------------------ "<<pilha_retornos[0]<<endl;
        for(int i = pilha_retornos.size()-1; i >= 0; i--){
          
            if(pilha_retornos[i].compare(s7.tipo) != 0)
                yyerror("Retorno "+ pilha_retornos[i]+" incompatível com tipo "+s7.tipo+ " .");
            
        }
    }

    else if(s7.tipo.compare("void") != 0)
        yyerror("Função "+ s7.tipo+ " sem retorno.");

    

    str_var_declaradas = "";
    str_var_temporarias = "";
    pilha_funcoes.push_back(f);
    pilha_retornos.clear();
}

void contaString(int cont_loop)
{
    string conta_String = "int conta(char* aux)\n{\n\t"+cria_temporaria("1","int") + "inicio_loop_"+ to_string(cont_loop) 
+ ":\n\n" + cria_temporaria("aux["+retorna_nome_temp("int",1)+"]!='\\0'","bool") +
    cria_temporaria(" !" + retorna_nome_temp("bool",1), "bool") + "\tif( " + retorna_nome_temp("bool",0) 
    + " ) goto " + "fim_loop_"+ to_string(cont_loop) + ";\n\n"
    +"\t" + cria_temporaria(retorna_nome_temp("int",3)+" + 1","int")+"\n\n\t"+retorna_nome_temp("int",3)+" = "+retorna_nome_temp("int",0) + ";\n\n\tgoto "
    + "inicio_loop_"+ to_string(cont_loop) + ";\n\n\t" + "fim_loop_"+ to_string(cont_loop) + ":\n\n return "+retorna_nome_temp("int",3)+";\n}\n";

    cont_loop++;
    cout<<conta_String<<endl;
}

void gera_codigo_pow(atributos&ss, atributos s3, atributos s5){

    if(s3.tipo.compare("int") != 0 && s3.tipo.compare("float") != 0)
        yyerror(s3.amostra + " não é do tipo int ou float.");
    if(s5.tipo.compare("int") != 0 && s5.tipo.compare("float") != 0)
        yyerror(s5.amostra + " não é do tipo int ou float.");


    string temp_resultado =  cria_temporaria("1", s3.tipo); 
    string temp_i = cria_temporaria("0", "int");
    string temp_3 = cria_temporaria(retorna_nome_temp("int", -1) + "< " + s5.amostra, "bool");
    string temp_4 = cria_temporaria("!" + retorna_nome_temp("bool", -1), "bool"); 
    string temp_if = "\tif(" + retorna_nome_temp("bool", -1) + ") goto fim_loop_" + to_string(cont_loop) +";\n\t";
    string temp_5 = cria_temporaria(retorna_nome_temp(s3.tipo, -4) + " * " + s3.amostra, s3.tipo); 
    string temp_recebe =  "\t" + retorna_nome_temp(s3.tipo, -5) + " = " + retorna_nome_temp(s3.tipo, -1) + ";\n";
    string temp_6 = cria_temporaria(retorna_nome_temp("int", -4) + "+ 1", "int") + "\n\t";
    string temp_soma = retorna_nome_temp("int", -5) + " = " + retorna_nome_temp("int", -1) + ";\n\t";

    if(stoi (s5.valor) > 0){
        ss.traducao = 
        temp_resultado + temp_i + "inicio_loop_" + to_string(cont_loop) + " :\n" 
        + temp_3 + temp_4 + temp_if + temp_5 + temp_recebe + temp_6 + temp_soma + "goto inicio_loop_" + to_string(cont_loop) + ";\n" 
        + "fim_loop_" + to_string(cont_loop) + ":\n";

        ss.amostra = retorna_nome_temp(s3.tipo, -6);
    }
    
    else{

        string temp_modulo = cria_temporaria(s5.amostra + "* (-1)", s5.tipo);
        string recebe_modulo = s5.amostra  + " = " + retorna_nome_temp(s5.tipo, -1) + "\n\t";

        ss.traducao = 
        temp_modulo + recebe_modulo + temp_resultado + temp_i + "inicio_loop_" + to_string(cont_loop) + " :\n" 
        + temp_3 + temp_4 + temp_if + temp_5 + temp_recebe + temp_6 + temp_soma + "goto inicio_loop_" + to_string(cont_loop) + ";\n" 
        + "fim_loop_" + to_string(cont_loop) + ":\n";
    }

    cont_loop++;
}

void gera_codigo_fatorial(atributos&ss, atributos s3){

    if(s3.tipo.compare("int") !=0)
        yyerror(s3.amostra + " não é do tipo int.");

    string temp_fatorial = cria_temporaria("1", "int");
    string temp_2 = cria_temporaria(s3.amostra + "> 1" , "bool");
    string temp_3 = cria_temporaria("!" + retorna_nome_temp("bool", -1), "bool"); 
    string temp_if = "\tif(" + retorna_nome_temp("bool", -1) + ") goto fim_loop_" + to_string(cont_loop) +";\n\t";
    string temp_4 = cria_temporaria( retorna_nome_temp("int", -3) + " * " + s3.amostra, "int");
    string temp_recebe = "\t" + retorna_nome_temp("int", -4) + " = " + retorna_nome_temp("int", -1) + ";\n\t";
    string temp_diminui = cria_temporaria(s3.amostra + " - 1", "int");
    string temp_atualiza = "\t" + s3.amostra + " = " + retorna_nome_temp("int", -1) + ";\n\t";

    ss.traducao = 

    temp_fatorial + "inicio_loop_" + to_string(cont_loop) + " :\n" 
    + temp_2 + temp_3 + temp_if + temp_4 + temp_recebe + temp_diminui + temp_atualiza 

    +  "goto inicio_loop_" + to_string(cont_loop) + ";\n"  + "fim_loop_" + to_string(cont_loop) + ":\n";

    ss.amostra = retorna_nome_temp("int", -5);
}

