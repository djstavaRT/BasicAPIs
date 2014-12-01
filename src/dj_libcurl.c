/*
 * =====================================================================================
 *
 *       Filename:  dj_libcurl.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_base.h"
#include "dj_trace.h"
#include "dj_libcurl.h"

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_libcurl_download
 *      Description:
 *	  Params:  
 *		1 DJ_INT8 *url
 *		2 DJ_INT8 *filePath,local path to save the download file,including filename
 *      Return: DJ_INT32
 *
 *======================================================================================
 */ 
DJ_INT32 dj_libcurl_download(DJ_INT8 *url,DJ_INT8 *filePath)
{
	DJ_INT32 ret = DJ_SUCCESS;
	FILE *fp = NULL;
	CURL *curl;

	fp = fopen(filePath,"wb");
	if(fp == NULL)
	{
		dj_printf("[DJ_LIBCURL] open file <%s> failed.\n",filePath);
	}
	
	curl_global_init (CURL_GLOBAL_ALL);
	curl = curl_easy_init(); 
	if (!curl) 
	{ 
		printf("[DJ_LIBCURL] init curl failed.\n"); 
		ret = DJ_FAILURE;
		return ret;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &dj_libcurl_download_processRoutine);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	//curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, curl); 
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIMEOUT); 
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, DOWNLOAD_TIMEOUT);
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
	ret = curl_easy_perform(curl);

	fclose(fp);
	dj_printf("[DJ_LIBCURL] curl ret = %d\n",ret);
	
	if(CURLE_OK == ret)
	{
		dj_printf("[DJ_LIBCURL] Download finished.\n");				
	}
	return ret;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_libcurl_download_processRoutine
 *      Description:
 *	  Params:  
 *		1 DJ_INT8 *url
 *		2 DJ_INT8 *filePath
 *      Return: DJ_INT8 *
 *
 *======================================================================================
 */
size_t dj_libcurl_download_processRoutine(DJ_VOID *buffer, size_t size, size_t nmemb, DJ_VOID *user_p)
{
	static DJ_UINT64 totalfsize = 0;
	size_t rSize;
	FILE *fp;
	
	fp = (FILE *)user_p;
	rSize = fwrite(buffer, size, nmemb, fp);
	totalfsize += rSize;
	
	dj_printf("download:%llu\n",totalfsize);
	return rSize;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_libcurl_download_ftp
 *      Description:
 *	  Params:  
 *		1 DJ_INT8 *url
 *		2 DJ_UINT32 size
 *      Return: DJ_INT8 *
 *
 *======================================================================================
 */ 
DJ_INT32 dj_libcurl_download_ftp(DJ_INT8 *fileName,DJ_INT8 *url,DJ_INT8 *userPwd)
{
	DJ_INT32 ret = DJ_SUCCESS;
	FILE *fp = NULL;
	CURL *curl;

	fp = fopen(fileName,"wb");
	if(fp == NULL)
	{
		dj_printf("[DJ_LIBCURL] open local file <%s> failed.\n",fileName);
	}
	
	curl_global_init (CURL_GLOBAL_ALL);
	curl = curl_easy_init(); 
	if (!curl) 
	{ 
		dj_printf("[DJ_LIBCURL] init curl failed.\n"); 
		ret = DJ_FAILURE;
		return ret;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
	ret = curl_easy_perform(curl);

	dj_printf("[DJ_LIBCURL] curl ret = %d\n",ret);
	
	if(CURLE_OK == ret)
	{
		dj_printf("[DJ_LIBCURL] Download finished.\n");				
	}
	fclose(fp);
	return ret;
}


/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_libcurl_upload
 *      Description:
 *	  Params:  
 *		1 DJ_INT8 *url
 *		2 DJ_UINT32 size
 *      Return: DJ_INT8 *
 *
 *======================================================================================
 */ 
DJ_INT32 dj_libcurl_upload_ftp(DJ_INT8 *fileName,DJ_INT8 *url,DJ_INT8 *userPwd)
{
	DJ_INT32 ret = DJ_SUCCESS;
	FILE *fp = NULL;
	CURL *curl;

	fp = fopen(fileName,"r");
	if(fp == NULL)
	{
		dj_printf("[DJ_LIBCURL] open local file <%s> failed.\n",fileName);
	}
	
	curl_global_init (CURL_GLOBAL_ALL);
	curl = curl_easy_init(); 
	if (!curl) 
	{ 
		dj_printf("[DJ_LIBCURL] init curl failed.\n"); 
		ret = DJ_FAILURE;
		return ret;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd);
	curl_easy_setopt(curl, CURLOPT_READDATA, fp);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);	
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, dj_libcurl_upload_ftp_getLocalFileSize(fp));
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
	ret = curl_easy_perform(curl);

	dj_printf("[DJ_LIBCURL] curl ret = %d\n",ret);
	
	if(CURLE_OK == ret)
	{
		dj_printf("[DJ_LIBCURL] upload finished.\n");				
	}
	fclose(fp);
	return ret;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_libcurl_upload_ftp_getLocalFileSize
 *      Description:
 *	  Params:  
 *		1 FILE *fp
 *      Return: DJ_UINT32 
 *
 *======================================================================================
 */
DJ_UINT32 dj_libcurl_upload_ftp_getLocalFileSize(FILE *fp)
{	/*
	DJ_UINT64 size;
	struct stat file_info;

	if(stat(localpath, &file_info) == 0)
      {
      		size =  file_info.st_size;
    	}
    	*/
    DJ_UINT32 size;
	
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);	
	fseek(fp, 0L, SEEK_SET);
	
	return size;
}

