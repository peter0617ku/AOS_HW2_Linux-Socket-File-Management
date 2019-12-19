/****************************************************************************
*Name: 古孟平
*Student ID:M083040024
*Function: Program for processing capability list
*Date: 2019/12/19
*
****************************************************************************/

#include "server.h"

/*capability list -> Initialization*/
TOP initialize(TOP a,char name[], char group[])
{
	strcpy(a.owner,name);
	strcpy(a.group,group);
	a.top=NULL;
	return a;
}
/*capability list -> insertion*/
TOP insert(int x,TOP a,char file[],const char file_owner[],int read ,int write)
{
	same[x]=0;
	char* ans;
	/*Check whether the list have this file*/
	if(fi[x]==0)
	{
		if(file_num!=0)
		{
			ans = search_owner(file);
			if(ans[0]!='N') same[x]=1;
			else same[x]=0;
		}
		else same[x]=0;
	}
	fi[x]=1;
	if(same[x]==1)
	{
		puts("INFO: [Already have this file.]");
		return a;
	}
	else if(!(read == 0 && write == 0))
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
		
		return a;
	}
	return a;
}
/*capability list -> change right set or delete node*/
TOP change_right(TOP a,char file[],int read,int write)
{
	int find=0;
	NODE* search = a.top;
	NODE* prenode = a.top;
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
					if(prenode->next == NULL) a.top=NULL;
					else 
					{
						if(search==a.top)a.top=search->next;
						else prenode->next=search->next;
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
/*capability list -> print each node*/
void print_list(const TOP a)
{
	NODE* ptr=a.top;
	printf("name: %s ",a.owner);
	
	if(ptr==NULL) puts("[Empty list.]");
	else
	{
		while(ptr!=NULL)
		{
			printf(" => <%s(%c),{%d,%d}>",ptr->file_name,ptr->file_owner[7],ptr->read,ptr->write);
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
