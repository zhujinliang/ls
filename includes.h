#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
/* header file in ourhdr.h*/
#include <sys/types.h>	        /* required for some of our prototypes */
#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
/* header file in myErr.h*/
#include <errno.h> /* for definition of errno */
#include <stdarg.h> /* ISO C variable aruments */

#include "ourhdr.h"
#include "myErr.h"

#include "LinkList.h"

typedef struct file 
{
    char *name;
    off_t size;
    time_t time;
}fileInfo, *pFile;


void GetOptions(int argc, char *argv[]);
int List(char *path);
int ListDir(char *path);
int Print(char *path);
void PrintNode(void *file);
int CompareString(void *file1, void *file2);
int CompareSize(void *file1, void *file2);
int CompareTime(void *file1, void *file2);


#endif 
