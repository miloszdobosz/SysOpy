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

small:
	./main ct 10 search_directory /usr/lib . tmp

medium:
	./main ct 1000 search_directory /usr . tmp

big:
	./main ct 1000000 search_directory / . tmp

create_delete:
	./main ct 1000 search_directory /usr . tmp remove_block 0 search_directory /usr . tmp remove_block 1 search_directory /usr . tmp remove_block 2 search_directory /usr . tmp remove_block 3 search_directory /usr . tmp remove_block 4

all_tests:
	make small
	make medium
	make big
	make create_delete
