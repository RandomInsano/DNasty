#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "main.h"
#include "resolver.h"

// TODO: I'm going to have to do this differently soon.
//       Since a single domain name can have many IP addresses, we're going to
//       need a structure to hold them. Ideally the lookup would just be the
//       straight text from the query header, but that may not be flexible
//       enough. Oh well, more thought needed.

// Returns address in buffer and length of address. Always pass enough space
// for a v6 address, or I'm going to be very mad at you!
size_t get_address(const char* hostname, char* address_buffer)
{
	// TODO: Fancy data structure for quick caching and lookup
	// TODO: Load from external file
	// TODO: Lookup from a more authorative server when not found
	// TODO: Cache that discovery
	// TODO: Remove stale cache goodness

	struct in_addr address4;
	struct in6_addr address6;

	memset(address_buffer, 0, IP_MAX_LEN);

	if (strcmp(hostname, "router.enet.local.") == 0)
	{
		// IPv4 address lookup placeholder
		if (inet_pton(AF_INET, "1.2.3.4", &address4) != 1)
			return 0;

		memcpy(address_buffer, &address4, IPV4_LEN);
		return IPV4_LEN;
	}

	if (strcmp(hostname, "router6.enet.local.") == 0)
	{
		if (inet_pton(AF_INET6, "fe80::1234", &address6) != 1)
			return 0;

		memcpy(address_buffer, &address6, IPV6_LEN);
		return IPV6_LEN;
	}

	return 0;
}
