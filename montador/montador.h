int main();
int primeira_passagem(FILE *arquivo);
int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2, char *comment);
int numero(char letra);
void coloca_pool(char *op);
int segunda_passagem();

int lcounter = 0;
