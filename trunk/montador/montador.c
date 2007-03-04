/*
	Sistema de Programa��o para um computador hipot�tico
    Parte 1: Montador
            
	Projetos em Computa��o 1 2006/2

	Autores:
        Guilherme Corr�a <>
		Filipe Giusti <filipegiusti@gmail.com>

	Montador da arquitetura hipot�tica
    vista em aula, realiza a montagem do
    arquivo "MASMAPRG.ASM" proveniente
    do processador de macros. O formato 
    do arquivo est� descrito em 
    "".
    
    Gera como sa�da um arquivo para o
    ligador relocador que est� descrito em
    "Defini��o do arquivo objeto.txt".

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "montador.h"
#include "tabelas.h"
#define ARQUIVO "MASMAPRG.ASM"

int main()
   {
   // Abre arquivo para leitura
   FILE *arquivo;
   if(!(arquivo = fopen(ARQUIVO, "r")))
      {
      exit;
      }
   inicializa_tabelas(); 
   primeira_passagem(arquivo);
   imprime_tabelas();
// TODO: verificar se tem erros, imprimir e parar se tiver.   
   segunda_passagem();
   system("PAUSE");
   }

int primeira_passagem(FILE *arquivo)
   {
   char label[9];
   char operacao[7];
   char op1[9];
   char op2[9];
   char comment[81]; // TODO: Ignorar comment
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';
   comment[0] = '\0';

   int line, leu, id_op;
   leu = 1;
   line = 0;
   Tsimbolos *pos;
   Tdefinicoes *pos_def;
   Tusos *pos_uso;
   
// Enquanto o parser obter uma linha do arquivo fonte, faz o processamento.
   while(leu)
      {
      leu = parser(arquivo, label, operacao, op1, op2, comment);
      if(!leu)
         return 0;
      line++;
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         // TODO: Verificar se label eh v�lido.
         coloca_Tsimbolos_label(label, lcounter, line);
         pos_def = pesquisa_Tdef(label);
         if(pos_def)
            coloca_Tdef_end(lcounter, pos_def);
         if(operacao[0] == '\0')                                 // N�o pode haver linha com label e sem instru��o.
            coloca_Terros(666, line, lcounter, label);
         }
      if(operacao[0] != '\0')                                    // Verifica se h� alguma instru��o especificada na linha.
         {
         id_op = verifica_operacao(operacao);
         if(id_op == -1)
            coloca_Terros(2, line, lcounter, operacao);
         else if(id_op == 19)                                    // Caso seja EXTR coloca o s�mbolo na tabela de usos.
            {
            coloca_Tusos(label);
            pesquisa_Tsimbolos(label)->reloc = 0;
            }
         }
         // TODO: Verificar n� de operandos.
      if(op1[0] != '\0')                                         // Verifica se h� operando(s).         
         {
         if((op1[0] != '@') && !(numero(op1[0])))                // Se o operando n�o � um literal nem um endere�o (n�o-s�mbolo)...
            {
            pos_uso = pesquisa_Tusos(op1);
            if(pos_uso)
               coloca_Tusos_end(lcounter, pos_uso);
            if(pesquisa_Tsimbolos(op1) == NULL)
               coloca_Tsimbolos_operando(op1);
            if(id_op == 18)                                      // Caso seja uma declara��o de s�mbolo global (EXTDEF � 18).
               coloca_Tdef(op1);
            }
         else if(op1[0] == '@')                                  // Verifica se o operando � um literal.
            coloca_pool(op1);
         if(op2[0] != '\0')                                      // Verifica se h� um segundo operando.
            if(!numero(op2[0]))                                  // Se o segundo operando n�o � um endere�o (n�o-s�mbolo)...
               {
               pos_uso = pesquisa_Tusos(op1);
               if(pos_uso)
                  coloca_Tusos_end(lcounter, pos_uso);
               if(pesquisa_Tsimbolos(op1) == NULL)
                  coloca_Tsimbolos_operando(op1);
               }
         }
      lcounter += Tinstrucoes[id_op].length;                     // Incrementa o location counter, dependendo do length da opera��o.
      label[0] = '\0';                                           // Limpa os registros de label, opera��o, operadores e coment�rio.
      operacao[0] = '\0';
      op1[0] = '\0';
      op2[0] = '\0';
      comment[0] = '\0';     
      }      
   }

int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2, char *comment)
   {
   char linha[80];
   // TODO: Trocar o fgets.
   if(fgets(linha, 80, arquivo))
      {
      if(linha[0] == ' ')
         {
         sscanf(linha, "%s %s %s %s", operacao, op1, op2, comment);
         printf("operacao: %s | op1: %s | op2: %s | comment: %s\n", operacao, op1, op2, comment);
         }
      else if(linha[0] != '*')
         {
         sscanf(linha, "%s %s %s %s %s", label, operacao, op1, op2, comment);
         printf("label: %s | operacao: %s | op1: %s | op2: %s | comment: %s\n", label, operacao, op1, op2, comment);
         }
         else
            {
            sscanf(linha, "%s", comment);
            }
      return 1;
      }
   else return 0;
   }

int numero(char letra)
   {
   // aguardando fun��o da Francine
   }

// Coloca o op na tabela de literais.
void coloca_pool(char *op)
   {
   // aguardando fun��o da Francine
   }

int segunda_passagem()
   {
   // come�arei em breve... :)
   }
