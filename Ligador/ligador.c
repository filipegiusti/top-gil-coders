/*
	Sistema de Programa��o para um computador hipot�tico
    Parte 2: Ligador Relocador
    
	Projetos em Computa��o 1 2006/2

	Autores:
        Mauro Kade <>
        Rodrigo Prestes <>
		Filipe Giusti <filipegiusti@gmail.com>

	Ligador relocador da arquitetura hipot�tica
    vista em aula, recebe como argumentos os 
    arquivos a serem ligados provenientes
    do montador. O formato do arquivo est� descrito 
    em "Defini��o do arquivo objeto.txt".
    
    Gera como sa�da um arquivo para o
    carregador absoluto que est� descrito em
    "Defini��o do arquivo execut�vel.txt".

*/

#include <stdio.h>
#include <strings.h>
#include "ligador.h"
//#include ".\Estrutura comuns\estruturas.h"

#define DEBUG

int pilhaGlobal = 0;
int endGlobal = 0;
int tamCodGlobal = 0;

tabelaSimbolos tabelaSG[TAM_TAB];
int auxTabelaSG = 0;
tabelaUsos tabelaUso[TAM_TAB];
int auxTabelaUso = 0;
codigoReloc codigo[100*TAM_TAB];
int auxCodigo = 0;

main(int argc, char *argv[]) {
    short int   i, ok;          // Auxiliares

// TODO: Testar se passou algum argumento.

// Executa a 1� passagem
    for (i = 1; i < argc; i++) {
        #ifdef DEBUG
        printf("\nAbrindo %s",argv[i]);
        #endif
        ok = carregar(argv[i]);     
        /*
        codigoGlobal = codigoGlobal - ((argc-1)-1)*3;
      
            FORTES DUVIDAS A RESPEITO DESSA MERDA AQUI.
            (argc-1) � qtdade de arquivos
            ((argc-1)-1) � que o primeiro arquivo vai continuar com os endere�os 0, 1 e 2
        */
        #ifdef DEBUG
        printf("\nOK = %d\n",ok);
        system("pause");
        #endif
        if(!ok)
            printf("\n\n\tFUDEU o %s\n", argv[i]);
    }
// Executa a 2� passagem
    ok = salvar(argv[1]);
    if(!ok)
        printf("\n\n\tFUDEU enquanto salvava");
}
