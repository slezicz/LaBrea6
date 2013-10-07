/*
 * ctl.c
 *
 *  Created on: May 14, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/ip6.h>
#include <err.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

#include "ctl.h"
#include "lbio.h"
#include "labrea6.h"

void
init_ctr()
{
  ctl.addresses_arr = calloc(sizeof(struct in6_addr),
      DEFAULT_ADDRESSES_ARR_LENGHT);
  ctl.addresses_arr_size = DEFAULT_ADDRESSES_ARR_LENGHT;

  ctl.tcpport_filter = FALSE; /* we are listening on every port */
  ctl.ipaddr_filter = FALSE; /* we are listening on all ips on subnet */
}

/*
 * Used for determining existance of the file in char *filename.
 * RETURN:
 * TRUE - if file with filename in char * filename exists
 * FALSE - if the file doesn't exist
 */
int
file_exists(char * fileName)
{
  struct stat buf;
  int i = stat(fileName, &buf);
  /* File found */
  if (i == 0)
    {
      return TRUE;
    }
  return FALSE;

}

/*
 * Trims white spaces before and after the string in char * s.
 * This function modifies directly the string in parametr.
 *
 */
void
trim(char * s)
{
  char * p = s;
  int l = strlen(p);

  while (isspace(p[l - 1]))
    p[--l] = 0;
  while (*p && isspace(* p))
    ++p, --l;

  memmove(s, p, l + 1);
}

/*
 * Parse the IPv6 address from the char * address.
 *
 * RETURN
 * converted address in struct in6_addr in case the correct ipv6 address is in the string.
 * Otherwise it returns struct in6_addr initialized to IN6ADDR_ANY_INIT.
 */
struct in6_addr
parse_addr(char *address)
{
  struct in6_addr tmp_addr =
  IN6ADDR_ANY_INIT;

  trim(address); /* trim whitespaces */

  if (inet_pton(AF_INET6, address, &tmp_addr) < 0)
    return tmp_addr; /* init value */
#ifdef DEBUG
  warnx("%x%x%x%x", ntohl(tmp_addr.s6_addr32[0]), ntohl(tmp_addr.s6_addr32[1]),
      ntohl(tmp_addr.s6_addr32[2]), ntohl(tmp_addr.s6_addr32[3]));
#endif
  return tmp_addr;
}

void
parse_addr_file(char *filename)
{
  FILE *file;
  char line[INET6_ADDRSTRLEN];
  struct in6_addr tmp_addr;
  struct in6_addr null_addr =
  IN6ADDR_ANY_INIT;
  ctl.addresses = 0;

  /*check if the file with ips exists */
  if (file_exists(filename))
    {
      file = fopen(filename, "r"); /*open the file*/
      while (fgets(line, INET6_ADDRSTRLEN, file) != NULL) /*read one line of the file */
        {
#ifdef DEBUG
          warnx("line= %s, ctl.addresses_arr_size=%i, address %li ", line,
              ctl.addresses_arr_size, ctl.addresses);

#endif
          if (ctl.addresses >= ctl.addresses_arr_size) /* let's check if we have enougth space to store new ip address */
            {
              /* if not increase the size of the array */
              ctl.addresses_arr = realloc(ctl.addresses_arr,
                  sizeof(struct in6_addr) * (ctl.addresses_arr_size
                      + DEFAULT_ADDRESSES_ARR_LENGHT));
              ctl.addresses_arr_size = ctl.addresses_arr_size
                  + DEFAULT_ADDRESSES_ARR_LENGHT;
            }

          tmp_addr = parse_addr(line); /* parse the address to the structure */
          if (IN6_ARE_ADDR_EQUAL(&tmp_addr, &null_addr)) /* check if the conversion successed */
            continue;
          ctl.addresses_arr[ctl.addresses] = tmp_addr; /* add the new addres to the array */
          ctl.addresses++;
#ifdef DEBUG
          warnx("adding address %s to array with index %i", line, ctl.addresses);
#endif

        }
      fclose(file); /* we are on the end of file so we can close it */
    }
}

