#include <stdio.h>

int main(int argc, char ** argv) {
	printf("\33[1m"); /* Para habilitar escrita em negrito */
	printf("Oi em negrito\n");
	printf("\33[0m"); /* Para habilitar escrita normal */
	printf("Oi normal\n");
	/* Podem ser usados juntos */
	printf("Hello Normal \33[1m Hello Negrito \33[0m Hello Normal\n");

	return 0;
}