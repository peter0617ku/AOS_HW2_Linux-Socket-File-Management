#include "server.h"

void mutex_read_lock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			while(file_list[i].mutex_write != 1);
			file_list[i].mutex_read--;
			break;
		}
	}
}
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
void mutex_write_lock(char s[])
{
	for(int i=0;i<file_num;i++)
	{
		if(strcmp(file_list[i].file_name , s)==0)
		{
			while(file_list[i].mutex_read != 6);
			file_list[i].mutex_read=0;
			while(file_list[i].mutex_write != 1);
			file_list[i].mutex_write--;
			break;
		}
	}
}
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
int if_exist(char s[])
{
	for(int i=0;i<file_num;i++)
		if(strcmp(file_list[i].file_name , s)==0)
			return 1;
	return 0;
}
