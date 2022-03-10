#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bibl1.h"

void** allocate(int n) {
	void** w = calloc(n, sizeof(void*));
	printf("zarezerwowano");
	return w;
}

void count(char* file) {
	system(strcat(strcat("wc ", file), ">> temp"));
}

void write() {
}

void clear() {
}
