#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SOCK_BUFLEN 512 /* Max UDP size anyway */
#define SOCK_PORT    53

void die(int code, const char* message)
{
	perror(message);
	printf("\n");
	exit(code);
}

#include <ctype.h>
#include <stdio.h>
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

void parse_stuff(char* payload)
{
	union msg_array m;
	memcpy(m.data, payload, sizeof(m.data));

	printf("Query Infos:\n");
	printf("- ID:    %u\n", m.msg.id);
	printf("- Q#:    %u\n", htons(m.msg.question_count));
	printf("- A1#:   %u\n", htons(m.msg.answer_count));
	printf("- A2#:   %u\n", htons(m.msg.additional_count));

	hexdump(payload, sizeof(m.data));
}

int main()
{
	printf("Starting...\n");

	char buffer[SOCK_BUFLEN];
	int s;
	socklen_t s_len;
	int retcode;
	struct sockaddr_in si_local, si_remote;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1)
	{
		die(1, "Oh god! Socket creation failed!");
	}

	si_local = (const struct sockaddr_in){0};
	si_local.sin_family = AF_INET;
	si_local.sin_port = htons(SOCK_PORT);
	si_local.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *)&si_local, sizeof(si_local)) == -1)
	{
		die(2, "Binding failed...");
	}

	printf("Listening...\n");

	for (;;)
	{
		s_len = sizeof(si_remote);
		memset(buffer, 0, SOCK_BUFLEN);
		retcode = recvfrom(s, buffer, SOCK_BUFLEN, 0,
				(struct sockaddr *)&si_remote, &s_len);
		if (retcode == -1)
		{
			die(3, "Receive failed.");
		}

		// This is where the real stuff is. Everything in this function
		// is just bootstrapping. Stupid C and it's simplicity >:(
		parse_stuff(buffer);

		/*
		retcode = sendto(s, buffer, SOCK_BUFLEN, 0, (struct sockaddr *)&si_remote,
				s_len);
		if (retcode == -1)
		{
			die(4, "Echo back failed.");
		}
		*/
	}

	printf("I'm done.\n");
}