void
parse_ports(char *ports)
{
  char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  char *old_subtoken;
  char *endptr;

  for (str1 = ports;; str1 = NULL) /* go through the string and split them into substrings */
    {
      token = strtok_r(str1, ",", &saveptr1); /* give us substring that is in the string separated with , */
      if (token == NULL)
        break;
#ifdef DEBUG
      warnx("%s\n", token);
#endif
      if (strstr(token, "-") != NULL) /* check if the substring if sigle port definition or range */
        {
          old_subtoken = NULL; /* it's a port range */
          for (str2 = token;; str2 = NULL)
            {
              subtoken = strtok_r(str2, "-", &saveptr2); /* separate the string lower and upper boundary of port range*/
              if (subtoken == NULL)
                break;
#ifdef DEBUG
              warnx("%s\n", subtoken);
#endif
              if (old_subtoken != NULL) /* do we have the lower boundary port?? */
                {
                  /* yes we have it in old_subtoken */
                  int low = strtol(old_subtoken, &endptr, 10); /* convert it to the number.. */
                  /* Check for various possible errors */

                  if ((errno == ERANGE && (low == LONG_MAX || low == LONG_MIN))
                      || (errno != 0 && low == 0))
                    {
#ifdef DEBUG
                      perror("strtol");
#endif
                      break;
                    }
                  if (endptr == old_subtoken)
                    {
#ifdef DEBUG
                      fprintf(stderr, "No digits were found\n");
#endif
                      break;
                    }

                  int top = strtol(subtoken, &endptr, 10); /* convert also the upper boundary port */
                  /* Check for various possible errors */

                  if ((errno == ERANGE && (top == LONG_MAX || top == LONG_MIN))
                      || (errno != 0 && top == 0))
                    {
#ifdef DEBUG
                      perror("strtol");
#endif
                      break;
                    }
                  if (endptr == subtoken)
                    {
#ifdef DEBUG
                      fprintf(stderr, "No digits were found\n");
#endif
                      break;
                    }

                  for (; low <= top; low++) /* go through the range and mark all port in the range as monitored*/
                    {
#ifdef DEBUG
                      warnx("%i,", low);
#endif
                      if (low > 0 && low <= PORT_NUM)
                        ctl.port_array[low] = PORT_MONITOR;
                    }

                }
              else
                {
                  old_subtoken = subtoken; /* we have found the lower boundary port */
                }
            }
        }
      else /* no "-" has been found -> single port definition */
        {
#ifdef DEBUG
          warnx("%i, ", atoi(token));
#endif
          int port = strtol(token, &endptr, 10); /* conversion */
          /* Check for various possible errors */

          if ((errno == ERANGE && (port == LONG_MAX || port == LONG_MIN))
              || (errno != 0 && port == 0))
            {
#ifdef DEBUG
              perror("strtol");
#endif
              continue;
            }
          if (endptr == token)
            {
#ifdef DEBUG
              fprintf(stderr, "No digits were found\n");
#endif
              continue;
            }

          if (port > 0 && port <= PORT_NUM) /* set the monitoring flag */
            ctl.port_array[port] = PORT_MONITOR;
        }
    }

}

int
filter_check_ip(const struct ip6_hdr *ip6)
{
  uint64_t i;

  /*TODO: tohle je podle me spatna logika, kdyz bude vypnuty filtrovani tak nebude poslouchat nic, protoze vraci false*/
  /* filter traffic for ip that labrea is running at */
  if (io.man_host_info && (IN6_ARE_ADDR_EQUAL(&io.myip, &ip6->ip6_dst) || IN6_ARE_ADDR_EQUAL(&io.myip, &ip6->ip6_src)))
    return FALSE;

  if(!ctl.ipaddr_filter) /* if the filtering is off we care about all the traffic */
    return TRUE;

  for (i = 0; i <= ctl.addresses; i++) /* find if we have ip address in the array */
    {
      /*This part is because of the ND. It requests are addressed special multicast format */
      /*Mulicast ff02::1:ffxx:xxxx*/
      if ((ntohl(ip6->ip6_dst.s6_addr16[0]) == 0xff020000) && ((ntohl(ip6->ip6_dst.s6_addr32[3])
          | 0xff000000) == (ntohl(ctl.addresses_arr[i].s6_addr32[3])
          | 0xff000000)))
        {
          return TRUE; /* success */
        }
#ifdef DEBUG
      warnx("%x:%x:%x:%x == %x:%x:%x:%x\n", htonl(ip6.s6_addr32[0]), htonl(
          ip6.s6_addr32[1]), htonl(ip6.s6_addr32[2]), htonl(ip6.s6_addr32[3]),

      htonl(ctl.addresses_arr[i].s6_addr32[0]), htonl(
          ctl.addresses_arr[i].s6_addr32[1]), htonl(
          ctl.addresses_arr[i].s6_addr32[2]), htonl(
          ctl.addresses_arr[i].s6_addr32[2]));
      warnx("%x = %x, %x = %x", htonl(ip6.s6_addr16[0]), 0xff02, ntohl(
          ip6.s6_addr32[3]) | 0xff000000, ntohl(
          ctl.addresses_arr[i].s6_addr32[3]) | 0xff000000);
#endif
      if (IN6_ARE_ADDR_EQUAL(&ip6->ip6_dst, &ctl.addresses_arr[i]))
        {

          return TRUE; /* success */
        }
    }
  return FALSE;
}

int
filter_check_port(u_int16_t port)
{
#ifdef DEBUG
  warnx("port %i = %i", port, ctl.port_array[port]);
#endif
  if (ctl.port_array[port] == PORT_MONITOR) /* check if the port is monitored */
    return TRUE;
  return FALSE;
}
