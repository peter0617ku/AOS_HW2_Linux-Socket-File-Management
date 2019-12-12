#include "server.h"

/*Take 1 read key if no one is writing the file*/
void mutex_read_lock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			while(file_list[i].mutex_write != 1);/*If someone is writing the file*/
			file_list[i].mutex_read--;
			break;
		}
	}
}
/*Release 1 read key*/
void mutex_read_unlock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			file_list[i].mutex_read++;
			break;
		}
	}
}
/*Take 1 write key & 6 read key if no one is reading and writing the file*/
void mutex_write_lock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			while(file_list[i].mutex_read != 6);/*If someone is reading the file*/
			file_list[i].mutex_read=0;
			while(file_list[i].mutex_write != 1);/*If someone is writing the file*/
			file_list[i].mutex_write--;
			break;
		}
	}
}
/*Release 1 write key & 6 read key*/
void mutex_write_unlock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			file_list[i].mutex_read=6;
			file_list[i].mutex_write++;
			break;
		}
	}
}
/*Check if this file exists in the file list (1: Yes, 0: No)*/
int if_exist(char s[])
{
	for(int i=0;i<file_num;i++)
		if(strcmp(file_list[i].file_name , s)==0)
			return 1;
	return 0;
}
