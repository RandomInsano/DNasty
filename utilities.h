#ifndef __utilities_h
#define __utilities_h

#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

void die(int code, const char* message);
void hexdump(void *ptr, int buflen);

#endif
