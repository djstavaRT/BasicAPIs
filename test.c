/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *       Description:  
 *       Version:   1.0
 *       Created:   2014年08月21日 17时20分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:    djstava , djstava@gmail.com
 *       Company:   ABC Inc
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>

void dj_string_toLowerCase(char *src)  
{
    while(*src)  
    {
        printf("%c\n",*src);
        *src = tolower(*src);
        src++;
    }
}

char* dj_string_rstrchr(char * src, char x)  
{
    int i = strlen(src);
    printf("str length = %d,ch = %c\n",i,x);
    if(!(*src))  
        return 0;
    while(src[i-1]) 
    {
        if(strchr(src + (i - 1), x) != NULL)  
            return (src + (i - 1));  
        else 
            i--;
    }
    printf("str1 length = %d\n",i);
                  
    return 0;
}

int main()
{
    char str[] = "heLlO";
    char *tmp;

    dj_string_toLowerCase(str);
    printf("%s\n",str);

    tmp = dj_string_rstrchr(str,'d');
    printf("%s\n",tmp);
}
