#pragma once 
extern void** allocate(int n);
extern void count(char* file);
extern void write_temp(void* buf);
extern void clear(void** table, int index);
