/*
 * utils.h
 *
 *  Created on: May 13, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <sys/types.h>
/* Degree of verbosity in logging */
typedef enum
{
  QUIET, NORMAL, VERBOSE, VERY_VERBOSE
} amt_log_t;


/* Output / logging control structure */
struct outp_s {

  /* flags */
  u_int16_t              output;
#define FL_OUTP_STDOUT_EPOCH    0x0001  /* -O */
#define FL_OUTP_STDOUT          0x0002  /* -o */



  /* globals */
  amt_log_t             verbose;        /* Degree of verbosity in logging */
  int                   savedatalog;    /* Save old logging level when logging turned off */
  int                   syslog_open;    /* TRUE if syslog open */
};

typedef struct outp_s outp_t;
extern outp_t outp;


/*
 * This function is intended for further development. Not implemented yet.
 */
void
catch_sig_restart(int sig);

/*
 * This function is intended for further development. Not implemented yet.
 */

void
catch_sig_timer(int sig);

/*
 * This function is intended for further development. Not implemented yet.
 */

void
catch_sig_toggle_logging(int sig);

void
catch_sig_quit(int sig);

void
util_quit(void);

/*
 * This function is intended for further development. Not implemented yet.
 */

void
util_restart(void);

/*
 * Set signal handlers
 */
void
util_set_signal_handlers(void);


/*
 * Logg function
 */
void
util_print(const amt_log_t verbosity_msg, const char *fmt, ...);


/*
 * Clean up everithing and quit.
 */
void
util_clean_exit(int err);
#endif /* UTILS_H_ */
