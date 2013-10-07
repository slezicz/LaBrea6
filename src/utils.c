/*
 * utils.c
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#include <stdlib.h>
#include <pcap.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>


#include "utils.h"
#include "lbio.h"
#include "ctl.h"
#include "labrea6.h"

void
catch_sig_restart(int sig)
{
  ctl.signals |= SIG_RESTART;
}

void
catch_sig_timer(int sig)
{
  ctl.signals |= SIG_TIMER;
}

void
catch_sig_toggle_logging(int sig)
{
  ctl.signals |= SIG_TOGGLE_LOGGING;
}

void
catch_sig_quit(int sig)
{
  util_quit();
}

/*
 * Do processing for a kill / int signal
 */
void
util_quit(void)
{
  ctl.signals &= ~SIG_QUIT;
  util_clean_exit(0);
}

/*
 * Do processing for a HUP signal
 */
void
util_restart(void)
{

  /*
   *  We won't react on the restart sig
   *
   *  */

  /*
   * DEBUG_PRT(DEBUG_SIGNAL, "util_restart");
   * ctl_init_arrays(1);
   * util_print(NORMAL, "Received HUP signal, processing re-initialized...");
   */
  ctl.signals &= ~SIG_RESTART; /* reset flag */
  return;
}

/*
 * set up the signal handlers
 */

void
util_set_signal_handlers(void)
{
  /* set signals so that we behave nicely */
  /* generic unix signal handlers */
  signal(SIGTERM, catch_sig_quit);
  signal(SIGINT, catch_sig_quit);
  signal(SIGQUIT, catch_sig_quit);
  signal(SIGHUP, catch_sig_restart);
  signal(SIGALRM, catch_sig_timer);
  signal(SIGUSR1, catch_sig_toggle_logging);
}


/*
 * Print a message to STDOUT, with the timestamp in one of two
 * formats, or we send it to syslog
 *
 */

void
util_print(const amt_log_t verbosity_msg, const char *fmt, ...)
{
  char buf[BUFSIZE] = "";
  char tnow[BUFSIZE] = "";
  time_t current = time(NULL);
  char *p = NULL;

  va_list ap; /* parameters */

  if (verbosity_msg > outp.verbose) /* is the degree of verbosity sufficent */
    return;

  va_start(ap, fmt);
  if (fmt != NULL) {
    vsnprintf(buf, sizeof(buf), fmt, ap);
  }
  va_end(ap);

  /* do we log in stdout or syslog  */
  if (outp.output & FL_OUTP_STDOUT || !outp.syslog_open) {
    if (outp.output & FL_OUTP_STDOUT_EPOCH){ /*time format */
      (void)printf("%lu %s\n", current, buf);
      fflush(stdout);
    }

    else { /* set the log string */
      strlcpy(tnow, ctime(&current),sizeof(tnow));
      p = tnow + strlen(tnow) -1; /* Point to last character */
      if (*p == '\n')
	*p = ' ';		/* replace trailing \n by blank */
      (void)printf("%s %s\n", tnow, buf);
      fflush(stdout);
    }
  } else
    syslog(INFOTYPE, buf); /* log to syslog */
}

/* Free all arrays list memory, close io ports,  and exit */

void
util_clean_exit(int err)
{
  MYFREE(ctl.addresses_arr);
  lbio_close();
  exit(err);
}
