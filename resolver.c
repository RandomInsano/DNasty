#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "main.h"
#include "resolver.h"

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

	if (strcmp(hostname, "router.enet.local.") == 0)
	{
		// IPv4 address lookup placeholder
		len = 4;
		if (inet_pton(AF_INET, "1.2.3.4", &address) != 1)
			die(10, "IP conversion failed!");
	}

	// Oh boy does this feel wrong.
	memset(address_buffer, 0, IP_MAX_LEN);
	memcpy(address_buffer, &address, len);

	return len;
}
