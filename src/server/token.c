#include <stdio.h>
#include <string.h>

#define str_copy(x) \
	strncpy(x,string+z-i,i2);\
	s++;

/*Function that cuts instruction into tokens*/
void str_token(const char* string, char a1[],char a2[],char a3[])
{
	int i=0;
	int i2=0;
	int y;
	int start=0;
	int index=0;
	int s=0;
	
	//printf("total test=%s\n",string);
	for(int z=0;z<strlen(string);z++)
	{
		for(i=0;z<strlen(string);z++,i++)
		{
			if(i==0 && string[z]==' ')
				i--;
			else if(i && string[z]==' ')
				break;
		}
		i2=i;//表示欲複製字串長度
		if(z==strlen(string))//去掉最後一個換行字元
			i2=i-1;
		if(s==0)
		{	
			str_copy(a1);
			a1[i2]='\0';
			//printf("test=%d\n",i2);
		}
		else if(s==1)
		{
			str_copy(a2);
			a2[i2]='\0';
			//printf("test=%d,%s\n",i2,a2);
		}
		else if(s==2)
		{
			str_copy(a3);
			a3[i2]='\0';
			//printf("test=%d\n",i2);
		}
	}
	switch(s)
	{
		case 0:
			strcpy(a1,"null");
			strcpy(a2,"null");
			strcpy(a3,"null");
			break;
		case 1:
			strcpy(a2,"null");
			strcpy(a3,"null");
			break;
		case 2:
			strcpy(a3,"null");
			break;
	}
}
