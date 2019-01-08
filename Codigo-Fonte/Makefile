all:	main

main:	lexica compila

lexica:	
		clear						
		lex wlexica.l
		yacc -d wsintatica.y

gera_codigo.o:	gera_codigo.cpp
			g++ -std=c++11 -c gera_codigo.cpp  	

criacao_variaveis.o:	criacao_variaveis.cpp
		g++ -std=c++11 -c criacao_variaveis.cpp

operacoes.o:	operacoes.cpp
		g++ -std=c++11 -c operacoes.cpp

conversao.o:	conversao.cpp
		g++ -std=c++11 -c conversao.cpp

verificacao.o:	verificacao.cpp
		g++ -std=c++11 -c verificacao.cpp

compila:	gera_codigo.o criacao_variaveis.o operacoes.o conversao.o verificacao.o  
			g++ -std=c++11 -o winter_compile y.tab.c gera_codigo.o criacao_variaveis.o operacoes.o conversao.o verificacao.o  -ll
			./winter_compile < exemplo.winter	

