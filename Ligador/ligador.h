#define TAM_TAB 100

short int carregar(int nArq, char arq[]);
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

/* ERROS
      1. Símbolo multidefinido
      2. Símbolo não definido
*/
struct Terros
{
   short int tipo;
   char texto[8];
   struct Terros *prox;
};
typedef struct Terros Terros;


extern int pilhaGlobal;
extern int endGlobal;
extern int tamCodGlobal;

extern tabelaSimbolos tabelaSG[];
extern int auxTabelaSG;
extern tabelaUsos tabelaUso[];
extern int auxTabelaUso;
extern codigoReloc codigo[];
extern int auxCodigo;

extern Terros *tabelaErros;
extern Terros *ultimoErro;
