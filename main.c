#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "utilities.h"
#include "main.h"

void parse_stuff(char* payload)
{
	union msg_array m;
	memcpy(m.data, payload, sizeof(m.data));

	printf("Query Infos:\n");
	printf("- ID:    %u\n", m.msg.id);
	printf("- Q#:    %u\n", ntohs(m.msg.question_count));
	printf("- A1#:   %u\n", ntohs(m.msg.answer_count));
	printf("- A2#:   %u\n", ntohs(m.msg.additional_count));

	hexdump(payload, 128);


	////////////////////////////////////////////////////
	// Modify and make an answer by hand for now...
	// *** This is SUPER brittle! ***
	////////////////////////////////////////////////////

	m.msg.flags |= htons(DNS_MSGFLAG_QR|DNS_MSGFLAG_RD|DNS_MSGFLAG_RA);
	m.msg.answer_count = htons(1);
	memcpy(payload, m.data, sizeof(m.data));

	// Scan for the end of the question resource. Assume one for now...
	char* p;
	p = payload;
	p += sizeof(m.data);             // Skip over main header
	p += 1;                          // Skip over question start flag
	p += strlen(p);                  // Skip over its hostname
	p += 4;                          // Skip over its class and type definitions

	uint32_t ttl = 0;
	struct answer answer;
	answer.type  = htons(A);         // A Record response
	answer.class = htons(Internet);  // Pretty much the only one going
	answer.rdlen = htons(4);         // Length of IP address
	answer.ttl   = htons(ttl);       // Five minutes
	answer.name  = DNS_ANSFLAG_COPY; // Use the name from the request

	// Copy answer into buffer
	memcpy(p, &answer, sizeof(answer));

	p += sizeof(answer);
	p[0] = 200;
	p[1] = 236;
	p[2] = 31;
	p[3] = 11;

	printf("Modified: \n");
	hexdump(payload, 39+5);
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

		// Fix the number of bytes its sending
		retcode = sendto(s, buffer, 39, 0, (struct sockaddr *)&si_remote,
				s_len);
		if (retcode == -1)
		{
			die(4, "Echo back failed.");
		}
	}

	printf("I'm done.\n");
}
