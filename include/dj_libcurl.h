/*
 * =====================================================================================
 *
 *       Filename:  dj_libcurl.h
 *       Description: 
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#ifndef _DJ_LIBCURL_H
#define _DJ_LIBCURL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#include "dj_base.h"
#include "libcurl/curl.h"
#include "libcurl/easy.h"

/*
**  Macros
*/
#define CONNECT_TIMEOUT	30
#define DOWNLOAD_TIMEOUT 900

size_t dj_libcurl_download_processRoutine(DJ_VOID *buffer, size_t size, size_t nmemb, DJ_VOID *user_p);
DJ_INT32 dj_libcurl_download(DJ_INT8 *url,DJ_INT8 *filePath);
DJ_INT32 dj_libcurl_download_ftp(DJ_INT8 *fileName,DJ_INT8 *url,DJ_INT8 *userPwd);

DJ_INT32 dj_libcurl_upload_ftp(DJ_INT8 *fileName,DJ_INT8 *url,DJ_INT8 *userPwd);
DJ_UINT32 dj_libcurl_upload_ftp_getLocalFileSize(FILE *fp);

#endif
