/*
 * =====================================================================================
 *
 *       Filename:  dj_number.h
 *       Description: 
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_NUMBER_H
#define _DJ_NUMBER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#include "dj_base.h"

DJ_VOID dj_number_quickSort(DJ_INT32 array[],DJ_INT32 left,DJ_INT32 right);
DJ_VOID dj_number_bubbleSort(DJ_INT32 array[],DJ_INT32 count);
DJ_VOID dj_number_selectSort(DJ_INT32 array[],DJ_INT32 count);
DJ_VOID dj_number_insertSort(DJ_INT32 array[],DJ_INT32 count);

#endif
