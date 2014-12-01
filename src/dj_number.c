/*
 * =====================================================================================
 *
 *       Filename:  dj_number.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://www.macernow.com
 *
 * =====================================================================================
 */

#include "dj_number.h"
#include "dj_trace.h"


/*=========================FUNCTION=====================================================
 *
 *      Name: dj_string_quickSort
 *      Description: 快速排序
 *      Param: 
 *          DJ_INT32 array[]: 待排序的数组
 *          DJ_INT32 left: 第一个元素的下标，即0
 *          DJ_INT32 right: 最后一个元素的下标，即array.length - 1
 *      Return: DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_number_quickSort(DJ_INT32 array[],DJ_INT32 left,DJ_INT32 right)
{
    DJ_INT32 i,j,tmp;
    
    i = left;
    j = right;
    tmp = array[left];
    
    if(left > right)
        return;
    
    while (i != j) {
        while((array[j] >= tmp) && (j > i))
            j--;
        
        if (j > i) {
            array[i++] = array[j];
        }
        
        while ((array[i] <= tmp) && (j > i)) {
            i++;
        }
        
        if (j > i) {
            array[j--] = array[i];
        }
    }
    
    array = tmp;
    dj_number_quickSort(array,left,i - 1);
    dj_number_quickSort(array,i + 1,right);
}


/*=========================FUNCTION=====================================================
 *
 *      Name: dj_number_bubbleSort
 *      Description: 冒泡排序
 *      Param:
 *          DJ_INT32 array[]: 待排序的数组
 *          DJ_INT32 count: 数组的长度
 *      Return: DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_number_bubbleSort(DJ_INT32 array[],DJ_INT32 count)
{
    DJ_INT32 i,j,tmp;
    
    for (j = 0; j < count; j++)
        for (i = 0; i < count - j;i++ ) {
            if (array[i] > array[i+1]) {
                tmp = array[i];
                array[i] = array[i+1];
                array[i+1] = tmp;
            }
        }
}


/*=========================FUNCTION=====================================================
 *
 *      Name: dj_number_selectSort
 *      Description: 选择排序
 *      Param:
 *          DJ_INT32 array[]: 待排序的数组
 *          DJ_INT32 count: 数组的长度
 *      Return: DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_number_selectSort(DJ_INT32 array[],DJ_INT32 count)
{
    DJ_INT32 i,j,min,tmp;
    
    for (i = 0; i < count; i++) {
        min = i;
        for (j = i; j < count; j++) {
            if (array[min] > array[j]) {
                min = j;
            }
            if (min != 1) {
                tmp = array[i];
                array[i] = array[min];
                array[min] = tmp;
            }
        }
    }
}


/*=========================FUNCTION=====================================================
 *
 *      Name: dj_number_insertSort
 *      Description: 选择排序
 *      Param:
 *          DJ_INT32 array[]: 待排序的数组
 *          DJ_INT32 count: 数组的长度
 *      Return: DJ_VOID
 *
 *======================================================================================
 */
DJ_VOID dj_number_insertSort(DJ_INT32 array[],DJ_INT32 count)
{
    int i,j,tmp;
    for(i = 1;i < count;i++){
        tmp = array[i];
        for(j = i-1;j > -1&& array[j] > tmp;j--){
            array[j+1] = array[j];
        }
        array[j+1] = tmp;
    }
}

/*=========================FUNCTION=====================================================
 *
 *      Name: dj_number_generateRandomNum
 *      Description: 生成1～base之间的随机数 
 *      Param: 
 *          DJ_UINT32 base
 *      Return: 
 *          DJ_UINT32
 *
 *======================================================================================
 */
DJ_UINT32 dj_number_generateRandomNum(DJ_UINT32 base)
{
    DJ_UINT32 number = rand() % base + 1;
	return number;
}
