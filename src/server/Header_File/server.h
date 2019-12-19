/****************************************************************************
*Name: 古孟平
*Student ID:M083040024
*Function: Header file on the server side program
*Date: 2019/12/19
*
****************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 20
/*Initialize mutex*/
#define MUTEX_INITIALIZE(MUTEX) \
if (pthread_mutex_init(&MUTEX, NULL) != 0){\
	exit(1);\
}

/*node of capability list*/
typedef struct node
{
	/*Domain*/
	char file_name[ARRAY_SIZE];
	char file_owner[ARRAY_SIZE];
	/*Right_set*/
	int read;
	int write;
	/*Pointer to next node*/
	struct node* next;
}NODE;
/*Top node of capability list*/
typedef struct top_node
{
	char owner[ARRAY_SIZE];/*list domain name*/
	char group[ARRAY_SIZE];/*owner group*/
	NODE* top;/*pointer: top*/
}TOP;
/*Node of file list*/
typedef struct file_node
{
	char file_name[ARRAY_SIZE];
	char file_owner[ARRAY_SIZE] ;
	int mutex_read;
	int mutex_write;
	pthread_mutex_t mutex;
}FNODE;

/*Global variable*/
FNODE file_list[100];
int file_num;
int same[6];
int fi[6];
/*6 capability list*/
TOP student[6];

/*Function prototype -- capability_list.c*/
TOP initialize(TOP a,char name[], char group[]);
TOP insert(int x,TOP a,char file[],const char file_owner[],int read ,int write);
TOP change_right(TOP a,char file[],int read,int write);
char* search(TOP a,char file[]);
void print_list(TOP a);
/*Function prototype -- file_mutex.c*/
void mutex_read_lock(char fileName[]);
void mutex_read_unlock(char fileName[]);
void mutex_write_lock(char fileName[]);
void mutex_write_unlock(char fileName[]);
int if_exist(char fileName[]);
char* search_owner(char file[]);

