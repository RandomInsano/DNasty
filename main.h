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


#define IP_MAX_LEN			16  /* IPv6 is only 16 bytes so far */
#define LABEL_MAX_LEN		63  /* Max length of a piece of domain name */
#define HOSTNAME_MAX_LEN	255 /* Max length of a domain name in an A query */

// Function definitions
size_t parse_stuff(char*);



#endif
