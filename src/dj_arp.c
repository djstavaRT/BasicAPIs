/*
 * =====================================================================================
 *
 *       Filename:  dj_arp.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_arp.h"
#include "dj_base.h"
#include "dj_mem.h"
#include "dj_trace.h"

/*
**  define Macros
*/
//#define IP_MAXPACKET (65535)
#define ARPOP_REQUEST 1
#define ARPOP_REPLY 2

#define ETH_HDRLEN 14      // Ethernet header length
#define IP4_HDRLEN 20      // IPv4 header length
#define ARP_HDRLEN 28      // ARP header length


 /*=========================FUNCTION===================================================== 
  *
  * 	 Name: dj_arp_send_request
  * 	 Description: 
  *    Param: void
  * 	 Return: true , DJ_SUCCESS
  * 		      false , DJ_FAILURE
  *
  *======================================================================================
  */
DJ_INT32 dj_arp_send_request(DJ_INT8 *ifaceNum,DJ_INT8 *srcAddr,DJ_INT8 *dstAddr)
{
	DJ_INT32 i, status, frame_length, sd, bytes;
	DJ_INT8 *iface, *target, *src_ip;
	dj_arp_hdr arphdr;
	DJ_UINT8 *src_mac, *dst_mac;
	DJ_INT8 *ether_frame;
	struct addrinfo hints, *res;
	struct sockaddr_in *ipv4;
	struct sockaddr_ll device;
	struct ifreq ifr;
	
	// Allocate memory for various arrays.
	src_mac = (DJ_UINT8 *)dj_mem_malloc(6);
	dst_mac = (DJ_UINT8 *)dj_mem_malloc(6);
	ether_frame = dj_mem_malloc(IP_MAXPACKET);
	iface = dj_mem_malloc(40);
	target = dj_mem_malloc(40);
	src_ip = dj_mem_malloc(INET_ADDRSTRLEN);
	
	// Interface to send packet through.
	strcpy(iface, ifaceNum);
	strcpy(src_ip,srcAddr);
	strcpy(target,dstAddr);
	
	// Submit request for a socket descriptor to look up interface.
	if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) 
	{
		dj_printf("[DJ_ARP] create socket failed.\n");
		return DJ_FAILURE;
	}
	
	// Use ioctl() to look up interface name and get its MAC address.
	memset(&ifr, 0, sizeof (ifr));
	snprintf(ifr.ifr_name, sizeof (ifr.ifr_name), "%s", iface);
	if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0) 
	{
		dj_printf("[DJ_ARP] ioctl() failed to get source MAC address ");
		return DJ_FAILURE;
	}
	close (sd);
	
	// Copy source MAC address.
	memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (DJ_UINT8));
	
	// Report source MAC address to stdout.
	dj_printf("[DJ_ARP] MAC address for interface %s is ", iface);
	for (i=0; i<5; i++) 
	{
		dj_printf("%02x:", src_mac[i]);
	}
	dj_printf("%02x.\n", src_mac[5]);
	
	// Find interface index from interface name and store index in
	// struct sockaddr_ll device, which will be used as an argument of sendto().
	if ((device.sll_ifindex = if_nametoindex (iface)) == 0) 
	{
		dj_printf("[DJ_ARP] if_nametoindex() failed to obtain interface index ");
		return DJ_FAILURE;
	}
	//dj_printf("[DJ_ARP] Index for interface %s is %i\n", iface, device.sll_ifindex);
	
	// Set destination MAC address: broadcast address
	memset(dst_mac, 0xff, 6 * sizeof (DJ_UINT8));
	
	// Fill out hints for getaddrinfo().
	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = hints.ai_flags | AI_CANONNAME;
	
	// Resolve source using getaddrinfo().
	if ((status = getaddrinfo(src_ip, NULL, &hints, &res)) != 0) 
	{
		dj_printf("[DJ_ARP] getaddrinfo() failed.\n");
		return DJ_FAILURE;
	}
	ipv4 = (struct sockaddr_in *) res->ai_addr;
	memcpy(&arphdr.sender_ip, &ipv4->sin_addr, 4 * sizeof (DJ_UINT8));
	freeaddrinfo(res);
	
	// Resolve target using getaddrinfo().
	if ((status = getaddrinfo(target, NULL, &hints, &res)) != 0) 
	{
		dj_printf("[DJ_ARP] getaddrinfo() failed.\n");
		return DJ_FAILURE;
	}
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	memcpy(&arphdr.target_ip, &ipv4->sin_addr, 4 * sizeof (DJ_UINT8));
	freeaddrinfo(res);
	
	// Fill out sockaddr_ll.
	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr, src_mac, 6 * sizeof (DJ_UINT8));
	device.sll_halen = htons(6);
	
	// ARP header
	
	// Hardware type (16 bits): 1 for ethernet
	arphdr.htype = htons(1);
	
	// Protocol type (16 bits): 2048 for IP
	arphdr.ptype = htons(ETH_P_IP);
	
	// Hardware address length (8 bits): 6 bytes for MAC address
	arphdr.hlen = 6;
	
	// Protocol address length (8 bits): 4 bytes for IPv4 address
	arphdr.plen = 4;
	
	// OpCode: 1 for ARP request
	arphdr.opcode = htons(ARPOP_REQUEST);
	
	// Sender hardware address (48 bits): MAC address
	memcpy(&arphdr.sender_mac, src_mac, 6 * sizeof(DJ_UINT8));
	
	// Sender protocol address (32 bits)
	// See getaddrinfo() resolution of src_ip.
	
	// Target hardware address (48 bits): zero, since we don't know it yet.
	memset(&arphdr.target_mac, 0, 6 * sizeof(DJ_UINT8));
	
	// Target protocol address (32 bits)
	// See getaddrinfo() resolution of target.
	
	// Fill out ethernet frame header.
	
	// Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
	frame_length = 6 + 6 + 2 + ARP_HDRLEN;
	
	// Destination and Source MAC addresses
	memcpy(ether_frame, dst_mac, 6 * sizeof(DJ_UINT8));
	memcpy(ether_frame + 6, src_mac, 6 * sizeof(DJ_UINT8));
	
	// Next is ethernet type code (ETH_P_ARP for ARP).
	// http://www.iana.org/assignments/ethernet-numbers
	ether_frame[12] = ETH_P_ARP / 256;
	ether_frame[13] = ETH_P_ARP % 256;
	
	// Next is ethernet frame data (ARP header).
	
	// ARP header
	memcpy(ether_frame + ETH_HDRLEN, &arphdr, ARP_HDRLEN * sizeof(DJ_UINT8));
	
	// Submit request for a raw socket descriptor.
	if ((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) 
	{
		dj_printf("[DJ_ARP] socket() failed.\n");
		return DJ_FAILURE;
	}
	
	// Send ethernet frame to socket.
	if ((bytes = sendto(sd, ether_frame, frame_length, 0, (struct sockaddr *)&device, sizeof(device))) <= 0) 
	{
		dj_printf("[DJ_ARP] sendto() failed.\n");
		return DJ_FAILURE;
	}
	
	// Close socket descriptor.
	close(sd);
	
	// Free allocated memory.
	dj_mem_free((DJ_INT8 *)src_mac);
	dj_mem_free((DJ_INT8 *)dst_mac);
	dj_mem_free(ether_frame);
	dj_mem_free(iface);
	dj_mem_free(target);
	dj_mem_free(src_ip);
	
	return DJ_SUCCESS;
}

 
 /*=========================FUNCTION===================================================== 
  *
  * 	 Name: dj_arp_get_reply
  * 	 Description: 
  *    Param: void
  * 	 Return: true , DJ_SUCCESS
  * 		      false , DJ_FAILURE
  *
  *======================================================================================
  */
