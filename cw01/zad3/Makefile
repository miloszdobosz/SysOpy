CC = gcc -Wall

static:
	make clear
	$(CC) -c bibl1.c
	ar rcs bibl1.a bibl1.o
	$(CC) -o main main.c bibl1.a

shared:
	make clear
	$(CC) -c -fPIC bibl1.c
	$(CC) -shared -fPIC -o bibl1.so bibl1.o
	$(CC) -o main main.c -L. bibl1.so -Wl,-rpath=`pwd`

dynamic:
	make clear
	$(CC) -c -fPIC bibl1.c
	$(CC) -shared -fPIC -o bibl1.so bibl1.o
	$(CC) -o main main_dynamic.c -ldl

clear:
	rm -f *.[aos]*
