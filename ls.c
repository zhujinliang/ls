/* ************************************************************************
 *       Filename:  ls.c
 *     Programmer: ZhuJinliang
 *    Description:  
 *        Version:  1.0
 *        Created:  2011/11/02
 *       Function: 
		  与ls命令类似，命令行参数可以有0到多个 
		  0个参数：列出当前目录下所有文件 
		  参数为目录：列出目录下所有文件 
		  -a：列出目录下所有文件包括以“.”和“..”开头的
		  -d：当遇到目录时列出目录本身而非目录内的文件
		  -l：使用较长格式列出信息
		  -S：根据文件大小排序
		  -t：根据文件修改时间排序
 * ************************************************************************/

/*header file*/
#include "includes.h"


/*global varible*/
int gl_aflag = 0;
int gl_dflag = 0;
int gl_lflag = 0;
int gl_Sflag = 0;  
int gl_tflag = 0; 

/* ****************************************************************************
 *   Function: GetOptions
 * Desciption: get the option from command line 
 *  Arguments: int argc, char *argv[]
  
 * ****************************************************************************/
void GetOptions(int argc, char *argv[])
{
    int ch;
    opterr = 0;   //do not display the err info

    while ((ch = getopt(argc,argv,"adlSt")) != -1)  //use getopt to get the options
    {
        switch (ch)
        {
            case 'a':
                     gl_aflag = 1;
                     break;

            case 'd':
                     gl_dflag = 1;
                     break;

            case 'l':
                     gl_lflag = 1;
                     break;

            case 'S':
                     gl_Sflag = 1;
                     break;


            case 't':
                     gl_tflag = 1;
                     break;

            case '?':
                     printf("Unknown option: %c\n", (char)optopt);
                     break;

            default:
                    printf("Step into the default mode!/n"); 
                    break;
        }    
    }
}

/* ****************************************************************************
 *   Function: List
 * Desciption: list the file
 *  Arguments: char *path
  
 * ****************************************************************************/
int List(char *path)
{
    struct stat st;
    char temp[100];

    if (path == NULL)      //if path is NULL, get current file directory
    {  
        path = temp;  
        getcwd(path, 100);   
    }  
    if (lstat(path, &st))  //open the inode of file
    {
        err_ret("Error: %s not exist.\n", path);
    }
    if (S_ISDIR(st.st_mode)) //check whether it is directory or a regular file
    {  
        if (gl_dflag == 1)   //if the option is -d, print the dir
        {
            printf("%s\n", path);
        }
        else
        {
            ListDir(path); 
        }
         
    }  
    else if (S_ISREG(st.st_mode))    //if is regular file, then print the file name
    {  
        Print(path);  
    }  
    else
    {  
        printf("Not a regular file or directory, can't be listed.\n");  
    }  
    return 0;
}


/* ****************************************************************************
 *   Function: ListDir
 * Desciption: list the directory
 *  Arguments: char *path
  
 * ****************************************************************************/
int ListDir(char *path)
{
    DIR *dp;
    struct dirent *dirp; 
    struct stat st;
    void *buf;
    pFile pInfo = NULL;
    PListNode pHead,pNode,cur;

    if (!(pHead = InitList())) 
    {
        err_sys("init list failed!");
    }
    if ((dp = opendir(path)) == NULL)
    {
        err_sys("can't open %s\n", path);
    }
    chdir(path);                   //cd the path 
   
    while ((dirp = readdir(dp)) != NULL)
    {
        if (((strcmp(dirp->d_name, ".")) == 0)
         || ((strcmp(dirp->d_name, "..")) == 0)
         || (dirp->d_name[0] == '.'))
        {
            if (gl_aflag == 0)  //if the option is not -a, don't print the file whose name is start with "." ".."
            {
                continue;
            }   
        }
        if ((gl_Sflag == 1) || (gl_tflag == 1))
        {
            if (lstat(dirp->d_name, &st))   //open the inode of file
            {
                err_ret("Error: %s is not exist.\n", dirp->d_name);
            }
            pInfo = (pFile)malloc(sizeof(fileInfo));
            pInfo->name = dirp->d_name;
            pInfo->size = st.st_size;
            pInfo->time  = st.st_mtime;
            if (!AddTail(pHead, pInfo))
            {
                printf("add node failed!");
            }
        }
        else
        {
            Print(dirp->d_name); 
        }
        
    }
    TraverseList(pHead, PrintNode); 
    if (gl_Sflag == 1)         //the option is -S, then sort according by size from max to min
    {
        SortList(pHead, CompareSize);

        pNode = pHead->next;
        while (pNode != NULL)
        { 
            Print(((pFile)(pNode->data))->name);
            pNode = pNode->next;
        } 
    }
    else if (gl_tflag == 1)    //the option is -t, then sort according by time from lately to previous
    {
        SortList(pHead, CompareTime);
        
        pNode = pHead->next;
        while (pNode != NULL)
        { 
            Print(((pFile)(pNode->data))->name);
            pNode = pNode->next;
        } 
    }
    printf("\n");

    if (closedir(dp))
    {
       err_sys("can't close the directory %s\n", path);
    }
    DeleteList(pHead);
    return 0; 
}

