/****************************************************************************
*Name: 古孟平
*Student ID:M083040024
*Function: Header file on the server side program -- Macro for multithread server
*Date: 2019/12/19
*
****************************************************************************/

#define CLIENT_SOCKET(PORT,STUDENT_ID,STUDENT_NUM) \
	int serverSocket, newSocket, nBytes;\
	int nCount;\
	int conflag=1;\
	char buffer[1024];\
	struct sockaddr_in serverAddr;\
	struct sockaddr_storage serverStorage;\
	socklen_t addr_size;\
	int i;\
	int res;\
	char action[ARRAY_SIZE];\
	char file[ARRAY_SIZE];\
	char mode[ARRAY_SIZE];\
	char* search_ans;\
	char* owner_ans;\
	pthread_t c2,c3,c4,c5,c6;\
	\
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);\
	serverAddr.sin_family = AF_INET;\
	serverAddr.sin_port = htons(PORT);\
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");\
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);\
	bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));\
	if(listen(serverSocket,5)==0)\
		printf("Server" #STUDENT_NUM " ready...\n");\
	else\
		puts("Error");\
	addr_size = sizeof serverStorage;\
	while(1)\
	{\
		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);\
		if(conflag==1){\
			printf("Student" #STUDENT_NUM " CONNECTION\n");\
			conflag=0;\
		}\
		while(1)\
		{\
			nBytes = recv(newSocket,buffer,1024,0);\
			str_token(buffer,action,file,mode);\
			if(strcmp(action,"create")==0)\
			{\
				res=1;\
				puts("---------------------------------------");\
				printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
				FILE *fp = fopen(file, "wb");\
				if(fp == NULL)\
				{\
					puts("Cannot find this file!");\
					exit(0);\
				}\
				close(fp);\
				send(newSocket,"1",2,0);\
				sleep(0.2);\
				fi[STUDENT_ID]=0;\
				if(strlen(mode)<6)\
				{\
					puts("[Server] Wrong format.");\
					send(newSocket,"1",2,0);\
				}\
				else\
				{\
					if(mode[0]=='r' && mode[1]=='w')\
						student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,1,1);\
					else if(mode[0]=='r' && mode[1]=='-')\
						student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,1,0);\
					else if(mode[0]=='-' && mode[1]=='w')\
						student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,0,1);\
					else if(mode[0]=='-' && mode[1]=='-')\
						student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,0,0);\
					else\
					{\
						puts("[Server] Wrong format.");\
						send(newSocket,"1",2,0);\
						continue;\
					}\
					if(same[STUDENT_ID]==0)\
					{\
						for(int i=0;i<6;i++)\
						{\
							if(i==STUDENT_ID)continue;\
							if(strcmp(student[i].group,student[STUDENT_ID].group)==0)\
							{\
								if(mode[2]=='r' && mode[3]=='w')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,1,1);\
								else if(mode[2]=='r' && mode[3]=='-')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,1,0);\
								else if(mode[2]=='-' && mode[3]=='w')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,0,1);\
								else if(mode[2]=='-' && mode[3]=='-')\
									;\
								else\
								{\
									puts("[Server] Wrong format.");\
									send(newSocket,"1",2,0);\
									res=0;\
									break;\
								}\
							}\
							else\
							{\
								if(mode[4]=='r' && mode[5]=='w')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,1,1);\
								else if(mode[4]=='r' && mode[5]=='-')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,1,0);\
								else if(mode[4]=='-' && mode[5]=='w')\
									student[i]=insert(STUDENT_ID,student[i],file,student[STUDENT_ID].owner,0,1);\
								else if(mode[4]=='-' && mode[5]=='-')\
									;\
								else\
								{\
									puts("[Server] Wrong format.");\
									send(newSocket,"1",2,0);\
									res=0;\
									break;\
								}\
							}\
						}\
						if(res==1)\
						{\
							for(int i=0;i<6;i++)\
								print_list(student[i]);\
							printf("INFO: create '%s'\n",file);\
							send(newSocket,"2",2,0);\
\
							strcpy(file_list[file_num].file_name,file);\
							strcpy(file_list[file_num].file_owner,student[STUDENT_ID].owner);\
							file_list[file_num].mutex_read=6;\
							file_list[file_num].mutex_write=1;\
							pthread_mutex_init(&(file_list[file_num].mutex),NULL);\
							file_num++;\
						}\
					}\
					else\
						send(newSocket,"3",2,0);\
				}\
			}\
			else if(strcmp(action,"read")==0)\
			{\
				search_ans=search(student[STUDENT_ID],file);\
				if(search_ans[0]=='2' || search_ans[0]=='0')\
				{\
					send(newSocket,"20",3,0);\
					puts("---------------------------------------");\
					printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
					puts("Permission denied");\
					continue;\
				}\
				else if(search_ans[0]=='1')\
				{\
					send(newSocket,"21",3,0);\
				}\
				sleep(0.2);\
				mutex_read_lock(file);\
				puts("---------------------------------------");\
				printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
				puts("Data Transfering...");\
				send(newSocket,"1",2,0);\
				sleep(10);\
				FILE *fp = fopen(file, "rb");\
				if(fp == NULL)\
				{\
					puts("Cannot find this file!");\
					exit(0);\
				}\
				while( (nCount = fread(buffer, 1, 1024, fp)) > 0 )\
				{\
					send(newSocket, buffer, nCount, 0);\
				}\
				fclose(fp);\
				puts("File transfer success!");\
				mutex_read_unlock(file);\
				close(newSocket);\
				break;\
			}\
			else if(strcmp(action,"write")==0)\
			{\
				search_ans=search(student[STUDENT_ID],file);\
				if(search_ans[0]=='2' || search_ans[1]=='0')\
				{\
					send(newSocket,"30",3,0);\
				}\
				else if(search_ans[1]=='1')\
				{\
					send(newSocket,"31",3,0);\
				}\
				nBytes = recv(newSocket,buffer,1024,0);\
				if(buffer[0]=='0')\
				{\
					puts("---------------------------------------");\
					printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
					puts("You don't have this file...OK~~Got it.");\
				}\
				else if(buffer[0]=='1')\
				{\
					if(search_ans[0]=='2' || search_ans[1]=='0')\
					{\
						puts("---------------------------------------");\
						printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
						puts("Permission denied");\
						continue;\
					}\
					sleep(0.2);\
					mutex_write_lock(file);\
					send(newSocket,"1",3,0);\
					puts("---------------------------------------");\
					printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
					puts("Data receiveing...");\
					if(mode[0]=='a')\
					{\
						FILE *fp = fopen(file, "ab");\
						if(fp == NULL){\
							puts("Cannot find this file!");\
							exit(0);\
						}\
						int nCount;\
						while( (nCount = recv(newSocket,buffer,1024,0)) > 0 ){\
							fwrite(buffer, nCount, 1, fp);\
						}\
						puts("File receive success!");\
						fclose(fp);\
						close(newSocket);\
						mutex_write_unlock(file);\
						break;\
					}\
					else\
					{\
						FILE *fp = fopen(file, "wb");\
						if(fp == NULL){\
							puts("Cannot find this file!");\
							exit(0);\
						}\
						int nCount;\
						while( (nCount = recv(newSocket,buffer,1024,0)) > 0 ){\
							fwrite(buffer, nCount, 1, fp);\
						}\
						puts("File receive success!");\
						fclose(fp);\
						close(newSocket);\
						mutex_write_unlock(file);\
						break;\
					}\
				}\
			}\
			else if(strcmp(action,"changemode")==0)\
			{\
				puts("---------------------------------------");\
				printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
				send(newSocket,"4",2,0);\
				sleep(0.2);\
					owner_ans=search_owner(file);\
					if(strcmp(owner_ans , student[STUDENT_ID].owner)!=0)\
					{\
						puts("Sorry! You are not the owner of the file.");\
						send(newSocket,"2",2,0);\
					}\
					else\
					{\
						if(strlen(mode)<6)\
						{\
							puts("[Server] Right set wrong.");\
							send(newSocket,"3",2,0);\
						}\
						else\
						{\
							search_ans=search(student[0],file);\
							if(search_ans[STUDENT_ID]=='2')\
							{\
								if(mode[0]=='r' && mode[1]=='w')\
									student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,1,1);\
								else if(mode[0]=='r' && mode[1]=='-')\
									student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,1,0);\
								else if(mode[0]=='-' && mode[1]=='w')\
									student[STUDENT_ID]=insert(STUDENT_ID,student[STUDENT_ID],file,student[STUDENT_ID].owner,0,1);\
								else if(mode[0]=='-' && mode[1]=='-')\
								{\
									;\
								}\
								else\
								{\
									puts("[Server] Right set wrong.");\
									send(newSocket,"6",2,0);\
									continue;\
								}\
							}\
							else\
							{\
								if(mode[0]=='r' && mode[1]=='w')\
									student[STUDENT_ID]=change_right(student[STUDENT_ID],file,1,1);\
								else if(mode[0]=='r' && mode[1]=='-')\
									student[STUDENT_ID]=change_right(student[STUDENT_ID],file,1,0);\
								else if(mode[0]=='-' && mode[1]=='w')\
									student[STUDENT_ID]=change_right(student[STUDENT_ID],file,0,1);\
								else if(mode[0]=='-' && mode[1]=='-')\
								{\
									student[STUDENT_ID]=change_right(student[STUDENT_ID],file,0,0);\
								}\
								else\
								{\
									puts("[Server] Right set wrong.");\
									send(newSocket,"6",2,0);\
									continue;\
								}\
							}\
							for(int i=0;i<6;i++)\
							{\
								if(i==STUDENT_ID)continue;\
								else\
								{\
									search_ans=search(student[i],file);\
									if(strcmp(student[i].group,student[STUDENT_ID].group)==0)\
									{\
										if(search_ans[0]=='2')\
										{\
											if(mode[2]=='r' && mode[3]=='w')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,1,1);\
											else if(mode[2]=='r' && mode[3]=='-')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,1,0);\
											else if(mode[2]=='-' && mode[3]=='w')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,0,1);\
											else if(mode[2]=='-' && mode[3]=='-')\
												;\
											else\
											{\
												puts("[Server] Right set wrong.");\
												send(newSocket,"6",2,0);\
												break;\
											}\
										}\
										else\
										{\
											if(mode[2]=='r' && mode[3]=='w')\
												student[i]=change_right(student[i],file,1,1);\
											else if(mode[2]=='r' && mode[3]=='-')\
												student[i]=change_right(student[i],file,1,0);\
											else if(mode[2]=='-' && mode[3]=='w')\
												student[i]=change_right(student[i],file,0,1);\
											else if(mode[2]=='-' && mode[3]=='-')\
												student[i]=change_right(student[i],file,0,0);\
											else\
											{\
												puts("[Server] Right set wrong.");\
												send(newSocket,"6",2,0);\
												break;\
											}\
										}\
									}\
									else\
									{\
										if(search_ans[0]=='2')\
										{\
											if(mode[4]=='r' && mode[5]=='w')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,1,1);\
											else if(mode[4]=='r' && mode[5]=='-')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,1,0);\
											else if(mode[4]=='-' && mode[5]=='w')\
												student[i]=insert(STUDENT_ID,student[i],file,owner_ans,0,1);\
											else if(mode[4]=='-' && mode[5]=='-')\
												;\
											else\
											{\
												puts("[Server] Right set wrong.");\
												send(newSocket,"6",2,0);\
												break;\
											}\
										}\
										else\
										{\
											if(mode[4]=='r' && mode[5]=='w')\
												student[i]=change_right(student[i],file,1,1);\
											else if(mode[4]=='r' && mode[5]=='-')\
												student[i]=change_right(student[i],file,1,0);\
											else if(mode[4]=='-' && mode[5]=='w')\
												student[i]=change_right(student[i],file,0,1);\
											else if(mode[4]=='-' && mode[5]=='-')\
												student[i]=change_right(student[i],file,0,0);\
											else\
											{\
												puts("[Server] Right set wrong.");\
												send(newSocket,"6",2,0);\
												break;\
											}\
										}\
									}\
								}\
							}\
						}\
						for(int i=0;i<6;i++)\
							print_list(student[i]);\
						printf("INFO: changemode '%s'\n",file);\
						send(newSocket,"5",2,0);\
					}\
				\
			}\
			else if(strcmp(action,"bye")==0)\
			{\
				puts("---------------------------------------");\
				printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
				printf("Bye~~\n");\
				close(newSocket);\
				close(serverSocket);\
				exit(0);\
			}\
			else\
			{\
				puts("---------------------------------------");\
				printf("[Student" #STUDENT_NUM "--%s]:\n",action);\
				printf("No instruction\n");\
				send(newSocket,"5",2,0);\
			}\
		}\
		close(newSocket);\
	}
