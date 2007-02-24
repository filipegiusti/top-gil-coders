//Desenvolvido por Bruno Machado da Silva 
//�ltima Modifica��o 17/02/07 18:14

/*Modulo possui a fun��o coloca_Tsimbolos, que insere um simbolo na respectiva 
tabela , sendo esta uma lista encadeada */

//bibliotecas

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//defini��o estrutura

struct Simbolos
    {
    char simbolo;
    int endereco;
    struct Simbolos* proximo;
    }*simb;
typedef struct Simbolos Simb;

//fun��o

void coloca_Tsimbolos(char* s)
    {    
    Simb* Aux;
    Aux = simb;
    
    //verifica ocorr�ncia
    while(Aux != NULL)
        {
        if (Aux->simbolo == *s)
                {
                printf(" Simbolo ja armazenado \n");
                getche();
                break;
                }
        Aux = Aux->proximo;
        }
     
    //cria n�
     
    Aux = simb;         
    Simb* novo =(Simb*) malloc(sizeof(Simb));
    novo->simbolo = *s;
    novo->endereco = -1;
    novo->proximo = NULL;
    
    //coloca na lista
    if(Aux == NULL) 
        simb = novo;
    else
        {
        while(Aux->proximo != NULL)
                Aux = Aux->proximo;
        Aux->proximo = novo;
        }
    }

