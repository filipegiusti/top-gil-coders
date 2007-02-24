typedef struct Tsimbolos
   {
   char nome[8];
   int endereco;
   char reloc;
   struct Tsimbolos *prox;
   } Tsimbolos;

/* tipos de erros:
         1 -> símbolo multidefinido
         2 -> operação inválida
*/
typedef struct Terros
   {
   int tipo;
   int linha;
   int lcounter;
   char texto[10];
   struct Terros *prox;
   } Terros;
   
struct Tinstrucoes
   {
   char nome[6];
   int length;
   } Tinstrucoes[22];

Tsimbolos *Tabela_S;
Tsimbolos *ultimo_simbolo;
Terros *Tabela_erros;
Terros *ultimo_erro;
   
void inicializa_tabelas();
void tabela_instrucoes();
int verifica_operacao(char *operacao);
Tsimbolos *pesquisa_Tsimbolos(char *op1);
Tsimbolos *coloca_Tsimbolos(char *label);
void coloca_Tendereco(int lcounter, Tsimbolos *pos);
void coloca_Terros(int erro, int linha, int lcounter, char *texto);
