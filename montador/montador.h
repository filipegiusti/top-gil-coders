int main();
int primeira_passagem();
int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2);
int numero(char letra);
void coloca_pool(char *op);
int segunda_passagem();

int lcounter = 0;
int tam_pilha = -1;
int start = -1;
