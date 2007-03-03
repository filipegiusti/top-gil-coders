/*
	Sistema de Programação para um computador hipotético
    Parte 1: Montador
            
	Projetos em Computação 1 2006/2

	Autores:
        Guilherme Corrêa <>
		Filipe Giusti <filipegiusti@gmail.com>

	Montador da arquitetura hipotética
    vista em aula, realiza a montagem do
    arquivo "MASMAPRG.ASM" proveniente
    do processador de macros. O formato 
    do arquivo está descrito em 
    "".
    
    Gera como saída um arquivo para o
    ligador relocador que está descrito em
    "Definição do arquivo objeto.txt".

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
      leu = parser(arquivo, label, operacao, op1, op2, comment); // Chama o parser. Se a leitura for bem-sucedida leu = 1, senão leu = 0.
      if(!leu)
         return 0;                                               // Terminou o arquivo.
      line++; 
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         if(pesquisa_Tsimbolos(label))                           // Verifica se o símbolo já está na tabela de símbolos (retorna o endereço).
            coloca_Terros(1, line, lcounter, label);             // Símbolo multidefinido na tabela de símbolos!!
         else                                                    // Se o símbolo ainda não está na tabela de símbolos...
            {
            pos = coloca_Tsimbolos(label);                       // Coloca o símbolo na tabela e salva o endereço.
            coloca_Tendereco(lcounter, pos);                     // Coloca o endereço que a label representa na tabela.
            }
         pos_def = pesquisa_Tdef(label);                         // Verifica se o símbolo está na tabela de definições.
         if(pos_def)
            if(!coloca_Tdef_end(lcounter, pos_def))              // Coloca o endereço correspondente ao símbolo na tabela de definições.
               coloca_Terros(4, line, lcounter, label);          // Se já existe um endereço na tab. de definições para este símbolo, informa o erro.            
         } 
      if(operacao[0] != '\0')                                    // Verifica se há alguma instrução especificada na linha.
         {
         id_op = verifica_operacao(operacao);                    // Verifica a validade da operação (se está na tabela de instruções).
         if(!id_op)
            coloca_Terros(2, line, lcounter, operacao);          // Coloca informação de operação desconhecida na tabela de erros.
         if(id_op == 19)                                         // Caso seja a utilização de um símbolo de outro módulo...
            {
            coloca_Tusos(label);                                  // Coloca o símbolo na tabela de usos.
            }
         } 
      if(op1[0] != '\0')                                         // Verifica se há operando(s).         
         {
         if((op1[0] != '@') && !(numero(op1[0])))                // Se o operando não é um literal nem um endereço (não-símbolo)...
            {
            pos_uso = pesquisa_Tusos(op1);                       // Pesquisa o símbolo na tabela de usos.
            if(pos_uso)                                          // Se o símbolo já está declarado na tabela de usos...
               {
               if(!coloca_Tusos_end(lcounter, pos_uso))          // Tenta colocar só endereço na sua entrada.
                  coloca_Tusos_novo(lcounter, op1);              // Se o endereço já está definido, então esta é uma segunda entrada na tabeça de usos.
               }
            pos = pesquisa_Tsimbolos(op1);                       // Pesquisa o símbolo na tabela e devolve a posição.
            if(pos == NULL)
               coloca_Tsimbolos(op1);                            // Coloca o símbolo na tabela de símbolos.
            if(id_op == 17)                                      // Caso seja uma declaração de símbolo global (EXTDEF é 17).
               {
               pos_def = pesquisa_Tdef(op1);                     // Pesquisa o símbolo na tabela de definições
               if(pos_def == NULL)                               // Se é um símbolo que ainda não foi definido como global...
                  coloca_Tdef(op1);                              // define.
               }
            else                                                 // Senão...
               {
               coloca_Terros(3, line, lcounter+1, op1);          // Coloca a informação de símbolo global multidefinido na tabela de definições.
               }
            }
         else if(op1[0] == '@')                                  // Caso o operando seja um literal...
            {
            coloca_pool(op1);                                    // Coloca o símbolo na tabela de literais.
            }
         if(op2[0] != '\0')                                      // Verifica se há um segundo operando.
            if(!numero(op2[0]))                                  // Se o segundo operando não é um endereço (não-símbolo)...
               {
               pos_uso = pesquisa_Tusos(op1);                       // Pesquisa o símbolo na tabela de usos.
               if(pos_uso)                                          // Se o símbolo já está declarado na tabela de usos...
                  {
                  if(!coloca_Tusos_end(lcounter, pos_uso))          // Tenta colocar só endereço na sua entrada.
                     coloca_Tusos_novo(lcounter, op1);              // Se o endereço já está definido, então esta é uma segunda entrada na tabeça de usos.
                  }
               pos = pesquisa_Tsimbolos(op1);                    // Pesquisa o símbolo na tabela e devolve a posição.
               if(pos == NULL)
                  coloca_Tsimbolos(op1);                         // Coloca o símbolo na tabela de símbolos.
               }
         }
      atualiza_lcounter(id_op);                                  // Incrementa o location counter, dependendo do length da operação.
      label[0] = '\0';                                           // Limpa os registros de label, operação, operadores e comentário.
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
   // aguardando função da Francine
   }

void coloca_pool(char *op)
   {
   // aguardando função da Francine
   }

int segunda_passagem()
   {
   // começarei em breve... :)
   }
