/* *************************************************************************
 *        name:  LinkList.h
 *  Programmer: ZhuJinliang
 * Description:  
 *     Version:  1.0
 *     Created:  2011/11/03
 *    Function: 
 *    Modified by ZhuJinliang 2011-11-8
 * ************************************************************************/
#ifndef  _LINKLIST_H_
#define  _LINKLIST_H_

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define TRUE     1
#define FAIL     0


typedef struct list
{
    void *data;
    struct list *next;
}ListNode, *PListNode;


/******************statement********************************/
PListNode InitList(void);
PListNode CreateNode(void);
int DeleteNode(PListNode pNode, int position);
int DeleteList(PListNode pHead);
int AddTail(PListNode pHeader, void *data);
int InsertAfter(PListNode pHead, int position, void *data);
int UpdateNode(PListNode pHead, int position, void *data);
int GetListCount(PListNode pHead);
PListNode FindNode(PListNode pHead, int position);
int SortList(PListNode pHead, char *compare);


#endif
