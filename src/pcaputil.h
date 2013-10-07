/*
 * pcaputil.h
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef PCAPUTIL_H_
#define PCAPUTIL_H_
#include <pcap.h>

/*
 * Open pcap device
 */
pcap_t *
pcap_open(u_char *device);

/*
 * Set bpf filter for capturing
 */
int
pcap_filter(pcap_t *pcap, const u_char *fmt, ...);

#endif /* PCAPUTIL_H_ */
