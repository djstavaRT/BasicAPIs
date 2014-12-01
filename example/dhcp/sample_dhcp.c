/*
 * =====================================================================================
 *
 *       Filename:  sample_dhcp.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_base.h"
#include "dj_trace.h"
#include "dj_dhcp.h"

DJ_INT32 main()
{
	DJ_UINT8 pMac[20];
	DJ_INT8 pIp[20],pGateway[20],pBroadcastAddr[20],pNetmask[20];
	DJ_INT8 *ifaceNum = "eth0";
	DJ_UINT8 *mac = "66:22:33:44:55:00";
	DJ_INT8 *netmask = "255.255.255.0";
	DJ_INT8 *gateway = "192.168.1.1";
	DJ_INT8 *ip = "192.168.1.211";

	dj_network_getMacAddr(ifaceNum,pMac);

	dj_network_setMacAddr(ifaceNum,mac);

	dj_network_getIPAddr(ifaceNum,pIp);

	dj_network_getNetmask(ifaceNum,pNetmask);

	dj_network_setNetmask(ifaceNum,netmask);

	dj_network_setGateway(gateway);

	dj_network_getGatewayByRoute(pGateway);
	//dj_network_getGatewayBySocket(pGateway);

	dj_network_setIPAddr(ifaceNum,ip);

	dj_network_getBroadcastAddr(ifaceNum,pBroadcastAddr);
	
	return DJ_SUCCESS;
}