DJ_INT32 dj_arp_get_reply ()
{
	DJ_INT32 i, sd, status;
	DJ_UINT8 *ether_frame;
  	dj_arp_hdr *arphdr;

  	// Allocate memory for various arrays.
  	ether_frame = (DJ_UINT8 *)dj_mem_malloc(IP_MAXPACKET);

  	// Submit request for a raw socket descriptor.
  	if ((sd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0) 
  	{
    	dj_printf("[DJ_ARP] create socket failed.\n");
    	return DJ_FAILURE;
  	}

  	// Listen for incoming ethernet frame from socket sd.
 	// We expect an ARP ethernet frame of the form:
  	// MAC (6 bytes) + MAC (6 bytes) + ethernet type (2 bytes)
  	//   + ethernet data (ARP header) (28 bytes)
  	// Keep at it until we get an ARP reply.
  	arphdr = (dj_arp_hdr *)(ether_frame + 6 + 6 + 2);
  	while (((((ether_frame[12]) << 8) + ether_frame[13]) != ETH_P_ARP) || (ntohs (arphdr->opcode) != ARPOP_REPLY)) 
  	{
    	if ((status = recv(sd, ether_frame, IP_MAXPACKET, 0)) < 0) 
		{
      		if (errno == EINTR) 
	  		{
        		memset(ether_frame, 0, IP_MAXPACKET * sizeof (DJ_UINT8));
        		continue;
      		} 
	  		else 
      		{
        		dj_printf("[DJ_ARP] recv failed.\n");
        		return -1;
      		}
    	}
  	}
  	close (sd);

  	// Print out contents of received ethernet frame.
  	dj_printf ("\nEthernet frame header:\n");
  	dj_printf ("Destination MAC (this node): ");
  	for (i=0; i<5; i++) 
  	{
  		dj_printf("%02x:", ether_frame[i]);
  	}
  	dj_printf("%02x\n", ether_frame[5]);
  	dj_printf("Source MAC: ");
  	for (i=0; i<5; i++) 
  	{
    	dj_printf("%02x:", ether_frame[i+6]);
  	}
  	dj_printf("%02x\n", ether_frame[11]);
  	// Next is ethernet type code (ETH_P_ARP for ARP).
  	dj_printf("Ethernet type code (2054 = ARP): %u\n", ((ether_frame[12]) << 8) + ether_frame[13]);
  	dj_printf("\nEthernet data (ARP header):\n");
  	dj_printf("Hardware type (1 = ethernet (10 Mb)): %u\n", ntohs (arphdr->htype));
  	dj_printf("Protocol type (2048 for IPv4 addresses): %u\n", ntohs (arphdr->ptype));
  	dj_printf("Hardware (MAC) address length (bytes): %u\n", arphdr->hlen);
  	dj_printf("Protocol (IPv4) address length (bytes): %u\n", arphdr->plen);
  	dj_printf("Opcode (2 = ARP reply): %u\n", ntohs (arphdr->opcode));
  	dj_printf("Sender hardware (MAC) address: ");
  	for (i=0; i<5; i++) 
  	{
    	dj_printf("%02x:", arphdr->sender_mac[i]);
  	}
  	dj_printf("%02x\n", arphdr->sender_mac[5]);
  	dj_printf("Sender protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr->sender_ip[0], arphdr->sender_ip[1], arphdr->sender_ip[2], arphdr->sender_ip[3]);
  	dj_printf("Target (this node) hardware (MAC) address: ");
  	for (i=0; i<5; i++) 
  	{
    	dj_printf("%02x:", arphdr->target_mac[i]);
  	}
  	dj_printf("%02x\n", arphdr->target_mac[5]);
  	dj_printf("Target (this node) protocol (IPv4) address: %u.%u.%u.%u\n",
    arphdr->target_ip[0], arphdr->target_ip[1], arphdr->target_ip[2], arphdr->target_ip[3]);

  	dj_mem_free((DJ_INT8 *)ether_frame);

  	return 0;
}


