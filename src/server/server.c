#include "server.h"
#include "socket.h"

void* client2()
{
	CLIENT_SOCKET(7892,1,2);
	pthread_exit(NULL);
}
void* client3()
{
	CLIENT_SOCKET(7893,2,3);
	pthread_exit(NULL);
}
void* client4()
{
	CLIENT_SOCKET(7894,3,4);
	pthread_exit(NULL);
}
void* client5()
{
	CLIENT_SOCKET(7895,4,5);
	pthread_exit(NULL);
}
void* client6()
{
	CLIENT_SOCKET(7896,5,6);
	pthread_exit(NULL);
}

int main()
{
	int serverSocket, newSocket, nBytes;
	int nCount;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int i;
	char action[ARRAY_SIZE];
	char file[ARRAY_SIZE];
	char mode[ARRAY_SIZE];
	char* search_ans;
	char* owner_ans;
	pthread_t c2,c3,c4,c5,c6;
	
	file_num=0;
	/*Initailize Capability list*/
	puts("Initailizing Capability list...");
	student[0] = ini(student[0],"student1","groupA");
	student[1] = ini(student[1],"student2","groupA");
	student[2] = ini(student[2],"student3","groupA");
	student[3] = ini(student[3],"student4","groupB");
	student[4] = ini(student[4],"student5","groupB");
	student[5] = ini(student[5],"student6","groupB");
	puts("Initailize Capability list: [Done]");
	
	/*puts("Initailizing mutexs...");
	MUTEX_INITIALIZE(mutex_read1);
	MUTEX_INITIALIZE(mutex_read2);
	MUTEX_INITIALIZE(mutex_read3);
	MUTEX_INITIALIZE(mutex_read4);
	MUTEX_INITIALIZE(mutex_read5);
	MUTEX_INITIALIZE(mutex_read6);
	MUTEX_INITIALIZE(mutex_write);
	puts("Initailize mutexs: [Done]");*/
	
	pthread_create(&c2,NULL,client2,NULL);
	pthread_create(&c3,NULL,client3,NULL);
	pthread_create(&c4,NULL,client4,NULL);
	pthread_create(&c5,NULL,client5,NULL);
	pthread_create(&c6,NULL,client6,NULL);
	
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);//插上插座
	
	serverAddr.sin_family = AF_INET;//TCP本機傳輸
	serverAddr.sin_port = htons(7891);//設定port
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//設定IP
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//接上電話線

	if(listen(serverSocket,5)==0)//聽頻道上有無client要連線->電話響了 拿起聽筒
		puts("Listening...");
	else
		puts("Error");

	addr_size = sizeof serverStorage;
	
	while(1)
	{
		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);//接受連線->聽client說話
		/*Client已連線，執行以下的程式碼*/
		puts("Student1 CONNECTION");
		while(1)
		{//連線中利用while loop連續處理字串接收
			puts("---------------------------------------");
			nBytes = recv(newSocket,buffer,1024,0);//接收client傳來的資料
			str_token(buffer,action,file,mode);//切token
			if(strcmp(action,"create")==0)
			{
				printf("[Student1--%s]:\n",action);
				FILE *fp = fopen(file, "wb");//create a new file
				if(fp == NULL)
				{
					puts("Cannot find this file!");
					exit(0);
				}
				close(fp);
				
				send(newSocket,"1",2,0);/*Tell client what kind of action(1:create, 2:read, 3:write, 4: changemode)*/
				
				/*Update Capability list and print it*/
				fi[0]=0;
				if(strlen(mode)<6)
					puts("[Server] Right set wrong.");
				else
				{
					/*set owner*/
					if(mode[0]=='r' && mode[1]=='w')
						student[0]=insert(0,student[0],file,student[0].owner,1,1);
					else if(mode[0]=='r')
						student[0]=insert(0,student[0],file,student[0].owner,1,0);
					else if(mode[1]=='w')
						student[0]=insert(0,student[0],file,student[0].owner,0,1);
					else if(mode[0]=='-' && mode[1]=='-')
						puts("No file!");
					else
						puts("rw?????");
					/*If does not have this file before (The file is not in capability list)*/
					if(same[0]==0)
					{
						for(int i=1;i<6;i++)
						{
							/*set group*/
							if(strcmp(student[i].group,student[0].group)==0)
							{
								if(mode[2]=='r' && mode[3]=='w')
									student[i]=insert(0,student[i],file,student[0].owner,1,1);
								else if(mode[2]=='r')
									student[i]=insert(0,student[i],file,student[0].owner,1,0);
								else if(mode[3]=='w')
									student[i]=insert(0,student[i],file,student[0].owner,0,1);
								else if(mode[2]=='-' && mode[3]=='-')
									;
								else
									puts("rw?????");
							}
							/*set other*/
							else
							{
								if(mode[4]=='r' && mode[5]=='w')
									student[i]=insert(0,student[i],file,student[0].owner,1,1);
								else if(mode[4]=='r')
									student[i]=insert(0,student[i],file,student[0].owner,1,0);
								else if(mode[5]=='w')
									student[i]=insert(0,student[i],file,student[0].owner,0,1);
								else if(mode[4]=='-' && mode[5]=='-')
									;
								else
									puts("rw?????");
							}
						}
						for(int i=0;i<6;i++)
							print_list(student[i]);
						printf("INFO: create '%s'\n",file);
					}	
				}
				
			}/*end of if*/
			else if(strcmp(action,"read")==0)
			{
				/*Check right or whether the file exists*/
				search_ans=search(student[0],file);
				if(search_ans[0]=='2' || search_ans[0]=='0')
				{
					send(newSocket,"20",3,0);/*Client cannot write the file (The file does not exist or permisssion denied)*/
					puts("Permission denied");
					continue;
				}
				else if(search_ans[0]=='1')
				{
					send(newSocket,"21",3,0);/*Client can write the file (And capability list have this right)*/
				}
				
				mutex_read_lock(file);/*lock*/
				printf("[Student1--%s]:\n",action);
				puts("Data Transfering...");
				FILE *fp = fopen(file, "rb");  //create a new file
				if(fp == NULL)
				{
					puts("Cannot find this file!");
					exit(0);
				}
				while( (nCount = fread(buffer, 1, 1024, fp)) > 0 )//讀檔
				{
					send(newSocket, buffer, nCount, 0);//傳送檔案
				}
				sleep(10);
				fclose(fp);
				puts("File transfer success!");
				
				mutex_read_unlock(file);/*unlock*/
				close(newSocket);
				break;
			}/*end of else if*/
			else if(strcmp(action,"write")==0)
			{
				/*Check right or whether the file exists*/
				search_ans=search(student[0],file);
				if(search_ans[0]=='2' || search_ans[1]=='0')
				{
					send(newSocket,"30",3,0);/*Client cannot write the file (The file does not exist or permisssion denied)*/
				}
				else if(search_ans[1]=='1')
				{
					send(newSocket,"31",3,0);/*Client can write the file (And capability list have this right)*/
				}
				nBytes = recv(newSocket,buffer,1024,0);
				if(buffer[0]=='0')
					puts("You don't have this file...OK~~Got it.");
				else if(buffer[0]=='1')
				{
					if(search_ans[0]=='2' || search_ans[1]=='0')
					{
						puts("Permission denied");
						continue;
					}
					
					mutex_write_lock(file);/*lock*/
					printf("[Student1--%s]:\n",action);
					puts("Data receiveing...");
					/*Append mode*/
					if(mode[0]=='a')
					{
						FILE *fp = fopen(file, "ab");  //create a new file
						
						if(fp == NULL){
							puts("Cannot find this file!");
							exit(0);
						}
						int nCount;
						while( (nCount = recv(newSocket,buffer,1024,0)) > 0 ){//接收資料
							fwrite(buffer, nCount, 1, fp);//將接收到的資料存進file中
						}
						puts("File receive success!");//傳送＆寫檔成功
						fclose(fp);
						
						mutex_write_unlock(file);/*unlock*/
						close(newSocket);
						break;
					}
					/*Overwrite mode*/
					else
					{
						FILE *fp = fopen(file, "wb"); 
						if(fp == NULL){
							puts("Cannot find this file!");
							exit(0);
						}
						int nCount;
						while( (nCount = recv(newSocket,buffer,1024,0)) > 0 ){//接收資料
							fwrite(buffer, nCount, 1, fp);//將接收到的資料存進file中
						}
						puts("File receive success!");//傳送＆寫檔成功
						fclose(fp);
						
						mutex_write_unlock(file);/*unlock*/
						close(newSocket);
						break;
					}
				}/*end of else if*/
			}/*end of if*/
			else if(strcmp(action,"changemode")==0)
			{
				printf("[Student1--%s]:\n",action);
				send(newSocket,"4",2,0);/*tell client: the action is changemode(4)*/
				
				search_ans=search(student[0],file);
				/*file is not in the list*/
				if(search_ans[0]=='2')
				{
					puts("Sorry! You don't have this file...");
					/*if(mode[0]=='r' && mode[1]=='w')
						student[0]=insert(student[0],file,1,1);
					else if(mode[0]=='r')
						student[0]=insert(student[0],file,1,0);
					else if(mode[1]=='w')
						student[0]=insert(student[0],file,0,1);
					else if(mode[0]=='-' && mode[1]=='-')
						;
					else
						puts("Right???");*/
				}
				/*file in the list*/
				else
				{
					owner_ans=search_owner(student[0],file);
					/*If the file is not yours, you cannot change the mode of this file*/
					if(strcmp(owner_ans , student[0].owner)!=0)
					{
						puts("It is not your file!!!!");
					}
					/*If file owner is you, you can change the mode.*/
					else
					{
						/*check mode type*/
						if(strlen(mode)<6)
							puts("[Server] Right set wrong.");
						else
						{
							if(mode[0]=='r' && mode[1]=='w')
								student[0]=change_right(student[0],file,1,1);
							else if(mode[0]=='r')
								student[0]=change_right(student[0],file,1,0);
							else if(mode[1]=='w')
								student[0]=change_right(student[0],file,0,1);
							else if(mode[0]=='-' && mode[1]=='-')
							{
								//student[0]=change_right(student[0],file,0,0);
								puts("[Error] If you do this, you will no longer have permission to this file.");
								continue;
							}
							else
								puts("Right???");
							/*如果自己的capability list有，才可以改別人的list*/
							for(int i=0;i<6;i++)
							{
								if(i==0)continue;
								else
								{
									search_ans=search(student[i],file);
									/*Same group*/
									if(strcmp(student[i].group,student[0].group)==0)
									{
										/*file is not in the list*/
										if(search_ans[0]=='2')
										{
											if(mode[2]=='r' && mode[3]=='w')
												student[i]=insert(0,student[i],file,owner_ans,1,1);
											else if(mode[2]=='r')
												student[i]=insert(0,student[i],file,owner_ans,1,0);
											else if(mode[3]=='w')
												student[i]=insert(0,student[i],file,owner_ans,0,1);
											else if(mode[2]=='-' && mode[3]=='-')
												;
											else
											{
												puts("Right???");
												break;
											}
											
										}
										/*file in the list*/
										else
										{
											if(mode[2]=='r' && mode[3]=='w')
												student[i]=change_right(student[i],file,1,1);
											else if(mode[2]=='r')
												student[i]=change_right(student[i],file,1,0);
											else if(mode[3]=='w')
												student[i]=change_right(student[i],file,0,1);
											else if(mode[2]=='-' && mode[3]=='-')
												student[i]=change_right(student[i],file,0,0);
											else
											{
												puts("Right???");
												break;
											}
										}	
									}
									/*Other*/
									else
									{
										/*file is not in the list*/
										if(search_ans[0]=='2')
										{
											if(mode[4]=='r' && mode[5]=='w')
												student[i]=insert(0,student[i],file,owner_ans,1,1);
											else if(mode[4]=='r')
												student[i]=insert(0,student[i],file,owner_ans,1,0);
											else if(mode[5]=='w')
												student[i]=insert(0,student[i],file,owner_ans,0,1);
											else if(mode[4]=='-' && mode[5]=='-')
												;
											else
											{
												puts("Right???");
												break;
											}
										}
										/*file in the list*/
										else
										{
											if(mode[4]=='r' && mode[5]=='w')
												student[i]=change_right(student[i],file,1,1);
											else if(mode[4]=='r')
												student[i]=change_right(student[i],file,1,0);
											else if(mode[5]=='w')
												student[i]=change_right(student[i],file,0,1);
											else if(mode[4]=='-' && mode[5]=='-')
												student[i]=change_right(student[i],file,0,0);
											else
											{
												puts("Right???");
												break;
											}
										}/*end of else*/	
									}/*end of else*/	
								}/*end of else*/
							}/*end of for*/
						}
						
						/*Output the result of change mode action*/
						for(int i=0;i<6;i++)
							print_list(student[i]);
						printf("INFO: changemode '%s'\n",file);
					}/*end of else*/
				}/*end of else*/
			}/*end of if*/
			else if(strcmp(action,"bye")==0)
			{
				printf("[Student1--%s]:\n",action);
				printf("Bye~~\n");
				close(newSocket);
				close(serverSocket);
				exit(0);
			}/*end of else*/
			else
			{
				printf("[Student1--%s]:\n",action);
				printf("No instruction\n");
				send(newSocket,"5",2,0);/*tell client: the action is NO instruction(5)*/
			}/*end of else*/
		}/*End of while*/
		close(newSocket);//關閉socket連線並等待下一個連線->掛電話
	}
	return 0;
}