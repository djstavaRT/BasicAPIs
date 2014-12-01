/*
 * =====================================================================================
 *
 *       Filename:  dj_string.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

 #include "dj_string.h"
 #include "dj_base.h"
 #include "dj_trace.h"

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_string_stringtoi
 *      Description: 
 *	        Param: DJ_INT8 s[]
 *      Return: DJ_INT32;
 *
 *======================================================================================
 */
DJ_INT32 dj_string_stringtoi(DJ_INT8 s[])  
{  
    DJ_INT32 i;  
    DJ_INT32 n = 0; 
	
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))  
    {  
        i = 2;  
    }  
    else  
    {  
        i = 0;  
    }  
	
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)  
    {  
        if (tolower(s[i]) > '9')  
        {  
            n = 16 * n + (10 + tolower(s[i]) - 'a');  
        }  
        else  
        {  
            n = 16 * n + (tolower(s[i]) - '0');  
        }  
    }  
	
    return n;  
}  


/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_string_reverse
 *      Description: 
 *	    Param: 
 *          DJ_INT8 *str
 *      Return: DJ_UINT8 *;
 *
 *======================================================================================
 */
DJ_UINT8 *dj_string_reverse(DJ_UINT8 *str)
{
      DJ_UINT8 *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
	  
      return str;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_string_showProgress
 *      Description: Show prgress on stdout,for example,download/upload
 *	    Param: 
            DJ_INT8 symbol: the character that would be showing 
            DJ_INT32 length: 
 *      Return: DJ_UINT8 *;
 *
 *======================================================================================
 */
DJ_VOID dj_string_showProgress(DJ_INT8 symbol,DJ_INT32 length)
{
	DJ_INT32 i = 0;
    DJ_INT8 *str = (DJ_INT8 *)malloc(sizeof(DJ_INT8) * (length + 1));
    
    for (i; i < length + 1; ++i)
    {
        str[i] = symbol;
    }

    for(i;i<length + 1;++i)
    {
	    printf("Progress: [%s]%d%%\r",str + length - i,i + 1);
	    fflush(stdout);
	    usleep(100000);
    }
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_string_rstrchr
 *      Description: strchr from the right of the src
 *	    Param: 
 *          DJ_INT8 * src: src string 
 *          DJ_INT8 x: target character
 *      Return: DJ_UINT8 *;
 *
 *======================================================================================
 */
DJ_INT8* dj_string_rstrchr(DJ_INT8 * src, DJ_INT8 x)  
{
	DJ_INT32 i = strlen(src);
    if(!(*src))  
  	    return 0;
    while(src[i-1]) 
    {
  	    if(strchr(src + (i - 1), x))  
		    return (src + (i - 1));  
	    else 
			i--;
    }
  
    return 0;
}

/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_string_toLowerCase
 *      Description: toLowerCase
 *	    Param: 
 *          DJ_INT8 *src; 
 *      Return: 
 *          DJ_VOID;
 *
 *======================================================================================
 */
DJ_VOID dj_string_toLowerCase(DJ_INT8 *src)  
{
    while(*src)  
    {
  	    *src = tolower(*src);
        src++;
    }
}