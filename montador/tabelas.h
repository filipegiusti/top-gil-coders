typedef struct Tsimbolos
   {
   char nome[8];
   int endereco;
   int linha;
   char reloc;
   struct Tsimbolos *prox;
   } Tsimbolos;

/* Tipos de erros:
         1 -> símbolo multidefinido;
         2 -> operação inválida;
         3 -> número errado de operandos para a operação;
         4 -> label inválido;
         5 -> símbolo indefinido;
         6 -> falta diretiva end;
         7 -> problema na diretiva stack;
         8 -> tamanho de pilha não definido;
         9 -> falta a diretiva start;
         10 -> problema na diretiva const;
         11 -> código após a diretiva end;
         12 -> Linha maior que 80 caracteres;
         13 -> Label sozinho;
         14 -> Digito inválido;
         15 -> Valor fora dos limites.
*/
typedef struct Terros
   {
   int tipo;
   int linha;
   int lcounter;
   char texto[15];
   struct Terros *prox;
   } Terros;

typedef struct Tliterais
   {
   char nome[9];
   int linha;
   int endereco;
   struct Tliterais *prox;
   } Tliterais;
   
typedef struct Tdefinicoes
   {
   char nome[9];
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
   char nome[9];
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
Tliterais *Tab_lit;
Tliterais *ultima_lit;
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
void escreve_definicao(const Tdefinicoes *simbolo, FILE *fp);

TusosEncapsulado *coloca_Tusos(char *label);
void coloca_Tusos_end(int lcounter, TusosEncapsulado *pos_uso);
TusosEncapsulado *pesquisa_Tusos(char *op);
void escreve_uso(const Tusos *simbolo, FILE *fp);

Tliterais *pesquisa_Tlit(char *op);
int busca_end_lit(char *literal);
int verifica_literal(char *simbolo);
void coloca_tab_literais(char *op, int linha);
int httoi(const char *value);
