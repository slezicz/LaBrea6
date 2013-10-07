/*
 * labrea6.h
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef LABREA6_H_
#define LABREA6_H_

#define PCAP_TIMEOUT 100
#include <syslog.h>
#include <netinet/ip6.h>

//#define DEBUG

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif
#define MYFREE(x)free(x); x = NULL;

#define ETH_ADDR_BOGUS  "\x00\x00\x0f\xff\xff\xff"
                                /* Bogus MAC addr used in IP capture */
#define ECHO_HOPS 64
/* define to whatever makes sense to you for your system setup */
#define LOGTYPE LOG_WARNING
#define INFOTYPE LOG_INFO

#define IPV6_HLEN sizeof(struct ip6_hdr)

enum port_constants {
  PORT_IGNORE = 0,              /* Ignore this port */
  PORT_MONITOR = 1,             /* Monitor this port for activity */
  PORT_NUM = 65535
};
enum sizes {
  RANDSIZE2 = 12,               /* Keep 12 different random numbers */
  BUFSIZE = 1024,               /* General character buffer size */
  BPFSIZE = 65536,              /* Holds entire bpf filter */
  IP_INP_SIZE = 20,             /* Max length of IP addr when input: xxx.xxx.xxx.xxx/nn */
  ONE_K = 1024,                 /* For conversions */
  HIST_MIN = 4,                 /* Keep 5 minutes of bandwidth history */
  MAX_NUM_ADAPTER = 10,         /* Windows only: max num of adapters in list */
  PGM_NAME_SIZE = 50            /* Ident tag for syslog */
};

void
labrea_init(int argc, char **argv);

extern size_t
strlcpy(char *dst, const char *src, size_t siz);

#endif /* LABREA6_H_ */
