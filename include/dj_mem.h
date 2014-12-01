/*
 * =====================================================================================
 *
 *       Filename:  dj_mem.h
 *       Description: memory operations  
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_MEM_H
#define _DJ_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#include "dj_base.h"

DJ_INT8 *dj_mem_malloc (DJ_UINT32 len);
DJ_VOID dj_mem_free(DJ_INT8* pBuf);
DJ_INT8 *dj_mem_calloc(DJ_UINT32 num, DJ_UINT32 size);
DJ_INT8 *dj_mem_realloc(DJ_INT8 *pBuf, DJ_UINT32 size);

#endif
