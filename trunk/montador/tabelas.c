#include <stdlib.h>
#include <string.h>
#include "tabelas.h"


void inicializa_tabelas()
   {
   Tabela_S = NULL;
   ultimo_simbolo = NULL;
   Tabela_erros = NULL;
   ultimo_erro = NULL;
   Tab_def = NULL;
   ultima_def = NULL;
   Tab_usos = NULL;
   ultima_usos = NULL;
   tabela_instrucoes();     
   }
   
// *********************************** MANIPULANDO TABELA DE INSTRUÇÕES:

// Constrói tabela de instruções
void tabela_instrucoes()
   {
   // Instruções de máquina
   strcpy(Tinstrucoes[0].nome, "ADD");
   Tinstrucoes[0].length = 2;
   Tinstrucoes[0].num_operandos = 1;
   Tinstrucoes[0].opcode = 2;
   strcpy(Tinstrucoes[1].nome, "BR");
   Tinstrucoes[1].length = 2;
   Tinstrucoes[1].num_operandos = 1;
   Tinstrucoes[1].opcode = 0;
   strcpy(Tinstrucoes[2].nome, "BRNEG");
   Tinstrucoes[2].length = 2;
   Tinstrucoes[2].num_operandos = 1;
   Tinstrucoes[2].opcode = 5;
   strcpy(Tinstrucoes[3].nome, "BRPOS");
   Tinstrucoes[3].length = 2;
   Tinstrucoes[3].num_operandos = 1;
   Tinstrucoes[3].opcode = 1;
   strcpy(Tinstrucoes[4].nome, "BRZERO");
   Tinstrucoes[4].length = 2;
   Tinstrucoes[4].num_operandos = 1;
   Tinstrucoes[4].opcode = 4;
   strcpy(Tinstrucoes[5].nome, "CALL");
   Tinstrucoes[5].length = 2;
   Tinstrucoes[5].num_operandos = 1;
   Tinstrucoes[5].opcode = 15;
   strcpy(Tinstrucoes[6].nome, "COPY");
   Tinstrucoes[6].length = 3;
   Tinstrucoes[6].num_operandos = 2;
   Tinstrucoes[6].opcode = 13;
   strcpy(Tinstrucoes[7].nome, "DIVIDE");
   Tinstrucoes[7].length = 2;
   Tinstrucoes[7].num_operandos = 1;
   Tinstrucoes[7].opcode = 10;
   strcpy(Tinstrucoes[8].nome, "LOAD");
   Tinstrucoes[8].length = 2;
   Tinstrucoes[8].num_operandos = 1;
   Tinstrucoes[8].opcode = 3;
   strcpy(Tinstrucoes[9].nome, "MULT");
   Tinstrucoes[9].length = 2;
   Tinstrucoes[9].num_operandos = 1;
   Tinstrucoes[9].opcode = 14;
   strcpy(Tinstrucoes[10].nome, "READ");
   Tinstrucoes[10].length = 2;
   Tinstrucoes[10].num_operandos = 1;
   Tinstrucoes[12].opcode = 12;
   strcpy(Tinstrucoes[11].nome, "RET");
   Tinstrucoes[11].length = 1;
   Tinstrucoes[11].num_operandos = 0;
   Tinstrucoes[11].opcode = 16;
   strcpy(Tinstrucoes[12].nome, "STOP");
   Tinstrucoes[12].length = 1;
   Tinstrucoes[12].num_operandos = 0;
   Tinstrucoes[12].opcode = 11;
   strcpy(Tinstrucoes[13].nome, "STORE");
   Tinstrucoes[13].length = 2;
   Tinstrucoes[13].num_operandos = 1;
   Tinstrucoes[13].opcode = 7;
   strcpy(Tinstrucoes[14].nome, "SUB");
   Tinstrucoes[14].length = 2;
   Tinstrucoes[14].num_operandos = 1;
   Tinstrucoes[14].opcode = 6;
   strcpy(Tinstrucoes[15].nome, "WRITE");
   Tinstrucoes[15].length = 2;
   Tinstrucoes[15].num_operandos = 1;
   Tinstrucoes[15].opcode = 8;
   
   // Instruções de montagem
   strcpy(Tinstrucoes[16].nome, "CONST");
   Tinstrucoes[16].length = 1;
   Tinstrucoes[16].num_operandos = 1;   
   strcpy(Tinstrucoes[17].nome, "END");
   Tinstrucoes[17].length = 0;   
   Tinstrucoes[17].num_operandos = 0;
   strcpy(Tinstrucoes[18].nome, "EXTDEF");
   Tinstrucoes[18].length = 0;
   Tinstrucoes[18].num_operandos = 1;
   strcpy(Tinstrucoes[19].nome, "EXTR");
   Tinstrucoes[19].length = 0;
   Tinstrucoes[19].num_operandos = 0;
   strcpy(Tinstrucoes[20].nome, "SPACE");
   Tinstrucoes[20].length = 1;
   Tinstrucoes[20].num_operandos = 0;
   strcpy(Tinstrucoes[21].nome, "STACK");
   Tinstrucoes[21].length = 0;
   Tinstrucoes[21].num_operandos = 1;
   strcpy(Tinstrucoes[22].nome, "START");
   Tinstrucoes[22].length = 0;
   Tinstrucoes[22].num_operandos = 1;
   }

