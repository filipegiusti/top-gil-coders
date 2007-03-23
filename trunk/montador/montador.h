int main();
int primeira_passagem();
int parser(FILE *arquivo, char *label, char *operacao, char *op1, char *op2);
int numero(char letra);
int segunda_passagem();
void escreveInfoReloc (FILE *fp);

int lcounter = 0;
int tam_pilha = -1;
int start = -1;
