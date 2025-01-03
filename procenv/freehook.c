#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void (*__free_hook)(void *);

int main() {
	char *binsh = (char *) malloc(64);
	if(binsh == NULL) {
		perror("malloc");
		return -1;
	}
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
#if defined(__GLIBC__) && !defined(__UCLIBC__) && __GLIBC__ == 2 && __GLIBC_MINOR__ < 34
	__free_hook = (void (*)(void *)) system;
	memcpy(binsh, "/bin/sh", 8);
	free(binsh);
#else
	fprintf(stderr, "__free_hook is only available before glibc 2.34 (exclusive)\n");
#endif
	return 0;
}

