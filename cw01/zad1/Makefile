CC = gcc -Wall -std=c11 -ggdb


all: static shared 

static:
	make clear
	$(CC) -c bibl1.c
	ar rcs bibl1.a bibl1.o

shared:
	make clear
	$(CC) -c -fPIC bibl1.c
	$(CC) -shared -fPIC -o bibl1.so bibl1.o

clear:
	rm -f *.[aos]*
