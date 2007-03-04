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
         // TODO: Verificar se label eh válido.
         coloca_Tsimbolos_label(label, lcounter, line);
         pos_def = pesquisa_Tdef(label);
         if(pos_def)
            coloca_Tdef_end(lcounter, pos_def);
         if(operacao[0] == '\0')                                 // Não pode haver linha com label e sem instrução.
            coloca_Terros(666, line, lcounter, label);
         }
      if(operacao[0] != '\0')                                    // Verifica se há alguma instrução especificada na linha.
         {
         id_op = verifica_operacao(operacao);
         if(id_op == -1)
            coloca_Terros(2, line, lcounter, operacao);
         else if(id_op == 19)                                    // Caso seja EXTR coloca o símbolo na tabela de usos.
            {
            coloca_Tusos(label);
            pesquisa_Tsimbolos(label)->reloc = 0;
            }
         }
         // TODO: Verificar nº de operandos.
      if(op1[0] != '\0')                                         // Verifica se há operando(s).         
         {
         if((op1[0] != '@') && !(numero(op1[0])))                // Se o operando não é um literal nem um endereço (não-símbolo)...
            {
            pos_uso = pesquisa_Tusos(op1);
            if(pos_uso)
               coloca_Tusos_end(lcounter, pos_uso);
            if(pesquisa_Tsimbolos(op1) == NULL)
               coloca_Tsimbolos_operando(op1);
            if(id_op == 18)                                      // Caso seja uma declaração de símbolo global (EXTDEF é 18).
               coloca_Tdef(op1);
            }
         else if(op1[0] == '@')                                  // Verifica se o operando é um literal.
            coloca_pool(op1);
         if(op2[0] != '\0')                                      // Verifica se há um segundo operando.
            if(!numero(op2[0]))                                  // Se o segundo operando não é um endereço (não-símbolo)...
               {
               pos_uso = pesquisa_Tusos(op1);
               if(pos_uso)
                  coloca_Tusos_end(lcounter, pos_uso);
               if(pesquisa_Tsimbolos(op1) == NULL)
                  coloca_Tsimbolos_operando(op1);
               }
         }
      lcounter += Tinstrucoes[id_op].length;                     // Incrementa o location counter, dependendo do length da operação.
      label[0] = '\0';                                           // Limpa os registros de label, operação, operadores e comentário.
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
   // aguardando função da Francine
   }

// Coloca o op na tabela de literais.
void coloca_pool(char *op)
   {
   // aguardando função da Francine
   }

int segunda_passagem()
   {
   // começarei em breve... :)
   }
