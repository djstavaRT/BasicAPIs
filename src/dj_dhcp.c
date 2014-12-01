/*
 * =====================================================================================
 *
 *       Filename:  dj_dhcp.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_dhcp.h"
#include "dj_trace.h"
#include "dj_base.h"
#include "dj_mem.h"

/*
**  define Macros
*/
#define IPV4ETHMAX 20
#define BUFSIZE 8192

struct route_info 
{
    struct in_addr dstAddr;
    struct in_addr srcAddr;
    struct in_addr gateWay;
    DJ_INT8 ifName[10];
};


/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_if_ipv4_addr_valid
 *      Description: 
 *	  Param: DJ_INT8 *pAddr
 *      Return: true , DJ_SUCCESS
 *			false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_if_ipv4_addr_valid(DJ_INT8 *pAddr)
{ 
	in_addr_t ret;
 
	ret = inet_network(pAddr); 
	if (ret == -1) 
	{
		dj_printf("%s is not a valid IPv4 address.\n", pAddr); 
		return DJ_FAILURE;
	}

	return DJ_SUCCESS;
}

 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_getMacAddress
 *      Description: 
 *	  Params: 
 *		1 DJ_INT8 *ifaceNum
 *		2 DJ_UINT8 *pMacAddr
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_getMacAddr(DJ_INT8 *ifaceNum,DJ_UINT8 *pMacAddr)
{
	struct ifreq ifr;
	DJ_INT32 sockfd;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name, ifaceNum, IFNAMSIZ - 1);
	 
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
	{
		dj_printf("[DJ_DHCP] ioctl error.\n");
		return DJ_FAILURE;
	}
		 
	strcpy((DJ_INT8 *)pMacAddr,ifr.ifr_hwaddr.sa_data);
	dj_printf("[DJ_DHCP] mac address: %x:%x:%x:%x:%x:%x\n", pMacAddr[0],pMacAddr[1],pMacAddr[2],pMacAddr[3],pMacAddr[4],pMacAddr[5]);

	close(sockfd);
	return DJ_SUCCESS;
}

 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_setMacAddress
 *      Description: 
 *	  Params: 
 *		1 DJ_INT8 *ifaceNum
 *		2 DJ_INT8 *macAddr
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_setMacAddr(DJ_INT8 *ifaceNum,DJ_UINT8 *pMacAddr)
{
	struct ifreq ifr;
    DJ_INT32 sockfd;

    sscanf((DJ_INT8 *)pMacAddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &ifr.ifr_hwaddr.sa_data[0],
        &ifr.ifr_hwaddr.sa_data[1],
        &ifr.ifr_hwaddr.sa_data[2],
        &ifr.ifr_hwaddr.sa_data[3],
        &ifr.ifr_hwaddr.sa_data[4],
        &ifr.ifr_hwaddr.sa_data[5]
        );

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);
 
    strcpy(ifr.ifr_name, ifaceNum);
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    assert(ioctl(sockfd, SIOCSIFHWADDR, &ifr) != -1);
 	close(sockfd);
	
    return DJ_SUCCESS;
}

 /*=========================FUNCTION===================================================== 
  *
  * 	 Name: dj_network_get_gatewayByRoute
  * 	 Description: 
  *    Params: DJ_INT8 *pGateway
  * 	 Return: true , DJ_SUCCESS
  * 		  	false , DJ_FAILURE
  *
  *======================================================================================
  */
