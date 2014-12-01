/*
 * =====================================================================================
 *
 *       Filename:  dj_file.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://www.macernow.com
 *
 * =====================================================================================
 */

#include "dj_file.h"
#include "dj_trace.h"

/*=========================FUNCTION=====================================================
 *
 *      Name: dj_file_check_file_exist
 *      Description: 
 *      Param: 
 *          DJ_INT8 *pfile
 *      Return: 
 *          success: DJ_SUCCESS
 *          fail   : DJ_FAILURE
 *
 *======================================================================================
 */
DJ_INT32 dj_file_check_file_exist(DJ_INT8 *pfile)
{
	if (0 != access(pfile, F_OK))
	{
		return DJ_FAILURE;
	}
	return DJ_SUCCESS;
}