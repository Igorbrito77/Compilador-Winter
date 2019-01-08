%{
#include <string>    
%}

LINHA  [\n]
DIGITO [0-9]
LETRA [A-Za-z_]
DECIMAL [0-9]*\.[0-9]+|[0-9]+\.[0-9]* 
BRK_N (break_)[1-9]

NUM {DIGITO}+
COMPOSTO "+="|"-="|"*="|"/="
SUB_UN "--" 
ADD_UN "++"
STRING \"[^"\\]*(?s:\\.[^"\\]*)*\"
ID {LETRA}({LETRA}|{NUM})*
FLUTUANTE {DECIMAL}
BOOLEANO (true)|(false)
CARACTER (\')([^\\\'\n]|(\\.))(\')
RELACIONAL (==)|(!=)|(<=)|(>=)|(>)|(<)

BREAK_N ({BRK_N})

%%

"int"   {yylval.tipo = "int"; return TK_TIPO;}
"float" {yylval.tipo = "float";return TK_TIPO;}
"char"    {yylval.tipo = "char";return TK_TIPO;}
"bool"  {yylval.tipo = "bool";return TK_TIPO;}
"string" {yylval.tipo = "string";return TK_TIPO;}
"void"	{yylval.tipo = "void"; return TK_TIPO;}


"Main" {return TK_MAIN;}

[(){}[\];,=*/:]    { return *yytext; }
[\*/+-]        { return *yytext; }

{COMPOSTO}	  {yylval.traducao = yytext;return TK_COMPOSTO;}

{SUB_UN} 		{yylval.traducao = yytext; return TK_SUB_UN;}
{ADD_UN}		{yylval.traducao = yytext; return TK_ADD_UN;}

{RELACIONAL} {yylval.traducao = yytext;return TK_RELACIONAL;}  
[&|!]		{yylval.traducao = yytext;return TK_LOGICO;} 

{BREAK_N}	{yylval.traducao = yytext; return TK_BREAK_N;}

{LINHA}         { n_linha++; }

(break)		{return TK_BREAK;}

(break_all) {return TK_BREAK_ALL;}

(continue)	{return TK_CONTINUE;}

(do)		{empilha_label("do_while"); return TK_DO;}
 
(while)		{
				//if(pilha_label[pilha_label.size()-1].tipo.compare("do") != 0){

					empilha_label("while");
				//}
				return TK_WHILE;
			}

(if)		{yylval.traducao = " ";return TK_IF;}

(else)		{return TK_ELSE;}

(for)		{empilha_label("for"); return TK_FOR;}

(switch)	{empilha_label("switch"); pilha_blocos.push_back(vector<tuple<string, string, string, string>>()); return TK_SWITCH;}

(case)		{return TK_CASE;}

(default)	{return TK_DEFAULT;}

(global)	{return TK_GLOBAL;}

(function)	{ pilha_blocos.push_back(vector<tuple<string, string, string, string>>()); return TK_FUNCTION;}

(return)	{return TK_RETURN;}

(static)	{return TK_STATIC;}

"input" {yylval.traducao = " "; return TK_INPUT;}

"print"	{yylval.traducao = " "; return TK_PRINT; }


(pow)	{yylval.traducao = " "; return TK_POW; }

(factorial)	{yylval.traducao = " "; return TK_FACTORIAL;}


{NUM} {yylval.valor = yytext;        yylval.tipo = "int"; yylval.traducao = " "; return TK_TERMINAL;}
{FLUTUANTE} {yylval.valor = yytext; yylval.tipo = "float"; yylval.traducao = " "; return TK_TERMINAL;}
{BOOLEANO} {yylval.valor = yytext;  yylval.tipo = "bool"; yylval.traducao = " "; return TK_TERMINAL;}
{CARACTER} {yylval.valor = yytext;  yylval.tipo = "char"; yylval.traducao = " "; return TK_TERMINAL;}
{ID} {yylval.label = yytext; return TK_ID;}
{STRING} {yylval.valor = yytext;  yylval.tipo = "string";return TK_TERMINAL;}

%%
