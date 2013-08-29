#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"
#include "main.h"

void decompress_domain(const char* input, char* output)
{
	unsigned short pos;			// Where we're at with the input
	unsigned short out;  		// Where we're at with the output
	unsigned char  lbl_pos;		// Where are we in the label?
	enum ddstates  state;		// What are we doing right now?

	const unsigned char mask = 0xC0;	// Control bit mask for label pointer
										// Also, two 1 bits denote a pointer.
										// Two 0 bits is a label, and the other
										// two combinations, I don't know yet.
	pos = 0;
	out = 0;
	lbl_pos = 0;
	state = ctrl;

	// If an octet is zero, that's the end of the domain as per the RFC
	while (input[pos])
	{
		if (pos > HOSTNAME_MAX_LEN)
			break;

		switch(state)
		{
		case readchar:
			output[out++] = input[pos];
			lbl_pos--;

			// If there's no more text, fall back to reading control codes
			if (0 == lbl_pos)
			{
				// Add a peroid since text is done
				output[out++] = '.';

				state = ctrl;
			}

			break;

		case ctrl:
			// If the two most significant bits are zero, this denotes
			// that what follows is a label. The last six bits of the octet
			// says how many ASCII characters we need to read.
			if (0 == (input[pos] & mask))
			{
				lbl_pos = (unsigned char)input[pos];
				state = readchar;
			}
			break;

		case pointer:

			break;
		}

		pos++;
	}

	// Add that final stringy null
	output[pos] = 0;
}

void die(int code, const char* message)
{
	perror(message);
	printf("\n");
	exit(code);
}

void hexdump(void *ptr, int buflen) {
  unsigned char *buf = (unsigned char*)ptr;
  int i, j;
  for (i=0; i<buflen; i+=16) {
    printf("%06x: ", i);
    for (j=0; j<16; j++)
      if (i+j < buflen)
        printf("%02x ", buf[i+j]);
      else
        printf("   ");
    printf(" ");
    for (j=0; j<16; j++)
      if (i+j < buflen)
        printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
    printf("\n");
  }
}

