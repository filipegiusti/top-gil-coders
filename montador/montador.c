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
   segunda_passagem();
   system("PAUSE");
   }

int primeira_passagem(FILE *arquivo)
   {
   char label[9];
   char operacao[7];
   char op1[9];
   char op2[9];
   char comment[81];
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
   
   while(leu)                                                    // Enquanto o parser obter uma linha do arquivo fonte, faz o processamento.
      {
      leu = parser(arquivo, label, operacao, op1, op2, comment); // Chama o parser. Se a leitura for bem-sucedida leu = 1, sen�o leu = 0.
      if(!leu)
         return 0;                                               // Terminou o arquivo.
      line++; 
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         if(pesquisa_Tsimbolos(label))                           // Verifica se o s�mbolo j� est� na tabela de s�mbolos (retorna o endere�o).
            coloca_Terros(1, line, lcounter, label);             // S�mbolo multidefinido na tabela de s�mbolos!!
         else                                                    // Se o s�mbolo ainda n�o est� na tabela de s�mbolos...
            {
            pos = coloca_Tsimbolos(label);                       // Coloca o s�mbolo na tabela e salva o endere�o.
            coloca_Tendereco(lcounter, pos);                     // Coloca o endere�o que a label representa na tabela.
            }
         pos_def = pesquisa_Tdef(label);                         // Verifica se o s�mbolo est� na tabela de defini��es.
         if(pos_def)
            if(!coloca_Tdef_end(lcounter, pos_def))              // Coloca o endere�o correspondente ao s�mbolo na tabela de defini��es.
               coloca_Terros(4, line, lcounter, label);          // Se j� existe um endere�o na tab. de defini��es para este s�mbolo, informa o erro.            
         } 
      if(operacao[0] != '\0')                                    // Verifica se h� alguma instru��o especificada na linha.
         {
         id_op = verifica_operacao(operacao);                    // Verifica a validade da opera��o (se est� na tabela de instru��es).
         if(!id_op)
            coloca_Terros(2, line, lcounter, operacao);          // Coloca informa��o de opera��o desconhecida na tabela de erros.
         if(id_op == 19)                                         // Caso seja a utiliza��o de um s�mbolo de outro m�dulo...
            {
            coloca_Tusos(label);                                  // Coloca o s�mbolo na tabela de usos.
            }
         } 
      if(op1[0] != '\0')                                         // Verifica se h� operando(s).         
         {
         if((op1[0] != '@') && !(numero(op1[0])))                // Se o operando n�o � um literal nem um endere�o (n�o-s�mbolo)...
            {
            pos_uso = pesquisa_Tusos(op1);                       // Pesquisa o s�mbolo na tabela de usos.
            if(pos_uso)                                          // Se o s�mbolo j� est� declarado na tabela de usos...
               {
               if(!coloca_Tusos_end(lcounter, pos_uso))          // Tenta colocar s� endere�o na sua entrada.
                  coloca_Tusos_novo(lcounter, op1);              // Se o endere�o j� est� definido, ent�o esta � uma segunda entrada na tabe�a de usos.
               }
            pos = pesquisa_Tsimbolos(op1);                       // Pesquisa o s�mbolo na tabela e devolve a posi��o.
            if(pos == NULL)
               coloca_Tsimbolos(op1);                            // Coloca o s�mbolo na tabela de s�mbolos.
            if(id_op == 17)                                      // Caso seja uma declara��o de s�mbolo global (EXTDEF � 17).
               {
               pos_def = pesquisa_Tdef(op1);                     // Pesquisa o s�mbolo na tabela de defini��es
               if(pos_def == NULL)                               // Se � um s�mbolo que ainda n�o foi definido como global...
                  coloca_Tdef(op1);                              // define.
               }
            else                                                 // Sen�o...
               {
               coloca_Terros(3, line, lcounter+1, op1);          // Coloca a informa��o de s�mbolo global multidefinido na tabela de defini��es.
               }
            }
         else if(op1[0] == '@')                                  // Caso o operando seja um literal...
            {
            coloca_pool(op1);                                    // Coloca o s�mbolo na tabela de literais.
            }
         if(op2[0] != '\0')                                      // Verifica se h� um segundo operando.
            if(!numero(op2[0]))                                  // Se o segundo operando n�o � um endere�o (n�o-s�mbolo)...
               {
               pos_uso = pesquisa_Tusos(op1);                       // Pesquisa o s�mbolo na tabela de usos.
               if(pos_uso)                                          // Se o s�mbolo j� est� declarado na tabela de usos...
                  {
                  if(!coloca_Tusos_end(lcounter, pos_uso))          // Tenta colocar s� endere�o na sua entrada.
                     coloca_Tusos_novo(lcounter, op1);              // Se o endere�o j� est� definido, ent�o esta � uma segunda entrada na tabe�a de usos.
                  }
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

void coloca_pool(char *op)
   {
   // aguardando fun��o da Francine
   }

int segunda_passagem()
   {
   // come�arei em breve... :)
   }
