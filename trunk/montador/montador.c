/*
	Sistema de Programação para um computador hipotético
    Parte 1: Montador
            
	Projetos em Computação 1 2006/2

	Autores:
      Guilherme Corrêa <gcorrea@gmail.com>
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
#include <ctype.h>
#include "montador.h"
#include "tabelas.h"
//#define DEBUG
#define CRLF "\n"
#define TAM_NOME_ARQUIVO 50

int main(int argc, char *argv[])
   {     
   //TODO: Suporte a endereçamento imediato e indireto
   if (argc != 2)
      {
      printf ("Argumentos invalidos.\nEX: monta teste.asm");
      return 0;
      }
   inicializa_tabelas(); 
   primeira_passagem(argv[1]);   
   if(imprime_erros())
      {
      system("PAUSE");
      return;
      }
   coloca_end_literais(lcounter);
#ifdef DEBUG
   imprime_tabelas();
#endif
   segunda_passagem(argv[1]);
   printf ("Montado com sucesso!");
   }

int primeira_passagem(char *nome_arquivo)
   {
   FILE *arquivo;
   char label[9];
   char operacao[7];
   char op1[9];   //Alterar tamanho afeta outras partes.
   char op2[9];
   
   // inicialização de variáveis
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';

   int line, id_op, num_op, end, i;
   line = 0;
   num_op = 0;
   end = 0;
   Tsimbolos *pos;
   TdefinicoesEncapsulado *pos_def;
   TusosEncapsulado *pos_uso;
   
   if(!(arquivo = fopen(nome_arquivo, "r")))
      {
      exit;
      }
      
// Enquanto o parser obter uma linha do arquivo fonte, faz o processamento.
   while(parser(arquivo, label, operacao, op1, op2))
      {
#ifdef DEBUG            
      printf("label: %s\t| operacao: %s\t| op1: %s\t| op2: %s\n", label, operacao, op1, op2);
#endif             
      line++;
      if(label[0] == '*')
         continue;
      if((end == 1) && ((label[0] != '\0') || (operacao[0] != '\0') || (op1[0] != '\0') || (op1[0] != '\0')))  // Não pode haver código após o END.
         coloca_Terros(11, line, lcounter, operacao);
      if(label[0] != '\0')                                       // Verifica se existe label na linha.
         {
         if(!isalpha(label[0]))
            coloca_Terros(4, line, lcounter, label);
         coloca_Tsimbolos_label(label, lcounter, line);
         pos_def = pesquisa_Tdef(label);
         if(pos_def)
            coloca_Tdef_end(lcounter, pos_def);
         if(operacao[0] == '\0')                                 // Não pode haver linha com label e sem instrução.
            coloca_Terros(13, line, lcounter, label);
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
         else if(id_op == 17)
            end = 1;
         }
      if(op1[0] != '\0')                                         // Verifica se há operando(s).         
         {
         num_op++;
         if(id_op == 21)
            {
            if(isdigit(op1[0]))                    // Entra se for um STACK com operando certo.
               tam_pilha = atoi(op1);
            else
               coloca_Terros(7, line, lcounter+1, op1);
            }
         if((isdigit(op1[0]) || isalpha(op1[0])) && (id_op == 22))  // Entra se for um START com operando certo.
            start = atoi(op1);
         if((op1[0] != '@') && !(isdigit(op1[0])))                   // Só entra se for label.
            {
            pos_uso = pesquisa_Tusos(op1);
            if(pos_uso)
               coloca_Tusos_end(lcounter+1, pos_uso);
            if(pesquisa_Tsimbolos(op1) == NULL)
               coloca_Tsimbolos_operando(op1, line);
            if(id_op == 18)                                      // Caso seja uma declaração de símbolo global (EXTDEF é 18).
               coloca_Tdef(op1);
            }
         else if(op1[0] == '@')                                  // Verifica se o operando é um literal.
            {
            if (verifica_literal(op1))
               coloca_tab_literais(op1, line);
            else
               coloca_Terros(14, line, lcounter, op1);
            }
         if(op2[0] != '\0')                                      // Verifica se há um segundo operando.
            {
            num_op++;
            if((op2[0] != '@') && !(isdigit(op2[0])))            // Entra se for label.   
               {
               pos_uso = pesquisa_Tusos(op2);
               if(pos_uso)
                  coloca_Tusos_end(lcounter+2, pos_uso);
               if(pesquisa_Tsimbolos(op2) == NULL)
                  coloca_Tsimbolos_operando(op2, line);
               }
            else if(op2[0] == '@')
               {
               if (verifica_literal(op2))
                  coloca_tab_literais(op2, line);
               else
                  coloca_Terros(14, line, lcounter, op2);
               }
            }
         }
      if(num_op != Tinstrucoes[id_op].num_operandos && operacao[0] != '\0')             // Número de operandos incorreto.
         coloca_Terros(3, line, lcounter, operacao);
      lcounter += Tinstrucoes[id_op].length;                     // Incrementa o location counter, dependendo do length da operação.
      label[0] = '\0';                                           // Limpa os registros de label, operação, operadores e comentário.
      operacao[0] = '\0';
      op1[0] = '\0';
      op2[0] = '\0';
      num_op = 0;
      }
   if(tam_pilha == -1)                       // O tamanho da pilha deve ser definido ou há um erro.
      coloca_Terros(8, -1, -1, "STACK");
   if(start == -1)                           // O endereço de início da execução deve ser definido ou há um erro.
      coloca_Terros(9, -1, -1, "START");
   if(end == 0)                              // O fim do código deve ser definido ou há um erro.
      coloca_Terros(6, line, lcounter, "END");
   verifica_Tsimbolos();                     // Verifica se há símbolos indefinidos na tabela de símbolos.
   fclose(arquivo);
   return 0;
   }

int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2)
   {
   char linha[81], *aux_linha;
   aux_linha = linha;
   char bobagem[10];        // Ler caracteres a mais que 80
lerdenovo:
   if(fgets(linha, 81, arquivo))
      {
      if (!strstr(linha, CRLF) && !feof(arquivo))
            {
            coloca_Terros(12,0,0,"Nao deu Gil!!");
            while (fgets(bobagem, 10, arquivo))
                {
                if (strstr(linha, CRLF))
                    break;
                }
            }         
      if(linha[0] == '*')
         {
            goto lerdenovo;
         }
      else if(linha[0] != '*' && linha[0] != ' ' && linha[0] != '\t')         //Se tiver label
         {
         sscanf(aux_linha, "%8s", label);
         for ( ; *aux_linha != ' ' && *aux_linha != '\t'; aux_linha++ )
            if (!strcmp(aux_linha,CRLF)  || feof(arquivo))
               {
                  operacao = '\0';
                  op1[0] = '\0';
                  op2[0] = '\0';
                  return 1;
               }
         }
      sscanf(aux_linha, "%*c%6s", operacao);
      for (aux_linha++ ; *aux_linha != ' ' && *aux_linha != '\t' ; aux_linha++ )
         if (!strcmp(aux_linha,CRLF) || feof(arquivo))
            {
               op1[0] = '\0';
               op2[0] = '\0';
               return 1;
            }
      aux_linha++;
      sscanf(aux_linha, "%8s", op1);
      for (aux_linha++ ; *aux_linha != ' ' && *aux_linha != '\t' ; aux_linha++ )
         if (!strcmp(aux_linha, CRLF) || feof(arquivo))
         {
            op2[0] = '\0';
            return 1;
         }
      aux_linha++;
      sscanf(aux_linha, "%8s", op2);             
      if((op1[0] == '*'))
         op1[0] = '\0';
      if((op2[0] == '*'))
         op2[0] = '\0';
      return 1;
      }
   else return 0;
   }

int segunda_passagem(char *nome_arquivo)
   {
   FILE *saida;
   FILE *entrada;
   TdefinicoesEncapsulado *aux_de;
   TusosEncapsulado *aux_usos;
   unsigned short int operacao_num, op1_num, op2_num, info_reloc, reloca = 0;
   int i, id_op;
   char label[9];
   char operacao[7];
   char op1[9];
   char op2[9];
   char nome_saida[TAM_NOME_ARQUIVO], *aux;
   
   // inicialização de variáveis
   label[0] = '\0';
   operacao[0] = '\0';
   op1[0] = '\0';
   op2[0] = '\0';
   i = 0;
   
   // Coloca o .obj
   strcpy(nome_saida, nome_arquivo);
   aux = strchr(nome_saida, '.');
   if (aux)
      {
      aux++;
      strcpy(aux, "obj");
      }
   else
      {
      aux = strchr(nome_saida, '\0');
      strcpy(aux, ".obj");
      }
   
   if(!(saida = fopen(nome_saida, "wb")))
      return 0;
   if(!(entrada = fopen(nome_arquivo, "r")))
      return 0;
   fwrite(&lcounter, sizeof(int), 1, saida);
   fwrite(&tam_pilha, sizeof(int), 1, saida);    
   fwrite(&start, sizeof(int), 1, saida);
   
   // Escreve no arquivo a tabela de definições   
   for(aux_de = Tab_def; aux_de; aux_de = aux_de->prox)
      escreve_definicao(&(aux_de->info), saida);
   aux_de = malloc(sizeof(Tdefinicoes));           // cria separador
   aux_de->info.nome[0] = '\0';
   aux_de->info.endereco = 0;
   aux_de->info.sinal = 0;
   escreve_definicao(&(aux_de->info), saida);  // imprime separador
   
   // Escreve no arquivo a tabela de usos
   for(aux_usos = Tab_usos; aux_usos; aux_usos = aux_usos->prox)
      escreve_uso(&(aux_usos->info), saida);
   aux_usos = malloc(sizeof(Tusos));           // cria separador
   aux_usos->info.nome[0] = '\0';
   aux_usos->info.endereco = 0;
   aux_usos->info.reloc = 0;
   escreve_uso(&(aux_usos->info), saida);  // imprime separador
      
   while(parser(entrada, label, operacao, op1, op2))
      {
#ifdef DEBUG            
         printf("label: %s\t| operacao: %s\t| op1: %s\t| op2: %s\n", label, operacao, op1, op2);
#endif     
         reloca = 0;           
         id_op = verifica_operacao(operacao);
         if(Tinstrucoes[id_op].length == 0)           // Não faz nada quando a instrução é de montador.
            continue;
         if(Tinstrucoes[id_op].length >= 1)           // Entra quando a instrução tem tamanho 1.
            {
            i++;
            info_reloc >>= 1;
            
            if(id_op == 16)                           // Entra se a instrução é um CONST.
               {
               op1_num = atoi(op1);
               fwrite(&op1_num, sizeof(short int), 1, saida);
               if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
                  fwrite(&info_reloc, sizeof(short int), 1, saida);
               continue;
               }
            if(id_op == 20)                           // Entra se a instrução é um SPACE.
               {
               op1_num = 0;
               fwrite(&op1_num, sizeof(short int), 1, saida);
               if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
                  fwrite(&info_reloc, sizeof(short int), 1, saida);               
               continue;
               }
            operacao_num = Tinstrucoes[id_op].opcode;
            fwrite(&operacao_num, sizeof(short int), 1, saida);
            if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
               fwrite(&info_reloc, sizeof(short int), 1, saida);            
            }
         if(Tinstrucoes[id_op].length >= 2)           // Traduz o primeiro operando.
            {
            i++;
            reloca = 0;
            
            if(isalpha(op1[0]))                       // Verifica se o operando é um símbolo.
               {
               Tsimbolos *aux;
               aux = pesquisa_Tsimbolos(op1);
               op1_num = aux->endereco;
               if(aux->reloc == 1)
                  reloca = 1;  
               }
            else if(isdigit(op1[0]))                  // Verifica se o operando é um endereço.
               {
               op1_num = atoi(op1);
               }
            else if(op1[0] == '@')                    // Verifica se o operando é um literal.
               {
               op1_num = busca_end_lit(op1);
               reloca = 1;
               }
            fwrite(&op1_num, sizeof(short int), 1, saida);
            info_reloc >>= 1;
            if (reloca)
               info_reloc += 0x8000;           
            if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
               fwrite(&info_reloc, sizeof(short int), 1, saida);         
            }
         if(Tinstrucoes[id_op].length == 3)           // Traduz o segundo operando.
            {
            i++;
            reloca = 0;
            
            if(isalpha(op2[0]))                       // Verifica se o operando é um símbolo.
               {
               Tsimbolos *aux;
               aux = pesquisa_Tsimbolos(op2);
               op2_num = aux->endereco;
               if(aux->reloc == 1)
                  reloca = 1; 
               }
            else if(isdigit(op2[0]))                  // Verifica se o operando é um endereço.
               {
               op2_num = atoi(op2);
               }
            else if(op2[0] == '@')                    // Verifica se o operando é um literal.
               {
               op2_num = busca_end_lit(op2);
               reloca = 1;
               }
            fwrite(&op2_num, sizeof(short int), 1, saida);
            info_reloc >>= 1;
            if (reloca)
               info_reloc += 0x8000;   
            if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
               fwrite(&info_reloc, sizeof(short int), 1, saida);
            }
      }
   // Coloca os literais ao final
#ifdef DEBUG
   printf ("\nEscrevendo literais:\n\n");
#endif
   for( ; Tab_lit ; Tab_lit = Tab_lit->prox)
      {
      char *aux_nome;
      i++;
      aux_nome = Tab_lit->nome;
      aux_nome++;
      //Conversão hexa ou decimal
      if (*aux_nome == 'H')
         {
         aux_nome += 2;
         i = httoi(aux_nome);
         op1_num = i;
         if (i < 0)                    // Excedeu o limite
            {
            coloca_Terros(15, 0, i, Tab_lit->nome);
            imprime_erros();
            system("pause");
            exit(1);
            }
         }
      else
         {
         i = atoi(aux_nome);
         if (i < 0 || i > 0xFFFE)      // Excedeu o limite
            {
            coloca_Terros(15, 0, i, Tab_lit->nome);
            imprime_erros();
            system("pause");
            exit(1);
            }
         op1_num = i;
         }
#ifdef DEBUG      
      printf ("op1_num = %d\tTab_lit->nome = %s\n", op1_num, Tab_lit->nome);
#endif
      fwrite(&op1_num, sizeof(short int), 1, saida);
      info_reloc >>= 1;
      if( !(i % 16) )                                  // Escreve a palavra de relocação se já escreveu 16 palavras de código.
         fwrite(&info_reloc, sizeof(short int), 1, saida);
      free(Tab_lit);
      }
   
   op1_num = -1; 
   for( ; i % 16 ; i++)
      {
         fwrite(&op1_num, sizeof(short int), 1, saida);
         info_reloc >>= 1;
      }
   fwrite(&info_reloc, sizeof(short int), 1, saida);
   fclose(entrada);
   fclose(saida);
   }
