#include <stdio.h>
#include <strings.h>
#include "ligador.h"
#define DEBUG


// Lista de erros
void coloca_Terros(short int tipo, char *texto)
{
   Terros *novo;
   printf("\nERROS\ttipo = %d\t%s ",tipo,texto);
   novo = malloc(sizeof(Terros));
   novo->tipo = tipo;
   strcpy(novo->texto, texto);
   novo->prox = NULL; 

   if(!tabelaErros)
   {
      tabelaErros = novo;
      ultimoErro = tabelaErros;
   }
   else
   {
      ultimoErro->prox = novo;
      ultimoErro = novo;
   }
}

// Armazena os símbolos indefinidos na lista de erros
void simbIndef()
{
   int i, j, aux;
   
   for(i=0; i<=auxTabelaUso; i++)
   {
      aux = 0;
      for(j=0; j<auxTabelaSG; j++)
      {
         if( strcmp(tabelaUso[i].nome,tabelaSG[j].nome)==0 )
            break;
         aux++;
      }
      if (aux == auxTabelaSG)
         coloca_Terros(2, tabelaUso[i].nome);
   }
}

void imprimeErros()
{
   Terros *aux;
   printf("\nTABELA DE ERROS\n");    
   for (aux = tabelaErros ; aux ; aux = aux->prox)
      printf ("\tTexto = %s\ttipo = %d\n",aux->texto, aux->tipo);
}


short int carregar(int arqN, char arq[])
{
    FILE *fp;
    int i;
    short int ok;       // Saber quantos itens foram lidos do arquivo
    short int aux;      // Auxilia na relocação dos relativos
    int tamCodigo, tamPilha, inicial;

    if( (fp = fopen(arq, "rb")) == NULL )
    {
        printf("\n\tArquivo inexistente ou corrompido.\n");
        return 0;
    }
// Lê tamanho do código.
    ok = fread(&tamCodigo, 4, 1, fp);
    #ifdef DEBUG
    printf("\n\ttamCodigo = %d",tamCodigo);
    #endif
    tamCodGlobal += tamCodigo;
    if(ok < 1)
    {
        printf("\n\tErro na leitura do tamanho do código.\n");
        return 0;
    }
// Lê tamanho da pilha.
    ok = fread(&tamPilha, 4, 1, fp);
    #ifdef DEBUG    
    printf("\n\ttamPilha = %d",tamPilha);
    #endif
    pilhaGlobal += tamPilha;
    if(ok < 1)
    {
        printf("\n\tErro na leitura do tamanho da pilha.\n");
        return 0;
    }
// Lê o endereço inicial do programa.
    ok = fread(&inicial, 4, 1, fp);
    #ifdef DEBUG
    printf("\n\tendInicial = %d",inicial);
    #endif
   if(arqN == 1)              // se é o 1º arquivo
      endGlobal = inicial;    // armazena o endereço inicial
   if(ok < 1)
    {
        printf("\n\tErro na leitura do endereco inicial.\n");
        return 0;
    }
// Lê a tabela de símbolos globais
    auxTabelaSG--;
    do
   {
      auxTabelaSG++;
      fread(&tabelaSG[auxTabelaSG], sizeof(tabelaSimbolos), 1, fp);
      #ifdef DEBUG
      printf("\n\ttabelaSG[auxTabelaSG].nome = %s  end = %d  infoReloc = %c",tabelaSG[auxTabelaSG].nome, tabelaSG[auxTabelaSG].end, tabelaSG[auxTabelaSG].infoReloc);
      #endif
      if(arqN > 1)           // se não é o 1º arquivo
         for(i=0; i<auxTabelaSG; i++)
            if( !(strcmp(tabelaSG[auxTabelaSG].nome,tabelaSG[i].nome)) )
            {
               coloca_Terros(1, tabelaSG[auxTabelaSG].nome);
               auxTabelaSG--;
            }
// Acerta o endereço com a constante de relocação
        if(tabelaSG[auxTabelaSG].infoReloc == 'r')
            tabelaSG[auxTabelaSG].end += (tamCodGlobal - tamCodigo);
    } while (strcmp(tabelaSG[auxTabelaSG].nome,"") || tabelaSG[auxTabelaSG].end != 0);

// Lê a tabela de usos
   auxTabelaUso--;
   do
   {
      auxTabelaUso++;
      fread(&tabelaUso[auxTabelaUso], sizeof(tabelaUsos), 1, fp);
      if( (arqN > 1) && (tabelaUso[auxTabelaUso].endSimbolo>0) )                                                             // se não é o 1º arquivo
         tabelaUso[auxTabelaUso].endSimbolo += (tamCodGlobal - tamCodigo);       // soma o tamanho dos arquivos anteriores ao campo de endereço.
      #ifdef DEBUG
      printf("\n\ttabelaUso[auxTabelaUso].nome = %s  end = %d  sinal = %c",tabelaUso[auxTabelaUso].nome, tabelaUso[auxTabelaUso].endSimbolo, tabelaUso[auxTabelaUso].sinal);
      #endif
   } while ( (strcmp(tabelaUso[auxTabelaUso].nome,"") != 0) || tabelaUso[auxTabelaUso].endSimbolo != 0);
   auxTabelaUso--;                                                               // apaga o espaço em branco lido.
   printf("\nauxTabelaUso = %d", auxTabelaUso);
// Lê Código e reloca os relativos
   fread(&codigo[auxCodigo], sizeof(codigoReloc), 1, fp);
   while (!feof(fp))
   {
      aux = codigo[auxCodigo].infoReloc;
      #ifdef DEBUG
      printf ("\n\tinfoReloc = %d",aux);
      #endif
      for(i=0 ; i<16 ; i++)
      {
         if(aux & 1) // testa se é relocável
            codigo[auxCodigo].var[i] += (tamCodGlobal - tamCodigo);
         aux >>= 1;  // shift right
         #ifdef DEBUG
         printf ("\t\tcodigo[auxCodigo].var[i] = %d\n",codigo[auxCodigo].var[i]);
         #endif
      }
      auxCodigo++;  
      fread(&codigo[auxCodigo], sizeof(codigoReloc), 1, fp);      
   }
   
   if (fclose(fp) != 0)
   {
      printf("\n\tErro no fechamento do arquivo\n");
      return 0;
   }
   return 1;
}