/* Retorna: Índice da operação na tabela de instruções
            ou -1 se a operação nao existir
*/
int verifica_operacao(char *operacao)
    {
    int i;
    for (i = 0 ; i<23 ; i++)
        {
        if(!strcmp(Tinstrucoes[i].nome, operacao))
            return i;
        }
    return -1;
    }

void imprime_tabelas() {
    int i;
    Tsimbolos *aux_Ts;
    TdefinicoesEncapsulado *aux_Td;
    TusosEncapsulado *aux_Tu;
    Terros *aux_Te;
    /*printf("TABELA DE INSTRUÇÕES\n");
    for (i = 0 ; i < 23 ; i++) {
        printf ("\tNome = %s\tlength = %d\n",Tinstrucoes[i].info.nome,Tinstrucoes[i].length);
    }*/
    printf("TABELA DE SIMBOLOS\n");    
    for (aux_Ts = Tabela_S ; aux_Ts ; aux_Ts = aux_Ts->prox) {
        printf ("\tNome = %s\tendereco = %d\treloc = %d\n",aux_Ts->nome, aux_Ts->endereco, aux_Ts->reloc);
    }
    printf("TABELA DE DEFINICOES\n");    
    for (aux_Td = Tab_def ; aux_Td ; aux_Td = aux_Td->prox) {
        printf ("\tNome = %s\tendereco = %d\tsinal = %d\n",aux_Td->info.nome, aux_Td->info.endereco, aux_Td->info.sinal);
    }
    printf("TABELA DE USOS\n");    
    for (aux_Tu = Tab_usos ; aux_Tu ; aux_Tu = aux_Tu->prox) {
        printf ("\tNome = %s\tendereco = %d\treloc = %d\n",aux_Tu->info.nome, aux_Tu->info.endereco, aux_Tu->info.reloc);
    }
    printf("TABELA DE ERROS\n");    
    for (aux_Te = Tabela_erros ; aux_Te ; aux_Te = aux_Te->prox) {
        printf ("\tTexto = %s\tlcounter = %d\tlinha = %d\ttipo = %d\n",aux_Te->texto, aux_Te->lcounter, aux_Te->linha, aux_Te->tipo);
    }

}

// *********************************** MANIPULANDO TABELA DE ERROS:
    
/* Recebe:  erro = codigo do erro
            texto = ???
*/
void coloca_Terros(int erro, int linha, int lcounter, char *texto)
   {
   Terros *novo;
   novo = malloc(sizeof(Terros));
   novo->tipo = erro;
   novo->linha = linha;
   novo->lcounter = lcounter;
   strcpy(novo->texto, texto);
   novo->prox = NULL; 

   if(!Tabela_erros)
      {
      Tabela_erros = novo;
      ultimo_erro = Tabela_erros;
      }
   else
      {
      ultimo_erro->prox = novo;
      ultimo_erro = novo;
      }
   }
   
int imprime_erros()
   {
   if(!Tabela_erros)
      return 0;
   else
      {
      Terros *aux;
      printf("TABELA DE ERROS\n");    
      for (aux = Tabela_erros ; aux ; aux = aux->prox)
         printf ("\tTexto = %s\tlcounter = %d\tlinha = %d\ttipo = %d\n",aux->texto, aux->lcounter, aux->linha, aux->tipo);
      }
    }
   
   
   
// *********************************** MANIPULANDO TABELA DE SÍMBOLOS:

// Cria nova entrada na tabela.
Tsimbolos *coloca_Tsimbolos(char *label, int linha)
   {
   Tsimbolos *novo;
   novo = malloc(sizeof(Tsimbolos));
   strcpy(novo->nome, label);
   novo->endereco = -1;
   novo->linha = linha;
   novo->reloc = -1;
   novo->prox = NULL;

   if(!Tabela_S)
      {
      Tabela_S = novo;
      Tabela_S->prox = NULL;
      ultimo_simbolo = Tabela_S;
      }
   else
      {
      ultimo_simbolo->prox = novo;
      ultimo_simbolo = novo;
      }
   return novo;
   }

