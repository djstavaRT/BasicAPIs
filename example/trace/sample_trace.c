/*
 * =====================================================================================
 *
 *       Filename:  sample_trace.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_base.h"
#include "dj_trace.h"
#include "dj_version.h"

DJ_INT32 main()
{
	dj_printf("This is a text for trace.\n");
	dj_printf_libdjstava_version();
	return DJ_SUCCESS;
}