/* ****************************************************************************
 *   Function: Print
 * Desciption: print file
 *  Arguments: char *path
  
 * ****************************************************************************/
int Print(char *path)
{
    struct stat st;
    char *fileName = NULL;
    char time[64];

    if (lstat(path, &st))  //open the inode of file
    {
        err_ret("Error: %s is not exist.\n", path);
    }
    if ((fileName = strrchr(path, '/')) != NULL)  //delete the directory, then get the file name 
    {  
        fileName++;  
    } 
    else
    {
        fileName = path;
    }
    if (gl_lflag == 1)        //print with long former              
    {
        (S_ISDIR(st.st_mode)) ? printf("d"): printf("-");
        //print permissions
        (st.st_mode & S_IRUSR) ? printf("r"): printf("-");
        (st.st_mode & S_IWUSR) ? printf("w"): printf("-");
        (st.st_mode & S_IXUSR) ? printf("x"): printf("-");
        (st.st_mode & S_IRGRP) ? printf("r"): printf("-");
        (st.st_mode & S_IWGRP) ? printf("w"): printf("-");
        (st.st_mode & S_IXGRP) ? printf("x"): printf("-");
        (st.st_mode & S_IROTH) ? printf("r"): printf("-");
        (st.st_mode & S_IWOTH) ? printf("w"): printf("-");
        (st.st_mode & S_IXOTH) ? printf("x"): printf("-");
        
        printf("%3d", st.st_nlink);        
        if (st.st_uid == 1000)
        {
            printf("\tguest  ");
        }
        else if (st.st_uid == 0)
        {
            printf("\troot   ");
        }
        if (st.st_gid == 1000)
        {
            printf("guest  ");
        }
        else if (st.st_gid == 0)
        {
            printf("root   ");
        }
        strftime(time, sizeof(time), "%Y-%m-%d %H:%M", localtime(&st.st_mtime));
        printf("%10ld   %s  %s\n", st.st_size, time, fileName);
    }
    else
    {
        printf("%s\t", fileName);
    }

    return 0;
}

/* ****************************************************************************
 *   Function: PrintNode
 * Desciption: print list node
 *  Arguments: PListNode pHead
  
 * ****************************************************************************/
void PrintNode(void *file)
{
    printf("%s\t", ((pFile)file)->name);
    printf("%10ld\t%10ld\n", ((pFile)file)->size, ((pFile)file)->time);
}

/* ****************************************************************************
 *   Function: CompareString
 * Desciption: 
 *  Arguments: PListNode pHead
 *
 *  **************************************************************************/
int CompareString(void *file1, void *file2)
{
    return (strcmp(((pFile)file1)->name, ((pFile)file2)->name));

}


/* ****************************************************************************
 *   Function: CompareSize
 * Desciption: 
 *  Arguments: void *file1, void *file2
 *
 *  **************************************************************************/
int CompareSize(void *file1, void *file2)
{
    if ((((pFile)file1)->size) < (((pFile)file2)->size))
    {
        return -1;
    }
    else  if ((((pFile)file1)->size) > (((pFile)file2)->size))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* ****************************************************************************
 *   Function: CompareTime
 * Desciption: 
 *  Arguments: void *file1, void *file2
 *
 *  **************************************************************************/
int CompareTime(void *file1, void *file2)
{
    if ((((pFile)file1)->time) < (((pFile)file2)->time))
    {
        return -1;
    }
    else if ((((pFile)file1)->time) > (((pFile)file2)->time))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/* ****************************************************************************
 *   Function: main
 * Desciption: 
 *  Arguments: int argc, char *argv[]
  
 * ****************************************************************************/

int main (int argc, char *argv[])
{
    GetOptions(argc, argv);
    List(argv[optind]);
    exit(0);
}
