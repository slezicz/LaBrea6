/*
 * pcaputil.c
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */
#include <stdarg.h>
#include <pcap.h>
#include "pcaputil.h"
#include "labrea6.h"

/*
 * Open pcap device
 */
pcap_t *
pcap_open(u_char *device)
{
  char ebuf[PCAP_ERRBUF_SIZE];
  pcap_t *pcap;

  if (device == NULL)
    {
      if ((device = pcap_lookupdev(ebuf)) == NULL)
        return (NULL);
    }

  if ((pcap = pcap_open_live(device, 31337, 1, PCAP_TIMEOUT, ebuf)) == NULL)
    return (NULL);

  return (pcap);
}

/*
 * Set bpf filter for capturing
 */
int
pcap_filter(pcap_t *pcap, const u_char *fmt, ...)
{
  struct bpf_program fcode;
  char buf[BUFSIZ];
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (pcap_compile(pcap, &fcode, buf, 1, 0) < 0)
    return (-1);

  if (pcap_setfilter(pcap, &fcode) == -1)
    return (-1);

  return (0);

}

//void pcap_stat(pcap_t *pd) {
//	struct pcap_stat stat;
//
//	/* show 'em some stats... */
//	if (pd != NULL) {
//		if (pcap_stats(pd, &stat) >= 0) {
//			util_print(NORMAL, "%d/%d packets (received/dropped) by filter", stat.ps_recv, stat.ps_drop);
//		}
//	}
//
//}
