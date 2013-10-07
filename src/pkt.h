/*
 * pkt.h
 *
 *  Created on: May 17, 2010
 *      Author: slezicz, slezicz@gmail.com
 */

#ifndef PKT_H_
#define PKT_H_

#define IPV6_HOP_BY_HOP 0
#define IPV6_DESTINATION_HDR 60
#define IPV6_ROUTING_HDR 43
#define IPV6_FRAGMENTATION_HDR 44

/*
 * Create a new packet. Allocate mem.
 */
u_char*
new_packet(int len);

/*
 * Allocate the memory for the copy. Create a copy of the packet.
 */
u_char*
copy_packet(const u_char* packet, int len);
/*
 * Allocate the memory for the copy. Create a copy of the ETH and IP headers.
 *
 */
u_char*
copy_eth_ip_hdr(const u_char* packet, int len);


/*
 * Append the next protocol to the new packet.
 */
u_char*
append_next_header(const u_char* header,u_char* packet_dst , int ofset, int len, int protocol_type);

/*
 * Find and skip ipv6 extension headers.
 * RETURN
 * pointer to the next protocol in case of TCP or ICMPv6, NULL in oter cases.
 * sets the next protocol type in uint8_t *protocol_type
 */
u_char*
next_proto_position(const u_char *packet, uint8_t *protocol_type);


/*
 * Free mem
 */
void
free_packet(u_char* packet);

#endif /* PKT_H_ */
