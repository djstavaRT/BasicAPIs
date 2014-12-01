/*
 * =====================================================================================
 *
 *       Filename:  sample_syslog.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

#include "dj_base.h"

DJ_INT32 main() 
{
	openlog(NULL, LOG_PID|LOG_CONS, LOG_USER);
 	syslog(LOG_INFO, "A different kind of Hello world ... ");
 	closelog();

 	return DJ_SUCCESS;
}