short int salvar(char arq[])
{
   FILE *fp;
   int auxTabUso = 0; // Contém o endereço do vetor da tabela de usos
   int auxTabSG = 0;
   int pc = 0;
   int auxStruct = 0;
   short int ok = 0;
   int endUltimaStruct; // Numero de elementos do ultimo caso da escrita.
   
    strcat(arq, ".hpx");
   
    if( (fp = fopen( arq, "wb")) == NULL )
    {
        printf("\n\tNao foi possivel criar o arquivo\n");
        return 0;
    }

    ok = fwrite(&tamCodGlobal,4,1,fp);
    if(!ok)
    {
        printf("\n\tErro na escrita do tamanho do codigo.\n");
        return 0;
    }
    ok = fwrite(&pilhaGlobal,4,1,fp);
    if(!ok)
    {
        printf("\n\tErro na escrita do tamanho da pilha.\n");
        return 0;
    }
    ok = fwrite(&endGlobal,4,1,fp);
    if(!ok)
    {
        printf("\n\tErro na escrita do endereço inicial.\n");
        return 0;
    }
    
   endUltimaStruct = tamCodGlobal-(tamCodGlobal%16);
   
   for(pc=endGlobal; pc<tamCodGlobal+endGlobal; pc+=16)                          // pc recebe endereço inicial
   {
      if(endUltimaStruct == pc-endGlobal)
      {
         for(auxStruct=0; auxStruct<(tamCodGlobal%16); auxStruct++)                                // percorre 16 palavras da estrututura
         {
            if( (tabelaUso[auxTabUso].endSimbolo == pc+auxStruct) && (tabelaUso[auxTabUso].endSimbolo!=-1) ) // se encontrar o endereço a ser substituido
            {
               for(auxTabSG=0; auxTabSG<auxTabelaSG; auxTabSG++)                             // percorre a tabela de simbolos globais buscando o simbolo
               {
                  if( strcmp(tabelaSG[auxTabSG].nome, tabelaUso[auxTabUso].nome) == 0 )      // se encontrou o simbolo
                  {
                     codigo[(pc-endGlobal)/16].var[auxStruct] += tabelaSG[auxTabSG].end;  // soma o endereço do símbolo
                     break;
                  }
               }
               auxTabUso++;
            }
            fwrite(&codigo[(pc-endGlobal)/16].var[auxStruct],2,1,fp);
         }
      }
      else
      {
         for(auxStruct=0; auxStruct<16; auxStruct++)                                         // percorre 16 palavras da estrututura
         {
            if( (tabelaUso[auxTabUso].endSimbolo == pc+auxStruct) && (tabelaUso[auxTabUso].endSimbolo!=-1) ) // se encontrar o endereço a ser substituido
            {
               for(auxTabSG=0; auxTabSG<auxTabelaSG; auxTabSG++)                             // percorre a tabela de simbolos globais buscando o simbolo
               {
                  if( strcmp(tabelaSG[auxTabSG].nome, tabelaUso[auxTabUso].nome) == 0 )      // se encontrou o simbolo
                  {
                     codigo[(pc-endGlobal)/16].var[auxStruct] += tabelaSG[auxTabSG].end;  // soma o endereço do símbolo
                     break;
                  }
               }
               auxTabUso++;
            }
            fwrite(&codigo[(pc-endGlobal)/16].var[auxStruct],2,1,fp);
         }
      }
   }
   if (fclose(fp) != 0)
   {
      printf("\n\tErro no fechamento do arquivo\n");
      return 0;
   }
   return 1;
}
