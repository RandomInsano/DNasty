#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "utilities.h"
#include "main.h"

// Returns address in buffer and length of address. Always pass enough space
// for a v6 address, or I'm going to be very mad at you!
size_t get_address(const char* hostname, char* address_buffer)
{
	// TODO: Fancy data structure for quick caching and lookup
	// TODO: Load from external file
	// TODO: Lookup from a more authorative server when not found
	// TODO: Cache that discovery
	// TODO: Remove stale cache goodness

	size_t len = 0;
	struct in_addr address;

	if (strcmp(hostname, "router.enet.local") == 0)
	{
		// IPv4 address lookup placeholder
		len = 4;
		if (inet_pton(AF_INET, "192.168.239.061", &address) != 1)
			die(10, "IP conversion failed!");
	}

	// Oh boy does this feel wrong.
	memset(address_buffer, 0, IP_MAX_LEN);
	memcpy(address_buffer, &address, len);

	printf("Debug Infos:\n");
	printf("  Host: %s\n", hostname);
	hexdump(address_buffer, IP_MAX_LEN);
	hexdump(&address, IP_MAX_LEN);

	return len;
}

size_t parse_stuff(char* payload)
{
	char address_buffer[16] = {0};
	char hostname_buffer[HOSTNAME_MAX_LEN];	// I think this in the RFC
	unsigned short address_len;

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

	m.msg.flags |= htons(DNS_MSGFLAG_QR|DNS_MSGFLAG_RD|DNS_MSGFLAG_AA|DNS_MSGFLAG_RA);
	m.msg.answer_count = htons(1);
	memcpy(payload, m.data, sizeof(m.data));

	// Scan for the end of the question resource. Assume one for now...
	char* p;
	p = payload;
	p += sizeof(m.data);             // Skip over main header
	p += 2;                          // Skip over question start flag

	// Copy hostname as safely as can be done
	strncpy(hostname_buffer, p, HOSTNAME_MAX_LEN);
	// Lookup the address
	address_len = (unsigned short)get_address(hostname_buffer, address_buffer);

	p += strnlen(p, HOSTNAME_MAX_LEN); // Skip over its hostname
	p += 5;                            // Skip over its class and type definitions

	uint32_t ttl = 86400;
	struct answer answer;
	answer.name  = DNS_ANSFLAG_COPY;   // Use the name from the request
	answer.type  = htons(A);           // A Record response
	answer.class = htons(Internet);    // Pretty much the only one going
	answer.ttl   = htonl(ttl);         // Five minutes
	answer.rdlen = htons(address_len); // Length of IP address

	// Copy answer into buffer
	memcpy(p, &answer, sizeof(answer));

	printf("Size: %u", sizeof(answer));

	// Skip over answer section
	p += sizeof(answer);
	memcpy(p, address_buffer, address_len);
	p += 4;

	printf("Modified: \n");
	hexdump(payload, 128);

	return (size_t)(p - payload);
}

int main()
{
	printf("Starting...\n");

	char buffer[SOCK_BUFLEN];
	int s;
	socklen_t s_len;
	size_t d_len;		// Length of response datas
	ssize_t retcode;
	char* interface = "lo";
	struct sockaddr_in si_local, si_remote;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1)
	{
		die(1, "Oh god! Socket creation failed!");
	}

	si_local = (const struct sockaddr_in){0};
	si_local.sin_family = AF_INET;
	si_local.sin_port = htons(SOCK_PORT);

	// Set inteface to bind to
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, interface, (socklen_t)strlen(interface)) == -1)
	{
		die (2, "Failed to select loopback interface for binding");
	}

	if (bind(s, (struct sockaddr *)&si_local, sizeof(si_local)) == -1)
	{
		die(3, "Binding failed...");
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
			die(4, "Receive failed.");
		}

		// This is where the real stuff is. Everything in this function
		// is just bootstrapping. Stupid C and it's simplicity >:(
		d_len = parse_stuff(buffer);

		// Fix the number of bytes its sending
		retcode = sendto(s, buffer, d_len, 0, (struct sockaddr *)&si_remote,
				s_len);
		if (retcode == -1)
		{
			die(5, "Echo back failed.");
		}
	}
}
