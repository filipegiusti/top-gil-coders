/*
	Sistema de Programação para um computador hipotético
    Parte 2: Ligador Relocador
    
	Projetos em Computação 1 2006/2

	Autores:
        Mauro Kade <maurokade@gmail.com>
        Rodrigo Prestes <rtprestes@gmail.com>
  		Filipe Giusti <filipegiusti@gmail.com>

	 Ligador relocador da arquitetura hipotética
    vista em aula, recebe como argumentos os 
    arquivos a serem ligados provenientes
    do montador. O formato do arquivo está descrito 
    em "Definição do arquivo objeto.txt".
    
    Gera como saída um arquivo para o
    carregador absoluto que está descrito em
    "Definição do arquivo executável.txt".

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

Terros *tabelaErros;
Terros *ultimoErro;

main(int argc, char *argv[])
{
   short int   i, ok;          // Auxiliares
   char argumento[50][argc-1];
   if (argc==1)
   {
      printf("\nNenhum argumento passado.\n");
      return;
   }
// Executa a 1ª passagem
   for (i = 1; i < argc; i++)
   {
      strcpy(argumento[i], argv[i]);
      strcat(argumento[i], ".txt");
      #ifdef DEBUG
      printf("\nAbrindo %s\n",argumento[i]);
      #endif
      ok = carregar(i, argumento[i]);
      #ifdef DEBUG
      printf("\n\tOK = %d\n",ok);
      #endif
      if(!ok)
         printf("\n\tFUDEU o %s\n", argumento[i]);
   }
   tabelaUso[auxTabelaUso+1].endSimbolo = -1;
// Verifica a existência de símbolos indefinidos
   simbIndef();
// Executa a 2ª passagem
   #ifdef DEBUG
   system("pause");
   #endif
   if(tabelaErros != NULL)
   {
      imprimeErros();
      return;
   }
   ok = salvar(argv[1]);
   if(!ok)
      printf("\n\tFUDEU enquanto salvava\n");   
   #ifdef DEBUG
   system("pause");
   #endif
}
