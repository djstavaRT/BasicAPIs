/*
 * =====================================================================================
 *
 *       Filename:  dj_arp.h
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_ARP_H_
#define _DJ_ARP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>          
#include <string.h>         
#include <netdb.h>           
#include <sys/types.h>     
#include <sys/socket.h>     
#include <netinet/in.h>  
#include <netinet/ip.h>   
#include <arpa/inet.h>     
#include <sys/ioctl.h>      
#include <bits/ioctls.h>   
#include <net/if.h>       
#include <linux/if_ether.h>  
#include <linux/if_packet.h> 
#include <net/ethernet.h>
#include <errno.h>  

#include "dj_base.h"

// Define an struct for ARP header
typedef struct _arp_hdr dj_arp_hdr;
struct _arp_hdr 
{
  DJ_UINT16 htype;
  DJ_UINT16 ptype;
  DJ_UINT8 hlen;
  DJ_UINT8 plen;
  DJ_UINT16 opcode;
  DJ_UINT8 sender_mac[6];
  DJ_UINT8 sender_ip[4];
  DJ_UINT8 target_mac[6];
  DJ_UINT8 target_ip[4];
};

DJ_INT32 dj_arp_send_request(DJ_INT8 *ifaceNum,DJ_INT8 *srcAddr,DJ_INT8 *dstAddr);
DJ_INT32 dj_arp_get_reply ();

#endif
