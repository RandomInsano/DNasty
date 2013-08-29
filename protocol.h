#ifndef __protocol_h
#define __protocol_h

// Contains structures and enums specifically for the wire-level protocol.

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

#endif
