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
         12 -> Linha maior que 80 caracteres;
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
   } Tdefinicoes;
   
typedef struct TdefinicoesEncapsulado
   {
   Tdefinicoes info;
   struct TdefinicoesEncapsulado *prox;
   }  TdefinicoesEncapsulado;

typedef struct Tusos
   {
   char nome[8];
   int endereco;
   char reloc;
   } Tusos;
   
typedef struct TusosEncapsulado
   {
   Tusos info;
   struct TusosEncapsulado *prox;
   }  TusosEncapsulado;   
      
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
TdefinicoesEncapsulado *Tab_def;
TdefinicoesEncapsulado *ultima_def;
TusosEncapsulado *Tab_usos;
TusosEncapsulado *ultima_usos;

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
int coloca_Tdef_end(int lcounter, TdefinicoesEncapsulado *pos_def);
TdefinicoesEncapsulado *pesquisa_Tdef(char *op1);

TusosEncapsulado *coloca_Tusos(char *label);
void coloca_Tusos_end(int lcounter, TusosEncapsulado *pos_uso);
TusosEncapsulado *pesquisa_Tusos(char *op);
