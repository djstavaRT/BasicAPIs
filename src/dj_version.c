/*
 * =====================================================================================
 *
 *       Filename:  dj_version.c
 *       Description: 
 *       Author:    djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_base.h"
#include "dj_trace.h"
#include "dj_version.h"

/*
**  Macros
*/
#define LIBDJSTAVAVERSION "1.0.0"

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_printf_libdjstava_version
 *      Description: printf lib version
 *	  Params: 
 *      	1 string
 *      Return:  DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_printf_libdjstava_version()
{
	dj_printf("\n******************************************************************************\n\n");	
	dj_printf("\tSANY Technology Co.,Ltd,lib Version=%s\n",LIBDJSTAVAVERSION);
	dj_printf("\tSANY Software,build at %s,%s\n\n",__DATE__,__TIME__);
	dj_printf("******************************************************************************\n");

}

