/*
 * =====================================================================================
 *
 *       Filename:  dj_trace.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_trace.h"
#include "dj_base.h"

/*
**  Macros
*/
#define MAX_TRACE_LEN (512)

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_printf
 *      Description: print statement
 *	  Params: 
 *      	1 string
 *      Return:  DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_printf(const DJ_INT8* string, ...)
{
	DJ_INT8 trace_buffer[MAX_TRACE_LEN] = {0};
	va_list args;

	va_start(args, string);
	vsprintf(trace_buffer, string, args);
	va_end(args);
 
	printf("%s", trace_buffer);
}
