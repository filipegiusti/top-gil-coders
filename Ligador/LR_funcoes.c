#include <stdio.h>
#include <strings.h>
#include "ligador.h"
#define DEBUG

short int carregar(char arq[]) {
    FILE *fp;
    int i;
    short int ok;       // Saber quantos itens foram lidos do arquivo
    short int aux;      // Auxilia na relocação dos relativos
    int tamCodigo, tamPilha, endInicial;

    if( (fp = fopen(arq, "rb")) == NULL ) {
        printf("\n\tArquivo inexistente ou corrompido.\n");
        return 0;
    }
// Lê tamanho do código.
    ok = fread(&tamCodigo, 4, 1, fp);
    #ifdef DEBUG
    printf("\n\ttamCodigo = %d",tamCodigo);
    #endif
    tamCodGlobal += tamCodigo;
    if(ok < 1) {
        printf("\n\tErro na leitura do tamanho do código.");
        return 0;
    }
// Lê tamanho da pilha.
    ok = fread(&tamPilha, 4, 1, fp);
    #ifdef DEBUG    
    printf("\n\ttamPilha = %d",tamPilha);
    #endif
    pilhaGlobal += tamPilha;
    if(ok < 1) {
        printf("\n\tErro na leitura do tamanho da pilha.");
        return 0;
    }
// Lê o endereço inicial do programa.
    ok = fread(&endInicial, 4, 1, fp);
    #ifdef DEBUG
    printf("\n\tendInicial = %d",endInicial);
    #endif
    if(ok < 1) {
        printf("\n\tErro na leitura do endereco inicial.");
        return 0;
    }
// Lê a tabela de símbolos globais
    auxTabelaSG--;
    do {
        auxTabelaSG++;
        fread(&tabelaSG[auxTabelaSG], sizeof(tabelaSimbolos), 1, fp);
        #ifdef DEBUG
        printf("\n\ttabelaSG[auxTabelaSG].nome = %s  end = %d  infoReloc = %c",tabelaSG[auxTabelaSG].nome, tabelaSG[auxTabelaSG].end, tabelaSG[auxTabelaSG].infoReloc);
        #endif
// Acerta o endereço com a constante de relocação
        if(tabelaSG[auxTabelaSG].infoReloc == 'r')
            tabelaSG[auxTabelaSG].end += (tamCodGlobal - tamCodigo);
    } while (strcmp(tabelaSG[auxTabelaSG].nome,"") || tabelaSG[auxTabelaSG].end != 0);

// Lê a tabela de usos
    auxTabelaUso--;
    do {
        auxTabelaUso++;
        fread(&tabelaUso[auxTabelaUso], sizeof(tabelaUsos), 1, fp);
        #ifdef DEBUG
        printf("\n\ttabelaUso[auxTabelaUso].nome = %s  end = %d  sinal = %c",tabelaUso[auxTabelaUso].nome, tabelaUso[auxTabelaUso].endSimbolo, tabelaUso[auxTabelaUso].sinal);
        #endif
    } while (strcmp(tabelaUso[auxTabelaUso].nome,"") || tabelaUso[auxTabelaUso].endSimbolo != 0);

// Lê Código e reloca os relativos
    do {
        fread(&codigo[auxCodigo], sizeof(codigoReloc), 1, fp);
        aux = codigo[auxCodigo].infoReloc;
        #ifdef DEBUG
        printf ("\n\tinfoReloc = %d",aux);
        #endif
        for(i=0 ; i<16 ; i++) {
            if(aux & 1)
                codigo[auxCodigo].var[i] += (tamCodGlobal - tamCodigo);
            aux >>= 1;
        }
        auxCodigo++;  
    } while (!feof(fp));
   
    if (fclose(fp) != 0) {
        printf("\n\tErro no fechamento do arquivo\n");
        getch();
    }
    return 1;
}


short int salvar(char arq[]) {
    FILE *fp;
    int end;
    short int ok;
    char arquivoSaida[50] = "";
    
    strcat(arquivoSaida, arq);
    strcat(arquivoSaida, ".hpx");
    endGlobal = 3+pilhaGlobal;
   
    if((fp = fopen( arquivoSaida, "wb")) == NULL ) {
        printf("\n\tNao foi possivel abrir o arquivo\n");
        return 0;
    }

    ok = fwrite(&tamCodGlobal,4,1,fp);
    if(!ok) {
        printf("\n\tErro na escrita do tamanho do codigo.");
        return 0;
    }
    ok = fwrite(&pilhaGlobal,4,1,fp);
    if(!ok) {
        printf("\n\tErro na escrita do tamanho da pilha.");
        return 0;
    }
    ok = fwrite(&endGlobal,4,1,fp);
    if(!ok) {
        printf("\n\tErro na escrita do endereço inicial.");
        return 0;
    }
    for(end = endGlobal; end<tamCodGlobal; end++) {
        
    }
}
