/*
 * =====================================================================================
 *
 *       Filename:  dj_mem.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_mem.h"
#include "dj_base.h"
#include "dj_trace.h"

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_mem_malloc
 *      Description: 
 *	  Param:  DJ_UINT32
 *      Return: true , return DJ_INT8*
 *			 false , DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT8 *dj_mem_malloc (DJ_UINT32 len)
{
	DJ_INT8 *tmp;

  	tmp = (DJ_INT8 *) malloc (len * sizeof (DJ_INT8));
  	if (tmp != NULL) 
  	{
  		dj_printf("[DJ_MEM] malloc buff success.\n");
  	} 
  	else 
  	{
    	dj_printf("[DJ_MEM] Cannot malloc memory.\n");
  	}
	return tmp;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_mem_free
 *      Description: free memory
 *	  Param:  DJ_INT8*
 *      Return: DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_mem_free(DJ_INT8* pBuf)
{
	free(pBuf);
	pBuf = NULL;
	dj_printf("[DJ_MEM] free buff success.\n");
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_mem_calloc
 *      Description:
 *	  Params:  
 *		1 DJ_UINT32 nmemb
 *		2 DJ_UINT32 size
 *      Return: DJ_INT8 *
 *
 *======================================================================================
 */
DJ_INT8 *dj_mem_calloc(DJ_UINT32 num, DJ_UINT32 size)
{
	DJ_INT8 *tmp;

  	tmp = (DJ_INT8 *)calloc(num,size);
  	if (tmp != NULL) 
  	{
  		dj_printf("[DJ_MEM] calloc buff success.\n");
  	} 
  	else 
  	{
    	dj_printf("[DJ_MEM] Cannot calloc memory.\n");
  	}
	return tmp;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_mem_realloc
 *      Description:
 *	  Params:  
 *		1 DJ_INT8 *ptr
 *		2 DJ_UINT32 size
 *      Return: DJ_INT8 *
 *
 *======================================================================================
 */
DJ_INT8 *dj_mem_realloc(DJ_INT8 *ptr, DJ_UINT32 size)
{
	DJ_INT8 *tmp;

  	tmp = (DJ_INT8 *)realloc(ptr,size);
  	if (tmp != NULL) 
  	{
  		dj_printf("[DJ_MEM] realloc buff success.\n");
  	} 
  	else 
  	{
    	dj_printf("[DJ_MEM] Cannot realloc memory.\n");
  	}
	return tmp;
}