DJ_INT32 dj_network_getGatewayByRoute(DJ_INT8 *pGateway)
{
	DJ_INT8 buff[256];
	DJ_INT32 nl = 0;
	struct in_addr dest;
	struct in_addr gw;
	DJ_INT32 flgs, ref, use, metric;
	DJ_ULONG d, g, m;
	DJ_INT32 find_default_flag = 0;
	DJ_INT8 sgw[16];

	FILE *fp = fopen("/proc/net/route", "r");
	assert(fp != NULL);

	while(fgets(buff, sizeof (buff), fp) != NULL)
	{
		if (nl)
		{
			int ifl = 0;
			while (buff[ifl] != ' ' && buff[ifl] != '\t' && buff[ifl] != '\0')
				ifl++;
			buff[ifl] = 0; /* interface */
			if (sscanf(buff + ifl + 1, "%lx%lx%X%d%d%d%lx",&d, &g, &flgs, &ref, &use, &metric, &m) != 7)
			{
				fclose(fp);
				return -1;
			}

			if (flgs & RTF_UP)
			{
				dest.s_addr = d;
				gw.s_addr = g;
				strcpy(sgw, (gw.s_addr == 0 ? "" : (DJ_INT8 *) inet_ntoa(gw)));

				if (dest.s_addr == 0)
				{
					find_default_flag = 1;
					memcpy(pGateway, sgw, strlen(sgw));
					dj_printf("[DJ_DHCP] gateway:%s\n", pGateway);
					break;
				}
			}
		}
		nl++;
	}
	fclose(fp);

	if (find_default_flag == 1)
		return DJ_SUCCESS;
	else
		return DJ_FAILURE;
}


 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_getGatewayBySocket
 *      Description: 
 *	  Params: DJ_INT8 *pGateway
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_getGatewayBySocket(DJ_INT8 *pGateway)
{
#if 0
	struct nlmsghdr *nlMsg;
    struct rtmsg *rtMsg;
    struct route_info *rtInfo;
	struct nlmsghdr *nlHdr;
    struct rtattr *rtAttr;
    DJ_INT32 rtLen;
    DJ_INT32 readLen = 0, msgLen = 0;
    DJ_INT8 msgBuf[BUFSIZE];
	DJ_INT8 *msgTmp = &msgBuf[0];
    DJ_INT32 sockfd, len, msgSeq = 0;

	/* Create Socket */
    if ((sockfd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
    {
        dj_printf("[DJ_DHCP] Socket create failed.\n");
		return DJ_FAILURE;
    }
    memset(msgBuf, 0, BUFSIZE);

	/* point the header and the msg structure pointers into the buffer */
    nlMsg = (struct nlmsghdr *) msgBuf;
    rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

	/* Fill in the nlmsg header */
    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));  // Length of message.
    nlMsg->nlmsg_type = RTM_GETROUTE;   // Get the routes from kernel routing table .

    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;    // The message is a request for dump.
    nlMsg->nlmsg_seq = msgSeq++;    // Sequence of the message packet.
    nlMsg->nlmsg_pid = getpid();    // PID of process sending the request.

	/* Send the request */
    if (send(sockfd, nlMsg, nlMsg->nlmsg_len, 0) < 0) 
	{
        dj_printf("[DJ_DHCP] Write To Socket Failed.\n");
        return DJ_FAILURE;
    }

	do {
    	/* Recieve response from the kernel */
        //if((readLen = recv(sockfd, msgBuf, BUFSIZE - msgLen, 0)) < 0) 
        if((readLen = recv(sockfd, msgTmp, BUFSIZE - msgLen, 0)) < 0)
		{
            dj_printf("[DJ_DHCP] SOCK recv error.\n");
            return DJ_FAILURE;
        }

        //nlHdr = (struct nlmsghdr *)msgBuf;
        nlHdr = (struct nlmsghdr *)msgTmp;

    	/* Check if the header is valid */
        if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) 
        {
            dj_printf("[DJ_DHCP] Error in recieved packet.\n");
            return DJ_FAILURE;
        }

    	/* Check if the its the last message */
        if(nlHdr->nlmsg_type == NLMSG_DONE) 
		{
            break;
        } 
		else 
        {
    		/* Else move the pointer to buffer appropriately */
            //msgBuf += readLen;
            msgTmp += readLen;
            msgLen += readLen;
        }

    	/* Check if its a multi part message */
        if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) 
		{
           /* return if its not */
            break;
        }
    } while((nlHdr->nlmsg_seq != msgSeq) || (nlHdr->nlmsg_pid != getpid()));

	/* Parse and print the response */
    rtInfo = (struct route_info *) malloc(sizeof(struct route_info));
    for(; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) 
	{
        memset(rtInfo, 0, sizeof(struct route_info));
        //parseRoutes(nlMsg, rtInfo);
        rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

		/* If the route is not for AF_INET or does not belong to main routing table then return. */
    	if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
        	return DJ_FAILURE;

		/* get the rtattr field */
    	rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
    	rtLen = RTM_PAYLOAD(nlHdr);
    	for(; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen)) 
		{
        	switch(rtAttr->rta_type) 
			{
        		case RTA_OIF:
            		if_indextoname(*(DJ_INT32 *) RTA_DATA(rtAttr), rtInfo->ifName);
            		break;
					
        		case RTA_GATEWAY:
            		rtInfo->gateWay.s_addr = *(DJ_UINT32 *) RTA_DATA(rtAttr);
            		break;
					
        		case RTA_PREFSRC:
            		rtInfo->srcAddr.s_addr = *(DJ_UINT32 *) RTA_DATA(rtAttr);
            		break;
					
        		case RTA_DST:
            		rtInfo->dstAddr.s_addr= *(DJ_UINT32 *) RTA_DATA(rtAttr);
            		break;
        	}
    	}
		strcpy(pGateway,(DJ_INT8 *)inet_ntoa(rtInfo->gateWay));
    	dj_printf("[DJ_DHCP] get gateway:%s\n", inet_ntoa(rtInfo->gateWay));
		dj_printf("[DJ_DHCP] get gateway:%s\n", pGateway);

	}
	
    free(rtInfo);
    close(sockfd);
	
    return DJ_SUCCESS;
