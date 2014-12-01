/*
 * =====================================================================================
 *
 *       Filename:  dj_dhcp.h
 *       Description: 
 *       Author:    djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_DHCP_H
#define _DJ_DHCP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <net/if_arp.h>
#include <sys/sysctl.h>
#include <linux/route.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <assert.h>

#include "dj_base.h"

DJ_INT32 dj_network_if_ipv4_addr_valid(DJ_INT8 *pAddr);
DJ_INT32 dj_network_getMacAddr(DJ_INT8 *ifaceNum,DJ_UINT8 *pMacAddr);
DJ_INT32 dj_network_setMacAddr(DJ_INT8 *ifaceNum,DJ_UINT8 *pMacAddr);
DJ_INT32 dj_network_getGatewayByRoute(DJ_INT8 *pGateway);
DJ_INT32 dj_network_getGatewayBySocket(DJ_INT8 *pGateway);
DJ_INT32 dj_network_setGateway(DJ_INT8 *pGateway);
DJ_INT32 dj_network_getNetmask(DJ_INT8 *ifaceNum,DJ_INT8 *pNetmask);   
DJ_INT32 dj_network_setNetmask(DJ_INT8 *ifaceNum,DJ_INT8 *pNetmask);
DJ_INT32 dj_network_getBroadcastAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pBroadcastAddr);
DJ_INT32 dj_network_getIPAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pIPAddr);
DJ_INT32 dj_network_setIPAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pIPAddr);

#endif
