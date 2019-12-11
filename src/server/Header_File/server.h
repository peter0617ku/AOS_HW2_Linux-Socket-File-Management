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
}\

typedef struct file_node
{
	char file_name[ARRAY_SIZE];
	int mutex_read;
	int mutex_write;
}FNODE;

/*Global variable*/
FNODE file_list[100];
int file_num;
int same[6];
int fi[6];

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

/*6 capability list*/
TOP student[6];

void print_list(TOP a);
char* search(TOP a,char file[]);
TOP change_right(TOP a,char file[],int read,int write);
TOP insert(int x,TOP a,char file[],char file_owner[],int read ,int write);
TOP ini(TOP a,char name[], char group[]);
char* search_owner(TOP a,char file[]);
