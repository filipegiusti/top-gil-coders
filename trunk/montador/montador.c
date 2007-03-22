/*
	Sistema de Programa��o para um computador hipot�tico
    Parte 1: Montador
            
	Projetos em Computa��o 1 2006/2

	Autores:
      Guilherme Corr�a <gcorrea@gmail.com>
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
#include <ctype.h>
#include "montador.h"
#include "tabelas.h"
#define ARQUIVO "MASMAPRG.ASM"
#define SAIDA "OUTPUT"
#define DEBUG

int main()
   {
   //TODO: Arrumar erro quando coment�rio na 1� linha
   //TODO: Arrumar erro quando coment�rio em outras linhas
   //TODO: suporte a literais
   //TODO: Suporte a endere�amento imediato e indireto
   //TODO: Escrever info reloc direito
   //TODO: Tamanho label/opera��o/operandos maior que o permitido   
   inicializa_tabelas(); 
   primeira_passagem();   
#ifdef DEBUG
   imprime_tabelas();
#endif
   if(imprime_erros())
      {
      system("PAUSE");
      return;
      }
   segunda_passagem();
   system("PAUSE");
   }

int primeira_passagem()
   {
   FILE *arquivo;
   char label[9];
   char operacao[7];
   char op1[9];
   char op2[9];
   
   // inicializa��o de vari�veis
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';

   int line, id_op, num_op, end;
   line = 0;
   num_op = 0;
   end = 0;
   Tsimbolos *pos;
   TdefinicoesEncapsulado *pos_def;
   TusosEncapsulado *pos_uso;
   
   if(!(arquivo = fopen(ARQUIVO, "r")))
      {
      exit;
      }
      
// Enquanto o parser obter uma linha do arquivo fonte, faz o processamento.
   while(parser(arquivo, label, operacao, op1, op2))
      {
      line++;
      if(label[0] == '*')
         continue;
      if((end == 1) && ((label[0] != '\0') || (operacao[0] != '\0') || (op1[0] != '\0') || (op1[0] != '\0')))  // N�o pode haver c�digo ap�s o END.
         coloca_Terros(11, line, lcounter, operacao);
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         if(!isalpha(label[0]))
            coloca_Terros(4, line, lcounter, label);
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
         else if(id_op == 17)
            end = 1;
         }
      if(op1[0] != '\0')                                         // Verifica se h� operando(s).         
         {
         num_op++;
         if(id_op == 21)
            {
            if(isdigit(op1[0]))                    // Entra se for um STACK com operando certo.
               tam_pilha = atoi(op1);
            else
               coloca_Terros(7, line, lcounter, op1);
            }
         if((isdigit(op1[0]) || isalpha(op1[0])) && (id_op == 22))  // Entra se for um START com operando certo.
            start = atoi(op1);
         if((op1[0] != '@') && !(isdigit(op1[0])))                   // S� entra se for label.
            {
            pos_uso = pesquisa_Tusos(op1);
            if(pos_uso)
               coloca_Tusos_end(lcounter, pos_uso);
            if(pesquisa_Tsimbolos(op1) == NULL)
               coloca_Tsimbolos_operando(op1, line);
            if(id_op == 18)                                      // Caso seja uma declara��o de s�mbolo global (EXTDEF � 18).
               coloca_Tdef(op1);
            }
         else if(op1[0] == '@')                                  // Verifica se o operando � um literal.
            coloca_pool(op1);
         if(op2[0] != '\0')                                      // Verifica se h� um segundo operando.
            {
            num_op++;
            if((op2[0] != '@') && !(isdigit(op2[0])))            // Entra se for label.   
               {
               pos_uso = pesquisa_Tusos(op2);
               if(pos_uso)
                  coloca_Tusos_end(lcounter, pos_uso);
               if(pesquisa_Tsimbolos(op2) == NULL)
                  coloca_Tsimbolos_operando(op2, line);
               }   
            }
         }
      if(num_op != Tinstrucoes[id_op].num_operandos)             // N�mero de operandos incorreto.
         coloca_Terros(3, line, lcounter, operacao);
      lcounter += Tinstrucoes[id_op].length;                     // Incrementa o location counter, dependendo do length da opera��o.
      label[0] = '\0';                                           // Limpa os registros de label, opera��o, operadores e coment�rio.
      operacao[0] = '\0';
      op1[0] = '\0';
      op2[0] = '\0';
      num_op = 0;
      }
   if(tam_pilha == -1)                       // O tamanho da pilha deve ser definido ou h� um erro.
      coloca_Terros(8, -1, -1, "STACK");
   if(start == -1)                           // O endere�o de in�cio da execu��o deve ser definido ou h� um erro.
      coloca_Terros(9, -1, -1, "START");
   if(end == 0)                              // O fim do c�digo deve ser definido ou h� um erro.
      coloca_Terros(6, line, lcounter, "END");
   verifica_Tsimbolos();                     // Verifica se h� s�mbolos indefinidos na tabela de s�mbolos.
   fclose(arquivo);
   return 0;
   }

int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2)
   {
   char linha[80];
   // TODO: Trocar o fgets.
   if(fgets(linha, 80, arquivo))
      {
      if(linha[0] == ' ' || linha[0] == '\t')                   //Verifica se come�o de linha com espa�o em braco ou tabula��o
         {
         sscanf(linha, "%s %s %s", operacao, op1, op2);
         if((op1[0] == '#'))
            op1[0] = '\0';
         if((op2[0] == '#'))
            op2[0] = '\0';
#ifdef DEBUG            
         printf("operacao: %s | op1: %s | op2: %s\n", operacao, op1, op2);
#endif
         }
      else if(linha[0] != '*')
         {
         sscanf(linha, "%s %s %s %s", label, operacao, op1, op2);
         if((op1[0] == '#'))
            op1[0] = '\0';
         if((op2[0] == '#'))
            op2[0] = '\0';
#ifdef DEBUG            
         printf("label: %s | operacao: %s | op1: %s | op2: %s\n", label, operacao, op1, op2);
#endif        
         }
      return 1;
      }
   else return 0;
   }

// Coloca o op na tabela de literais.
void coloca_pool(char *op)
   {
   // aguardando fun��o da Francine...
   }

int segunda_passagem()
   {
   FILE *saida;
   FILE *entrada;
   TdefinicoesEncapsulado *aux_de;
   TusosEncapsulado *aux_usos;
   short int info_reloc, operacao_num, op1_num, op2_num;
   int i, id_op;
   char label[9];
   char operacao[7];
   char op1[9];
   char op2[9];
   
   // inicializa��o de vari�veis
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';
   i = 0;
      
   if(!(saida = fopen(SAIDA, "wb")))
      return 0;
   if(!(entrada = fopen(ARQUIVO, "r")))
      return 0;
   fwrite(&lcounter, sizeof(int), 1, saida);
   fwrite(&tam_pilha, sizeof(int), 1, saida);    
   fwrite(&start, sizeof(int), 1, saida);
   
   // Escreve no arquivo a tabela de defini��es   
   for(aux_de = Tab_def; aux_de; aux_de = aux_de->prox)
      fwrite(&(aux_de->info), sizeof(Tdefinicoes), 1, saida);
   aux_de = malloc(sizeof(Tdefinicoes));           // cria separador
   aux_de->info.nome[0] = '\0';
   aux_de->info.endereco = 0;
   aux_de->info.sinal = 0;
   fwrite(&(aux_de->info), sizeof(Tdefinicoes), 1, saida);  // imprime separador
   
   // Escreve no arquivo a tabela de usos
   for(aux_usos = Tab_usos; aux_usos; aux_usos = aux_usos->prox)
      fwrite(&(aux_usos->info), sizeof(Tusos), 1, saida);
   aux_usos = malloc(sizeof(Tusos));           // cria separador
   aux_usos->info.nome[0] = '\0';
   aux_usos->info.endereco = 0;
   aux_usos->info.reloc = 0;
   fwrite(&(aux_usos->info), sizeof(Tusos), 1, saida);  // imprime separador
      
   while(parser(entrada, label, operacao, op1, op2))
      {
         id_op = verifica_operacao(operacao);
         if(Tinstrucoes[id_op].length == 0)           // N�o faz nada quando a instru��o � de montador.
            continue;
         if(Tinstrucoes[id_op].length >= 1)           // Entra quando a instru��o tem tamanho 1.
            {
            i++;
            info_reloc <<= 1;
            if(id_op == 16)                           // Entra se a instru��o � um CONST.
               {
               op1_num = atoi(op1);
               fwrite(&op1_num, sizeof(short int), 1, saida);
               continue;
               }
            if(id_op == 20)                           // Entra se a instru��o � um SPACE.
               {
               op1_num = 0;
               fwrite(&op1_num, sizeof(short int), 1, saida);
               continue;
               }
            operacao_num = Tinstrucoes[id_op].opcode;
            fwrite(&operacao_num, sizeof(short int), 1, saida);
            }
         if(Tinstrucoes[id_op].length >= 2)           // Traduz o primeiro operando.
            {
            i++;
            info_reloc <<= 1;
            if(isalpha(op1[0]))                       // Verifica se o operando � um s�mbolo.
               {
               Tsimbolos *aux;
               aux = pesquisa_Tsimbolos(op1);
               op1_num = aux->endereco;
               if(aux->reloc == 1)
                  info_reloc++;
               }
            else if(isdigit(op1[0]))                  // Verifica se o operando � um endere�o.
               op1_num = atoi(op1);
            //TODO: else if (� literal)               // Verifica se o operando � um literal.
            fwrite(&op1_num, sizeof(short int), 1, saida);
            }
         if(Tinstrucoes[id_op].length == 3)           // Traduz o segundo operando.
            {
            i++;
            info_reloc <<= 1;
            if(isalpha(op2[0]))                       // Verifica se o operando � um s�mbolo.
               {
               Tsimbolos *aux;
               aux = pesquisa_Tsimbolos(op2);
               op2_num = aux->endereco;
               if(aux->reloc == 1)
                  info_reloc++;
               }
            else if(isdigit(op2[0]))                  // Verifica se o operando � um endere�o.
               op2_num = atoi(op2);
            //TODO: else if (� literal)
            fwrite(&op2_num, sizeof(short int), 1, saida);
            }
         // TODO: Realizar esse teste sempre que i++
         if(i == 16)                                  // Escreve a palavra de reloca��o se j� escreveu 16 palavras de c�digo.
            {
            fwrite(&info_reloc, sizeof(short int), 1, saida);
            i = 0;
            }
      }
   //TODO: Preencher o c�digo com 00 at� i ser multiplo de 16 e colocar info info.reloc.
   fclose(entrada);
   fclose(saida);
   }
