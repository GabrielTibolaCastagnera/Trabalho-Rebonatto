#include <stdio.h>

int main(int argc, char ** argv) {
	printf("\33[40m"); /* Para habilitar escrita com fundo cor preta */
	printf("Oi com fundo cor preta\n");
	printf("\33[41m"); /* Para habilitar escrita com fundo cor vermelha */
	printf("Oi com fundo cor vermelha\n");
    printf("\33[42m"); /* Para habilitar escrita com fundo cor verde */
	printf("Oi com fundo cor verde\n");
	printf("\33[43m"); /* Para habilitar escrita com fundo cor laranja */
	printf("Oi com fundo cor laranja\n");	
	printf("\33[44m"); /* Para habilitar escrita com fundo cor azul */
	printf("Oi com fundo cor azul\n");
	printf("\33[45m"); /* Para habilitar escrita com fundo cor magenta */
	printf("Oi com fundo cor magenta\n");
	printf("\33[46m"); /* Para habilitar escrita com fundo cor ciano */
	printf("Oi com fundo cor ciano\n");
	printf("\33[47m"); /* Para habilitar escrita com fundo cor branca */
	printf("Oi com fundo cor branca\n");
	
	return 0;
}
