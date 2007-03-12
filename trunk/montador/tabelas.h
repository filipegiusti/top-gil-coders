typedef struct Tsimbolos
   {
   char nome[8];
   int endereco;
   int linha;
   char reloc;
   struct Tsimbolos *prox;
   } Tsimbolos;

/* Tipos de erros:
         1 -> s�mbolo multidefinido;
         2 -> opera��o inv�lida;
         3 -> n�mero errado de operandos para a opera��o;
         4 -> label inv�lido;
         5 -> s�mbolo indefinido;
         6 -> falta diretiva end;
         7 -> problema na diretiva stack;
         8 -> tamanho de pilha n�o definido;
         9 -> falta a diretiva start;
         10 -> problema na diretiva const;
         11 -> c�digo ap�s a diretiva end;
*/
typedef struct Terros
   {
   int tipo;
   int linha;
   int lcounter;
   char texto[10];
   struct Terros *prox;
   } Terros;

typedef struct Tdefinicoes
   {
   char nome[8];
   int endereco;
   char sinal;
   struct Tdefinicoes *prox;
   } Tdefinicoes;

typedef struct Tusos
   {
   char nome[8];
   int endereco;
   char reloc;
   struct Tusos *prox;
   } Tusos;   
      
struct Tinstrucoes
   {
   char nome[6];
   int length;
   int num_operandos;
   int opcode;
   } Tinstrucoes[23];

Tsimbolos *Tabela_S;
Tsimbolos *ultimo_simbolo;
Terros *Tabela_erros;
Terros *ultimo_erro;
Tdefinicoes *Tab_def;
Tdefinicoes *ultima_def;
Tusos *Tab_usos;
Tusos *ultima_usos;

void inicializa_tabelas();
void tabela_instrucoes();
int verifica_operacao(char *operacao);
void imprime_tabelas();

void coloca_Terros(int erro, int linha, int lcounter, char *texto);

Tsimbolos *coloca_Tsimbolos(char *label, int line);
void coloca_Tsimbolos_label(char *label, int lcounter, int line);
void coloca_Tsimbolos_operando(char *op, int line);
Tsimbolos *pesquisa_Tsimbolos(char *op1);

void coloca_Tdef(char *op1);
int coloca_Tdef_end(int lcounter, Tdefinicoes *pos_def);
Tdefinicoes *pesquisa_Tdef(char *op1);

Tusos *coloca_Tusos(char *label);
void coloca_Tusos_end(int lcounter, Tusos *pos_uso);
Tusos *pesquisa_Tusos(char *op);
