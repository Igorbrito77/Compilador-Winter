#ifndef TABELA_TIPOS_H
#define TABELA_TIPOS_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>


#define NUM_LINHAS 7
#define NUM_COLUNAS 3

static string matriz_tipos[NUM_LINHAS][NUM_COLUNAS] = {{"int", "int", "int"}, {"int", "float", "float"}, {"float", "float", "float"}, {"float", "int", "float"}, 
                                                {"bool", "bool", "bool"}, {"char", "char", "char"}, {"string", "string", "string"} };

#endif