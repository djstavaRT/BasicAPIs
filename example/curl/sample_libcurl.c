/*
 * =====================================================================================
 *
 *       Filename:  sample_libcurl.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dj_base.h"
#include "dj_trace.h"

#include "dj_libcurl.h"
 
DJ_INT32 main()
{
	DJ_INT8 *url = "http://192.168.1.211/iploader/15/4096/20130912_cos1007.zip"; 
	DJ_INT8 *filePath = "/home/djstava/Downloads/test.dj";

	//dj_libcurl_download(url,filePath);

	sleep(2);

	DJ_INT8 *upUrl = "ftp://192.168.1.211/test.txt";
	DJ_INT8 *localFile = "/home/djstava/tools/test.txt";
	DJ_INT8 *usrPwd = "djstava:d";

	dj_libcurl_upload_ftp(localFile,upUrl,usrPwd);

	dj_libcurl_download_ftp("djstava.txt",upUrl,usrPwd);
	
	return DJ_SUCCESS;
}

