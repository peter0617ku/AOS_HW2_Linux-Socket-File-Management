#include "server.h"

/*capability list -> Initialization*/
TOP ini(TOP a,char name[], char group[])
{
	strcpy(a.owner,name);
	strcpy(a.group,group);
	a.top=NULL;
	return a;
}
/*capability list -> insertion*/
TOP insert(int x,TOP a,char file[],char file_owner[],int read ,int write)
{
	same[x]=0;
	/*Check whether the list have this file*/
	if(fi[x]==0)
		same[x]=if_exist(file);
	fi[x]=1;
	if(same[x]==1)
		puts("INFO: [Already have this file.]");
	else
	{
		/*Create a new node*/
		NODE* new=(NODE*)malloc(sizeof(NODE));
		strcpy(new->file_name,file);
		strcpy(new->file_owner,file_owner);
		new->read=read;
		new->write=write;
		
		/*Insert the node*/
		new->next=a.top;
		a.top=new;

		/*Add file to the file list and initailization*/
		strcpy(file_list[file_num].file_name,file);
		file_list[file_num].mutex_read=6;
		file_list[file_num].mutex_write=1;
		file_num++;
		return a;
	}
}
/*capability list -> change right set or delete node*/
TOP change_right(TOP a,char file[],int read,int write)
{
	int find=0;
	NODE* search = a.top;
	NODE* prenode=a.top;
	if(a.top==NULL) puts("Capability list is empty. Please check!!");
	else
	{
		while(search != NULL)
		{
			if(strcmp(search->file_name,file)==0)
			{
				search->read = read;
				search->write = write;
				/*Delete node if read and write are false*/
				if(search->read == 0 && search->write == 0)
				{
					if(search->next!=NULL) prenode->next=search->next;
					else
					{		
						if(search==a.top) a.top=NULL;
						else prenode->next=NULL;
					}
					free(search);
				}
				find=1;
				break;
			}
			else
			{
				prenode = search;
				search = search->next;
			}
		}
		if(find==0) puts("Error! The file is not in Capability list.");
	}
	//delete the last node
	return a;
}
/*capability list -> search node*/
char* search(TOP a,char file[])
{
	NODE* search = a.top;
	while(search != NULL)
	{
		if(strcmp(search->file_name,file)==0)
		{
			if(search->read==1 &&search->write==1 ) return "11";
			else if(search->read==1) return "10";
			else return "01";
		}
		else search = search->next;
	}
	return "22";
}
/*capability list -> search node*/
char* search_owner(TOP a,char file[])
{
	NODE* search = a.top;
	while(search != NULL)
	{
		if(strcmp(search->file_name,file)==0)
		{
			return search->file_owner;
		}
		else search = search->next;
	}
	return "";
}
/*capability list -> print each node*/
void print_list(TOP a)
{
	NODE* ptr=a.top;
	printf("name: %s ",a.owner);
	
	if(ptr==NULL) puts("[Empty list.]");
	else
	{
		while(ptr!=NULL)
		{
			printf(" => <%s,{%d,%d}>",ptr->file_name,ptr->read,ptr->write);
			ptr=ptr->next;
		}
		puts("");
	}
}
/*test action
int main()
{
	TOP a;
	char* str;
	char aa[20],b[20],c[20];
	strcpy(aa,"a.c");
	strcpy(b,"bb.c");
	strcpy(c,"ccc.c");
	a=ini(a,"sss","CSE");
	
	a=insert(a,aa,1,1);
	print_list(a);
	
	a=insert(a,b,1,0);
	print_list(a);
	
	a=insert(a,c,1,0);
	print_list(a);
	
	str=search(a,aa);
	printf("ans: %s\n",str);
	
	a=change_right(a,b,0,0);
	print_list(a);
	
	a=change_right(a,aa,0,0);
	print_list(a);
	a=change_right(a,c,1,1);
	print_list(a);
	
	return 0;
}*/
