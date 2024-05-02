#ifndef FILESYSTEM_H

#define FILESYSTEM_H

#include <stdbool.h>

char line[256];
void fatFsInit(void);
bool fileWrite(const char * fName, const char* msg);
char* fileRead(const char* fileName);
void fileRemove(const char* fileName); 

#endif