#ifndef __utilities_h
#define __utilities_h

#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

// Decompression state machine states.
enum ddstates
{
	readchar,	// Text octet
	ctrl,		// Control octet
	pointer		// Special label pointer goodness
};

void die(int code, const char* message);
void hexdump(void *ptr, int buflen);
void decompress_domain(const char* input, char* output);

#endif
