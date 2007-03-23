#include <stdio.h>

#define TAM_TAB 10

struct tabelaSimbolosGlobal
{
   char nome[9];
   int end;
   char infoReloc;
};
typedef struct tabelaSimbolosGlobal tabelaSimbolos;

struct tabelaSimbolosUso
{
   char nome[9];
   int endSimbolo;
   char sinal;
};
typedef struct tabelaSimbolosUso tabelaUsos;

struct codigoRelocacao
{
   short int var[16];
   short int infoReloc;
};
typedef struct codigoRelocacao codigoReloc;



int main ()
{
   FILE * pFile;
   int tamCodigo = 8;
   int tamPilha = 10;
   int endInicial = 6;
   tabelaSimbolos tabelaSG[TAM_TAB];
   tabelaUsos tabelaUso[TAM_TAB];
   codigoReloc codigo[100*TAM_TAB];
   
   strcpy(tabelaSG[0].nome, "Var01");
   tabelaSG[0].end = 4;
   tabelaSG[0].infoReloc = 'r';
   strcpy(tabelaSG[1].nome, "Var02");
   tabelaSG[1].end = 5;
   tabelaSG[1].infoReloc = 'r';
   strcpy(tabelaSG[2].nome, "Var03");
   tabelaSG[2].end = 6;
   tabelaSG[2].infoReloc = 'r';
   strcpy(tabelaSG[3].nome, "");
   tabelaSG[3].end = 0;
   tabelaSG[3].infoReloc = ' ';
   
   strcpy(tabelaUso[0].nome, "Uso01");
   tabelaUso[0].endSimbolo = 6;
   tabelaUso[0].sinal = 1;
   strcpy(tabelaUso[1].nome, "Uso02");
   tabelaUso[1].endSimbolo = 7;
   tabelaUso[1].sinal = 1;
   strcpy(tabelaUso[2].nome, "");
   tabelaUso[2].endSimbolo = 0;
   tabelaUso[2].sinal = ' ';

   strcpy(tabelaUso[0].nome, "");
   tabelaUso[0].endSimbolo = 0;
   tabelaUso[0].sinal = ' ';     
   
   codigo[0].var[0] = 1;
   codigo[0].var[1] = 2;
   codigo[0].var[2] = 3;
   codigo[0].var[3] = 4;
   codigo[0].var[4] = 5;
   codigo[0].var[5] = 6;
   codigo[0].var[6] = 7;
   codigo[0].var[7] = 8;
   codigo[0].var[8] = 42;
   codigo[0].var[9] = 7;
   codigo[0].var[10] = 41;
   codigo[0].var[11] = 8;
   codigo[0].var[12] = 41;
   codigo[0].var[13] = 11;
   codigo[0].var[14] = 1;
   codigo[0].var[15] = 16;
   codigo[0].infoReloc = 0;
   
   pFile = fopen ( "teste.txt" , "wb" );
   
   fwrite (&tamCodigo , sizeof(int) , 1 , pFile );
   fwrite (&tamPilha , sizeof(int) , 1 , pFile );
   fwrite (&endInicial , sizeof(int) , 1 , pFile );
   
   fwrite (tabelaSG , sizeof(tabelaSimbolos) , 4 , pFile );
   fwrite (tabelaUso , sizeof(tabelaUsos) , 1 , pFile );
   fwrite (codigo , sizeof(codigoReloc) , 1 , pFile );
   
   fclose (pFile);
   
/***********************************************************************/ 
   tamCodigo = 4;
   tamPilha = 25;
   endInicial = 2;
   
   strcpy(tabelaSG[0].nome, "Uso01");
   tabelaSG[0].end = 0;
   tabelaSG[0].infoReloc = 'r';
   strcpy(tabelaSG[1].nome, "Uso02");
   tabelaSG[1].end = 1;
   tabelaSG[1].infoReloc = 'r';
   strcpy(tabelaSG[2].nome, "");
   tabelaSG[2].end = 0;
   tabelaSG[2].infoReloc = ' ';
   
   strcpy(tabelaUso[0].nome, "Var01");
   tabelaUso[0].endSimbolo = 2;
   tabelaUso[0].sinal = 1;
   strcpy(tabelaUso[1].nome, "Var02");
   tabelaUso[1].endSimbolo = 3;
   tabelaUso[1].sinal = 1;
   strcpy(tabelaUso[2].nome, "");
   tabelaUso[2].endSimbolo = 0;
   tabelaUso[2].sinal = ' ';
   
   strcpy(tabelaUso[0].nome, "");
   tabelaUso[0].endSimbolo = 0;
   tabelaUso[0].sinal = ' ';   
   
   codigo[0].var[0] = 11;
   codigo[0].var[1] = 12;
   codigo[0].var[2] = 13;
   codigo[0].var[3] = 14;
   codigo[0].var[4] = 40;
   codigo[0].var[5] = 3;
   codigo[0].var[6] = 40;
   codigo[0].var[7] = 15;
   codigo[0].var[8] = 42;
   codigo[0].var[9] = 7;
   codigo[0].var[10] = 41;
   codigo[0].var[11] = 8;
   codigo[0].var[12] = 41;
   codigo[0].var[13] = 11;
   codigo[0].var[14] = 1;
   codigo[0].var[15] = 00;
   codigo[0].infoReloc = 0;
   
   pFile = fopen ( "testa.txt" , "wb" );
   
   fwrite (&tamCodigo , sizeof(int) , 1 , pFile );
   fwrite (&tamPilha , sizeof(int) , 1 , pFile );
   fwrite (&endInicial , sizeof(int) , 1 , pFile );
   
   fwrite (tabelaSG , sizeof(tabelaSimbolos) , 3 , pFile );
   fwrite (tabelaUso , sizeof(tabelaUsos) , 1 , pFile );
   fwrite (codigo , sizeof(codigoReloc) , 1 , pFile );
   
   fclose (pFile);  
   
   return 0;
}
