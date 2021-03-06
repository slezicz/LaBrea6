/*
 * lbio.h
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef LBIO_H_
#define LBIO_H_
#include <pcap.h>
#include <netinet/ip6.h>

/* Main IO ctl structure */

#define IP6_ADDR_LEN 16

struct io_s
{
  struct in6_addr myip; /* My IP address */
  uint8_t man_host_info; /* is my ip set, so we can ignore trafic to it */
  struct addr *mymac; /* My mac addr */
  struct addr *bogusmac; /* Bogus mac addr */

  /* handles */
  pcap_t *pcap; /* pcap handler */
};

typedef struct io_s io_t;
extern io_t io; /* Let it available everywhere */

/*
 * Pseudo header, used for higher layer checksumming.
 */
union ip6_pseudo_hdr
{
  struct
  {
    struct in6_addr ph_src;
    struct in6_addr ph_dst;
    u_int32_t ph_len;
    u_int8_t ph_zero[3];
    u_int8_t ph_nxt;
  } ph;
  u_int16_t pa[20];
};

/* Open the device for capturing and set bpf filter */
void
lbio_init(u_char *dev, u_char *texpr);

/* Count the check sum*/
u_short
lbio_in_cksum(const u_short *addr, register u_int len, int csum);

/*Count the next protocol checksum*/
int
lbio_nextproto6_cksum(const struct ip6_hdr *ip6, const u_short *data,
    u_int len, u_int next_proto);

/* send the packet to the wires ;-)*/
int
lbio_send_pkt(u_char* packet, int len);

/* Prepare the reply packet from request and send it */
int
lbio_send_echo_reply(u_char* packet, int len);

/* Prepare the ND Neighbor adv. packet, add the bogus MAC and send it */
int
lbio_send_neighbor_adv(u_char* packet, int len);

/*Prepare the TCP packet and send it*/
int
lbio_send_tcp(u_char* packet, int len, char* msg);

/* End up the work with device, print stats */
void
lbio_close(void);
#endif /* LBIO_H_ */
