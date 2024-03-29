/****************************************************************************
*Name: 古孟平
*Student ID:M083040024
*Function: Program for file search and implement file mutex
*Date: 2019/12/19
*
****************************************************************************/

#include "server.h"

/*Take 1 read key if no one is writing the file*/
void mutex_read_lock(char fileName[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , fileName)==0)
		{
			pthread_mutex_lock(&(file_list[i].mutex));
			while(file_list[i].mutex_write != 1);/*If someone is writing the file*/
			file_list[i].mutex_read--;
			pthread_mutex_unlock(&(file_list[i].mutex));
			break;
		}
	}
}
/*Release 1 read key*/
void mutex_read_unlock(char fileName[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , fileName)==0)
		{
			file_list[i].mutex_read++;
			break;
		}
	}
}
/*Take 1 write key & 6 read key if no one is reading and writing the file*/
void mutex_write_lock(char fileName[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , fileName)==0)
		{
			pthread_mutex_lock(&(file_list[i].mutex));
			while(file_list[i].mutex_read != 6);/*If someone is reading the file*/
			file_list[i].mutex_read=0;
			while(file_list[i].mutex_write != 1);/*If someone is writing the file*/
			file_list[i].mutex_write--;
			pthread_mutex_unlock(&(file_list[i].mutex));
			break;
		}
	}
}
/*Release 1 write key & 6 read key*/
void mutex_write_unlock(char fileName[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , fileName)==0)
		{
			file_list[i].mutex_read=6;
			file_list[i].mutex_write++;
			break;
		}
	}
}
/*Check if this file exists in the file list (1: Yes, 0: No)*/
int if_exist(char fileName[])
{
	for(int i=0;i<file_num;i++)
		if(strcmp(file_list[i].file_name , fileName)==0)
			return 1;
	return 0;
}
/*file list -> search node*/
char* search_owner(char file[])
{
	for(int i=0;i<file_num;i++)
	{
		if( strcmp(file_list[i].file_name,file)==0 )
			return file_list[i].file_owner;
	}
	return "NULL";
}
