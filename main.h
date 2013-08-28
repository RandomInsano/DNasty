#ifndef __main_h
#define __main_h

#define SOCK_BUFLEN 512 /* Max UDP size anyway */
#define SOCK_PORT    53

#define DNS_MSGFLAG_QR       0x8000 /* 10000000 00000000 */
#define DNS_MSGFLAG_OPCODE   0x7800 /* 01111000 00000000 */
#define DNS_MSGFLAG_AA       0x0400 /* 00000100 00000000 */
#define DNS_MSGFLAG_TF       0x0200 /* 00000010 00000000 */
#define DNS_MSGFLAG_RD       0x0100 /* 00000001 00000000 */
#define DNS_MSGFLAG_RA       0x0080 /* 00000000 10000000 */
#define DNS_MSGFLAG_AD       0x0020 /* 00000000 00100000 */
#define DNS_MSGFLAG_CD       0x0010 /* 00000000 00010000 */
#define DNS_MSGFLAG_Z  	     0x0070 /* 00000000 01110000 */
#define DNS_MSGFLAG_RCODE    0x000F /* 00000000 00001111 */

// Handy pre-built flaggies
#define DNS_MSGFLAG_RESP     DNS_MSGFLAG_QR /* I refuse fragments for now! */

#define DNS_ANSFLAG_COPY     0x0CC0

enum opcode {
	Reserved0,
	LLQ,
	UL,
	NSID,
	Reserved1,
	DAU,
	DHU,
	N3U,
	ClientSubnet
};

// Message Opcodes
enum mopcode {
	Query,
	Inverse,
	Status
};

enum dclass {
	Reserved2,
	Internet,
	Unassigned0,
	Chaos,
	Hesiod,
	Unassigned1
};

enum rcode {
	NoError,
	FormErr,
	ServFail,
	NXDomain,
	NotImp,
	Refused,
	YXDomain,
	YXRRSet,
	NXRRSet,
	NotAuthServer,
	NotAuth,
	NotZone
};

enum acode {
	A     = 1,
	NS    = 2,
	CNAME = 5,
	SOA   = 6,
	WKS   = 11,
	PTR   = 12,
	MX    = 15,
	SRV   = 33,
	A6    = 38
};

#pragma pack(push, 1)

struct message {
	// DNS Header stuff
	unsigned short id;				// Transaction ID
	unsigned short flags;			// Random switches
	unsigned short question_count;	// Number of questions in this message
	unsigned short answer_count;	// Number of answers "
	unsigned short auth_count;		// Number of authority records "
	unsigned short additional_count;// Number of random crap records "
};

union msg_array
{
	struct message msg;
	unsigned char data[12];
};

struct question {
	unsigned char * message;
	unsigned short type;
	unsigned short class;
};

struct answer {
	unsigned short name;
	unsigned short type;
	unsigned short class;
	unsigned int ttl;
	unsigned short rdlen;
};

#pragma pack(pop)

// Function definitions
int parse_stuff(char*);



#endif
