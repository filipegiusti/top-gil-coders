/*******************************************************************************
  UNIVERSIDADE FEDERAL DE PELOTAS
  Departamento de Inform�tica
  Curso de Bacharelado em Ci�ncia da Computa��o
  
  SISTEMA MONTADOR-LIGADOR-CARREGADOR PARA UMA ARQUITETURA HIPOT�TICA
  Parte 1: MONTADOR

  Trabalho pr�tico elaborado na disciplina de Projetos em Computa��o I
  Professor: Gil Carlos Rodrigues Medeiros
  
  Autores: Bruno Machado
           Guilherme Corr�a
  
  Data: 06/03/2007
  �ltima altera��o: 24/02/2007
*******************************************************************************/

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
   segunda_passagem();
   system("PAUSE");
   }

int primeira_passagem(FILE *arquivo)
   {
   char label[8];
   char operacao[6];
   char op1[8];
   char op2[8];
   char comment[40];
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';
   comment[0] = '\0';

   int line, leu, id_op;
   leu = 1;
   line = 0;
   Tsimbolos *pos;
   
   while(leu)                                                    // Enquanto o parser obter uma linha do arquivo fonte, faz o processamento.
      {
      leu = parser(arquivo, label, operacao, op1, op2, comment); // Chama o parser. Se a leitura for bem-sucedida leu = 1, sen�o leu = 0.
      if(!leu)
         return 0;                                               // Terminou o arquivo.
      line++; 
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         if(pesquisa_Tsimbolos(label))                           // Verifica se o s�mbolo j� est� na tabela de s�mbolos (retorna o endere�o).
            coloca_Terros(1, line, lcounter, label);             // S�mbolo multidefinido!!
         else                                                    // Se o s�mbolo ainda n�o est� na tabela de s�mbolos...
            {
            pos = coloca_Tsimbolos(label);                       // Coloca o s�mbolo na tabela e salva o endere�o.
            coloca_Tendereco(lcounter, pos);                     // Coloca o endere�o que a label representa na tabela.
            }
         } 
      if(operacao[0] != '\0')                                    // Verifica se h� alguma instru��o especificada na linha.
         {
         id_op = verifica_operacao(operacao);                    // Verifica a validade da opera��o (se est� na tabela de instru��es).
         if(!id_op)
            coloca_Terros(2, line, lcounter, operacao);          // Coloca informa��o de opera��o desconhecida na tabela de erros.
         } 
      if(op1[0] != '\0')                                         // Verifica se h� operando(s).         
         {
         if((op1[0] != '@') && !(numero(op1[0])))                // Se o operando n�o � um literal nem um endere�o (n�o-s�mbolo)...
            {
            pos = pesquisa_Tsimbolos(op1);                       // Pesquisa o s�mbolo na tabela e devolve a posi��o.
            if(pos == NULL)
               coloca_Tsimbolos(op1);                            // Coloca o s�mbolo na tabela de s�mbolos.
            }
         else if(op1[0] == '@')                                  // Caso o operando seja um literal...
            {
            coloca_pool(op1);                                    // Coloca o s�mbolo na tabela de literais.
            }
         if(op2[0] != '\0')                                      // Verifica se h� um segundo operando.
            if(!numero(op2[0]))                                  // Se o segundo operando n�o � um endere�o (n�o-s�mbolo)...
               {
               pos = pesquisa_Tsimbolos(op1);                    // Pesquisa o s�mbolo na tabela e devolve a posi��o.
               if(pos == NULL)
                  coloca_Tsimbolos(op1);                         // Coloca o s�mbolo na tabela de s�mbolos.
               }
         }
      atualiza_lcounter(id_op);                                  // Incrementa o location counter, dependendo do length da opera��o.
      label[0] = '\0';                                           // Limpa os registros de label, opera��o, operadores e coment�rio.
      operacao[0] = '\0';
      op1[0] = '\0';
      op2[0] = '\0';
      comment[0] = '\0';      
      }      
   }


void atualiza_lcounter(int id_op)
   {
   lcounter = Tinstrucoes[id_op].length;
   }

int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2, char *comment)
   {
   char linha[80];
   if(fgets(linha, 80, arquivo))
      {
      if(linha[0] == ' ')
         {
         sscanf(linha, "%s %s %s %s", operacao, op1, op2, comment);
         printf("operacao: %s | op1: %s | op2: %s | comment: %s\n", operacao, op1, op2, comment);
         }
      else if(linha[0] != '#')
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

void coloca_pool(char *op)
   {
   // aguardando fun��o da Francine
   }

int segunda_passagem()
   {
   // come�arei em breve... :)
   }
