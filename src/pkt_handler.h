/*
 * pkt_handler.c
 *
 *  Created on: May 16, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef PKT_HANDLER_H
#define PKT_HANDLER_H

#include <pcap.h>

/*
 * The handler called in the capturing loop when packet arrives
 *
 */
void
pkt_handler(u_char* client_data, const struct pcap_pkthdr* pcpkt,
    const u_char* pktdata);

#endif /* PKT_HANDLER_H */
