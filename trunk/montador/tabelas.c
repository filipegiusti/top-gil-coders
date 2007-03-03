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
   
// MANIPULANDO TABELA DE INSTRUÇÕES:      
void tabela_instrucoes()
   {
   // Instruções de máquina
   strcpy(Tinstrucoes[1].nome, "ADD");
   Tinstrucoes[1].length = 2;
   strcpy(Tinstrucoes[2].nome, "BR");
   Tinstrucoes[2].length = 2;   
   strcpy(Tinstrucoes[3].nome, "BRNEG");
   Tinstrucoes[3].length = 2;
   strcpy(Tinstrucoes[4].nome, "BRPOS");
   Tinstrucoes[4].length = 2;
   strcpy(Tinstrucoes[5].nome, "BRZERO");
   Tinstrucoes[5].length = 2;
   strcpy(Tinstrucoes[6].nome, "CALL");
   Tinstrucoes[6].length = 2;
   strcpy(Tinstrucoes[7].nome, "COPY");
   Tinstrucoes[7].length = 3;
   strcpy(Tinstrucoes[8].nome, "DIVIDE");
   Tinstrucoes[8].length = 2;      
   strcpy(Tinstrucoes[9].nome, "LOAD");
   Tinstrucoes[9].length = 2;
   strcpy(Tinstrucoes[10].nome, "MULT");
   Tinstrucoes[10].length = 2;
   strcpy(Tinstrucoes[11].nome, "READ");
   Tinstrucoes[11].length = 2;
   strcpy(Tinstrucoes[12].nome, "RET");
   Tinstrucoes[12].length = 1;
   strcpy(Tinstrucoes[13].nome, "STOP");
   Tinstrucoes[13].length = 1;
   strcpy(Tinstrucoes[14].nome, "STORE");
   Tinstrucoes[14].length = 2;
   strcpy(Tinstrucoes[15].nome, "SUB");
   Tinstrucoes[15].length = 2;
   strcpy(Tinstrucoes[16].nome, "WRITE");
   Tinstrucoes[16].length = 2;
   
   // Instruções de montagem
   strcpy(Tinstrucoes[15].nome, "CONST");
   Tinstrucoes[15].length = 1;   
   strcpy(Tinstrucoes[16].nome, "END");
   Tinstrucoes[16].length = 0;   
   strcpy(Tinstrucoes[17].nome, "EXTDEF");
   Tinstrucoes[17].length = 0;   
   strcpy(Tinstrucoes[18].nome, "EXTR");
   Tinstrucoes[18].length = 0;
   strcpy(Tinstrucoes[19].nome, "SPACE");
   Tinstrucoes[19].length = 1;
   strcpy(Tinstrucoes[20].nome, "STACK");
   Tinstrucoes[20].length = 0;
   strcpy(Tinstrucoes[20].nome, "START");
   Tinstrucoes[20].length = 0;
   }
   
int verifica_operacao(char *operacao)
   {
   int i = 1;
   while(i<=20)
      {
      if(!strcmp(Tinstrucoes[i].nome, operacao))
         return i;
      i++;
      }
   return 0;
   }

// MANIPULANDO TABELA DE ERROS:
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
   
   
// MANIPULANDO TABELA DE SÍMBOLOS:
Tsimbolos *coloca_Tsimbolos(char *label)
   {
   Tsimbolos *novo;
   novo = malloc(sizeof(Tsimbolos));
   strcpy(novo->nome, label);
   novo->endereco = -1;
   novo->reloc = -1;

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
   return ultimo_simbolo;
   }

void coloca_Tendereco(int lcounter, Tsimbolos *pos)
   {
   pos->endereco = lcounter;
   pos->reloc = 1;          // Quando que o símbolo não será relativo??
   }
   
Tsimbolos *pesquisa_Tsimbolos(char *op1)
   {
   Tsimbolos *aux;
   aux = Tabela_S;
   while(aux)
      {
      if(strcmp(op1, aux->nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }


// MANIPULANDO TABELA DE DEFINIÇÕES:
void coloca_Tdef(char *op1)
   {
   Tdefinicoes *novo;
   novo = malloc(sizeof(Tdefinicoes));
   strcpy(novo->nome, op1);
   novo->endereco = -1;
   novo->reloc = -1;

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
   
Tdefinicoes *pesquisa_Tdef(char *op1)
   {
   Tdefinicoes *aux;
   aux = Tab_def;
   while(aux)
      {
      if(strcmp(op1, aux->nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }

int coloca_Tdef_end(int lcounter, Tdefinicoes *pos_def)
   {
   if(pos_def->endereco == -1)
      return 0;
   else
      pos_def->endereco = lcounter;
   }

// MANIPULANDO TABELA DE USOS:
void coloca_Tusos(char *label)
   {
   Tusos *novo;
   novo = malloc(sizeof(Tusos));
   strcpy(novo->nome, label);
   novo->endereco = -1;
   novo->reloc = -1;

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
   }

void coloca_Tusos_novo(int lcounter, char *op)
   {
   Tusos *novo;
   novo = malloc(sizeof(Tusos));
   strcpy(novo->nome, op);
   novo->endereco = lcounter;
   novo->reloc = -1;

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
   }

int coloca_Tusos_end(int lcounter, Tusos *pos_uso)
   {
   if(pos_uso->endereco == -1)
      return 0;
   else
      pos_uso->endereco = lcounter;
   }

Tusos *pesquisa_Tusos(char *op)
   {
   Tusos *aux;
   aux = Tab_usos;
   while(aux)
      {
      if(strcmp(op, aux->nome))
         return aux;
      else     
         aux = aux->prox;
      }
   return NULL;
   }      