// Cria nova entrada na tabela proveniente de um label.
void coloca_Tsimbolos_label(char *label, int lcounter, int line)
   {
   Tsimbolos *novo;
   if(pesquisa_Tsimbolos(label))
      {
      if(pesquisa_Tsimbolos(label)->endereco != -1)
         {
         coloca_Terros(1, line, lcounter, label);        // Símbolo multidefinido
         return;
         }
      else
         {
         pesquisa_Tsimbolos(label)->endereco = lcounter;
         return;
         }
      }
   novo = coloca_Tsimbolos(label, line);
   novo->endereco = lcounter;
   novo->reloc = 1;                                 // Quando for simbolo externo.
   }

// Cria nova entrada na tabela proveniente de um operando.
void coloca_Tsimbolos_operando(char *op, int linha)
   {
   Tsimbolos *novo;
   if(pesquisa_Tsimbolos(op))
      {
      return;
      }
   novo = coloca_Tsimbolos(op, linha);
   novo->reloc = 1;
   }

/* Retorna: Endereço do símbolo na tabela
            ou NULL caso nao exista
*/            
Tsimbolos *pesquisa_Tsimbolos(char *simbolo)
   {
   Tsimbolos *aux;
   aux = Tabela_S;
   while(aux)
      {
      if(!strcmp(simbolo, aux->nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }

            
void verifica_Tsimbolos()
   {
   Tsimbolos *aux;
   aux = Tabela_S;
   while(aux)
      {
      if(aux->endereco == -1)
         coloca_Terros(5, aux->linha, -1, aux->nome);
      aux = aux->prox;
      }
   }

// *********************************** MANIPULANDO TABELA DE DEFINIÇÕES:
    
// Cria nova entrada na tabela de definições
void coloca_Tdef(char *op1)
   {
   TdefinicoesEncapsulado *novo;
   
   if(pesquisa_Tdef(op1))
      return;
   
   novo = malloc(sizeof(TdefinicoesEncapsulado));
   strcpy(novo->info.nome, op1);
   novo->info.endereco = -1;
   novo->info.sinal = 1;
   novo->prox = NULL;

   if(!Tab_def)
      {
      Tab_def = novo;
      Tab_def->prox = NULL;
      ultima_def = Tab_def;
      }
   else
      {
      ultima_def->prox = novo;
      ultima_def = novo;
      }
   }

/* Retorna: Endereço do símbolo na tabela
            ou NULL caso nao exista
*/  
TdefinicoesEncapsulado *pesquisa_Tdef(char *op1)
   {
   TdefinicoesEncapsulado *aux;
   aux = Tab_def;
   while(aux)
      {
      if(strcmp(op1, aux->info.nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }

int coloca_Tdef_end(int lcounter, TdefinicoesEncapsulado *pos_def)
   {
   if(pos_def->info.endereco == -1)
      pos_def->info.endereco = lcounter;
   else
      return 0;
   }

// *********************************** MANIPULANDO TABELA DE USOS:
    
// Cria nova entrada na tabela de usos sem endereço.
TusosEncapsulado *coloca_Tusos(char *label)
   {
   TusosEncapsulado *novo;
   novo = malloc(sizeof(TusosEncapsulado));
   strcpy(novo->info.nome, label);
   novo->info.endereco = -1;
   novo->info.reloc = -1;
   novo->prox = NULL;

   if(!Tab_usos)
      {
      Tab_usos = novo;
      Tab_usos->prox = NULL;
      ultima_usos = Tab_usos;
      }
   else
      {
      ultima_usos->prox = novo;
      ultima_usos = novo;
      }
   return novo;
   }

/* Seta endereço de pos_uso na tabela de usos
   ou cria nova entrada na tabela de usos com endereço lcounter
*/
void coloca_Tusos_end(int lcounter, TusosEncapsulado *pos_uso)
   {
   if(pos_uso->info.endereco == -1)
      {
      pos_uso->info.endereco = lcounter;
      pos_uso->info.reloc = 1;
      }
   else
      {
      TusosEncapsulado *novo;
      novo = coloca_Tusos(pos_uso->info.nome);
      novo->info.endereco = lcounter;
      novo->info.reloc = 1;               // Quando será outro valor??
      }
   }

// Retorna: 1ª entrada de op na tabela de usos.
TusosEncapsulado *pesquisa_Tusos(char *op)
   {
   TusosEncapsulado *aux;
   aux = Tab_usos;
   while(aux)
      {
      if(!strcmp(op, aux->info.nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }      
