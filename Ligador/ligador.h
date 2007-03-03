#define TAM_TAB 100

short int carregar(char arq[]);
short int salvar(char arq[]);

struct tabelaSimbolosGlobal
{
   char nome[8];
   int end;
   char infoReloc;
};
typedef struct tabelaSimbolosGlobal tabelaSimbolos;

struct tabelaSimbolosUso
{
   char nome[8];
   int endSimbolo;
   char sinal;
};
typedef struct tabelaSimbolosUso tabelaUsos;

struct codigoRelocacao
{
   short int var[16];
   short int infoReloc;
};
typedef struct codigoRelocacao codigoReloc;

extern int pilhaGlobal;
extern int endGlobal;
extern int tamCodGlobal;

extern tabelaSimbolos tabelaSG[];
extern int auxTabelaSG;
extern tabelaUsos tabelaUso[];
extern int auxTabelaUso;
extern codigoReloc codigo[];
extern int auxCodigo;
