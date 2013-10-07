/*
 * labrea6.c
 *
 *  Created on: May 12, 2010
 *      Author: slezicz,slezicz@gmail.com
 */
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "labrea6.h"
#ifndef LBIO_H_
#include "lbio.h"
#endif
#ifndef CTL_H_
#include "ctl.h"
#endif
#ifndef UTILS_H_
#include "utils.h"
#endif
#ifndef PKT_HANDLER_H
#include "pkt_handler.h"
#endif


/* io control stucture */
io_t io;

/* control structure */
ctl_t ctl;

/*output control */
outp_t outp;

int
main(int argc, char **argv)
{

    outp.verbose = VERBOSE; /* to be able to log attackers */
    outp.output = FL_OUTP_STDOUT_EPOCH;/* This is default we want to use unix time format */
    outp.syslog_open = FALSE; /* stdout is default */
    ctl.throttlesize = 0;
    srand(time(NULL));
    ctl.sequence = htonl(rand());
  labrea_init(argc, argv); /* load parametrs...*/
  util_set_signal_handlers(); /* set signal handlers to be able to shutdown */


  /* capturing loop */
  for(;;){
    if (pcap_dispatch(io.pcap, -1, &pkt_handler, (u_char *)&ctl) < 0) {
          util_print(NORMAL, "Error in pcap loop - EXITING: %s", pcap_geterr(io.pcap));
          util_clean_exit(1);
    }
  }
  util_clean_exit(0);
}
