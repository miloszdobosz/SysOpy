CC = gcc -Wall

all:
	make clear
	$(CC) -c bibl1.c
	ar rcs bibl1.a bibl1.o
	$(CC) -o main main.c bibl1.a

clear:
	rm -f *.[aos]*