#endif
	return DJ_SUCCESS;
}

 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_set_gateway
 *      Description: 
 *	  Params: DJ_INT8 *pGateway
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_setGateway(DJ_INT8 *pGateway)
{
    struct rtentry r;
    struct in_addr *in_address = (struct in_addr *)dj_mem_malloc(sizeof(struct in_addr));
    struct in_addr *in_dst = (struct in_addr *)dj_mem_malloc(sizeof(struct in_addr));
    struct in_addr *in_netmask = (struct in_addr *)dj_mem_malloc(sizeof(struct in_addr));
    DJ_INT32 sockfd;
	
    inet_aton(pGateway, in_address);
    inet_aton("0.0.0.0", in_dst);
    inet_aton("0.0.0.0", in_netmask);

    memset(&r, 0, sizeof(r));
    r.rt_flags = RTF_UP | RTF_GATEWAY;    /* RTF_HOST not set */

    /* Create a channel to the NET kernel. */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        dj_printf("[DJ_DHCP] socket() failed.\n");
        return DJ_FAILURE;
    }

    r.rt_dst.sa_family = AF_INET;
    r.rt_gateway.sa_family = AF_INET;
    r.rt_genmask.sa_family = AF_INET;
    ((struct sockaddr_in *) &r.rt_dst)->sin_addr.s_addr = in_dst->s_addr;
    ((struct sockaddr_in *) &r.rt_gateway)->sin_addr.s_addr = in_address->s_addr;
    ((struct sockaddr_in *) &r.rt_genmask)->sin_addr.s_addr = in_netmask->s_addr;

    if (ioctl(sockfd, SIOCADDRT, (void *) &r) < 0)
    {
        dj_printf("[DJ_DHCP] set gateway ioctl failed.\n");
		dj_mem_free((DJ_INT8 *)in_address);
		dj_mem_free((DJ_INT8 *)in_dst);
		dj_mem_free((DJ_INT8 *)in_netmask);
        close(sockfd);
        return DJ_FAILURE;
    }

	dj_mem_free((DJ_INT8 *)in_address);
	dj_mem_free((DJ_INT8 *)in_dst);
	dj_mem_free((DJ_INT8 *)in_netmask);
	close (sockfd);
    return DJ_SUCCESS;
}

 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_getNetMask
 *      Description: get netmask
 *	  Params: 
 *		1 DJ_INT8 *ifaceNum
 *		2 DJ_INT8 *pNetmask
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_getNetmask(DJ_INT8 *ifaceNum,DJ_INT8 *pNetmask)   
{   
    DJ_INT32 sockfd;   
  
    struct ifreq ifr_mask;   
    struct sockaddr_in *net_mask;   
           
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );   
    if( sockfd == -1)   
    {   
        dj_printf("[DJ_DHCP] create socket failed.\n");   
        return DJ_FAILURE;   
    }   
       
    memset(&ifr_mask, 0, sizeof(ifr_mask));      
    strncpy(ifr_mask.ifr_name, ifaceNum, sizeof(ifr_mask.ifr_name )-1);      
  
    if( (ioctl( sockfd, SIOCGIFNETMASK, &ifr_mask ) ) < 0 )    
    {   
        dj_printf("[DJ_DHCP] ioctl error.\n");   
        return DJ_FAILURE;   
    }   
       
    net_mask = ( struct sockaddr_in * )&( ifr_mask.ifr_netmask );   
    strcpy( pNetmask, inet_ntoa( net_mask -> sin_addr ) );   
       
    dj_printf("[DJ_DHCP] netmask:%s\n",pNetmask);     
       
    close(sockfd);   
    return DJ_SUCCESS;   
} 

 /*=========================FUNCTION===================================================== 
 *
 *      Name: dj_network_setNetmask
 *      Description: 
 *	  Params: 
 *		1 DJ_INT8 *ifaceNum
 *		2 DJ_INT8 *pNetmask
 *      Return: true , DJ_SUCCESS
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_network_setNetmask(DJ_INT8 *ifaceNum,DJ_INT8 *pNetmask) 
{
	DJ_INT32 sockfd; 
    struct ifreq ifr;
    struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
    memset(&ifr, 0, sizeof(ifr));
    sin->sin_family = AF_INET;

	sockfd = socket( AF_INET, SOCK_DGRAM, 0);
	assert(sockfd != -1);
	
    if ( !inet_pton(AF_INET, pNetmask, &sin->sin_addr) )
    {
        dj_printf("[DJ_DHCP] failed to convert netmask.\n");
		close(sockfd);
        return DJ_FAILURE;
    }
    strncpy( ifr.ifr_name, ifaceNum, IFNAMSIZ-1 );
    if ( ioctl(sockfd,SIOCSIFNETMASK,&ifr) == -1 )
    {
        dj_printf("could not read interface");
		close(sockfd);
        return DJ_FAILURE;
    }
	
	close(sockfd);
	return DJ_SUCCESS;

}

/*=========================FUNCTION===================================================== 
*
*	   Name: dj_network_getBroadcastAddr
*	   Description: 
*	   Params: 
*		1 DJ_INT8 *ifaceNum
*		2 DJ_INT8 *pBroadcastAddr
*	   Return: true , DJ_SUCCESS
*			  false , DJ_FAILURE
*
*======================================================================================
*/
DJ_INT32 dj_network_getBroadcastAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pBroadcastAddr)
{
	struct ifreq ifr;
	DJ_INT32 sockfd;
	struct sockaddr_in *saddr;

	if((sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    	return DJ_FAILURE;
	memset(&ifr, 0, sizeof(ifr));
	
	strcpy(ifr.ifr_name, ifaceNum);
	ifr.ifr_addr.sa_family = AF_INET;
	
	if(ioctl(sockfd, SIOCGIFBRDADDR, &ifr)<0)
    	return DJ_FAILURE;
	
	saddr = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(pBroadcastAddr, inet_ntoa(saddr->sin_addr));
	dj_printf("[DJ_DHCP] broadcast addr: %s\n",pBroadcastAddr);
	return DJ_SUCCESS;
}

/*=========================FUNCTION===================================================== 
*
*	   Name: dj_network_getIPAddr
*	   Description: 
*	   Params: 
*		1 DJ_INT8 *ifaceNum
*		2 DJ_INT8 *pIPAddr
*	   Return: true , DJ_SUCCESS
*			  false , DJ_FAILURE
*
*======================================================================================
*/
DJ_INT32 dj_network_getIPAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pIPAddr)
{
	struct sockaddr_in *addr;
	struct ifreq ifr;
	DJ_INT32 sockfd;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name, ifaceNum, IFNAMSIZ - 1);

	if (ioctl(sockfd, SIOCGIFADDR, &ifr) == -1)
	{
		dj_printf("[DJ_DHCP] ioctl failed.\n");
		return DJ_FAILURE;
	}
	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	pIPAddr = inet_ntoa(addr->sin_addr);	
	dj_printf("[DJ_DHCP] ip addr: %s\n", pIPAddr);

	close(sockfd);
	return DJ_SUCCESS;
}

/*=========================FUNCTION===================================================== 
*
*	   Name: dj_network_setIPAddr
*	   Description: 
*	 Params: DJ_INT8 *pIPAddr
*	   Return: true , DJ_SUCCESS
*			  false , DJ_FAILURE
*
*======================================================================================
*/
DJ_INT32 dj_network_setIPAddr(DJ_INT8 *ifaceNum,DJ_INT8 *pIPAddr)
{
	DJ_INT32 sockfd;
	struct ifreq ifr;
	struct sockaddr_in addr;
	
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
       return DJ_FAILURE;
    }
	   
    bzero((DJ_INT8 *)&ifr, sizeof(ifr));
    strcpy(ifr.ifr_name,ifaceNum);
    
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    inet_aton(pIPAddr,&addr.sin_addr);
    memcpy((char*)&ifr.ifr_ifru.ifru_addr, (char*)&addr, sizeof(struct sockaddr_in));
    if(ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
    {
       close(sockfd);
       return DJ_FAILURE;
    }
    close(sockfd);
    return DJ_SUCCESS;
}
