/*
 * =====================================================================================
 *
 *       Filename:  dj_string.h
 *       Description: 
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_STRING_H
#define _DJ_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <ctype.h>

#include "dj_base.h"

DJ_INT32 dj_string_stringtoi(DJ_INT8 s[]);  
DJ_UINT8 *dj_string_reverse(DJ_UINT8 *str);
DJ_VOID dj_string_showProgress(DJ_INT8 symbol,DJ_INT32 length);
DJ_INT8* dj_string_rstrchr(DJ_INT8 * src, DJ_INT8 x);
DJ_VOID dj_string_toLowerCase(DJ_INT8 *src);

#endif
