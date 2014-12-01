/*
 * =====================================================================================
 *
 *       Filename:  sample_arp.c
 *       Description:   arp sample
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "dj_base.h"
#include "dj_trace.h"
#include "dj_arp.h"

int main()
{
	DJ_INT32 ret;
	DJ_INT8 *ifaceNum = "eth0";
	DJ_INT8 *srcAddr = "192.168.1.211";
	DJ_INT8 *dstAddr = "192.168.1.240";
	
	ret = dj_arp_send_request(ifaceNum, srcAddr,dstAddr);
	if(DJ_SUCCESS == ret)
		dj_printf("dj_arp_send_request success.\n");
	else
		dj_printf("dj_arp_send_request failed.\n");

	dj_arp_get_reply();
	
	return ret;
}
