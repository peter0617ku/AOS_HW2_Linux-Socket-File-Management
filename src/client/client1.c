#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define ARRAY_SIZE 20

int main(){
	int clientSocket, portNum, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char action[ARRAY_SIZE];
	char file[ARRAY_SIZE];
	char mode[ARRAY_SIZE];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);//插上插座

	serverAddr.sin_family = AF_INET;//TCP本機連線
	serverAddr.sin_port = htons(7891);//設定port
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//設定IP
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);//撥號碼 打給server

	while(1){
		printf("What do you want to do (create/read/write/changemode/bye): ____________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		fgets(buffer,1024,stdin);
		//printf("Instruction: %s",buffer);
		str_token(buffer,action,file,mode);
		nBytes = strlen(buffer) + 1;
		send(clientSocket,buffer,nBytes,0);//對server講話
		
		printf("action:%s\n",action);
		printf("file:%s\n",file);
		printf("mode:%s\n",mode);
		
		nBytes = recv(clientSocket,buffer,1024,0);//接收server傳來的資料
		//printf("receive: %s\n",buffer);
		
		if(buffer[0]=='2')
		{
			FILE *fp = fopen(file, "wb");  //開啟接收的檔案
			if(fp == NULL)
			{
				puts("Cannot find this file!");
				exit(0);
			}
			
			if(buffer[1]=='0')
			{
				puts("You cannot read");
				fclose(fp);
			}
			else if(buffer[1]=='1')	
			{
				puts("File recieving...");
				int nCount;
				while( (nCount = recv(clientSocket, buffer, 1024, 0)) > 0 )//接收資料
				{
					fwrite(buffer, nCount, 1, fp);//將接收到的資料存進file中
				}
				puts("File receive success!");//傳送＆寫檔成功
				fclose(fp);//關閉file descriptor
				close(clientSocket);
				clientSocket = socket(PF_INET, SOCK_STREAM, 0);
				connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
			}
			
		}
		else if(buffer[0]=='3')
		{
			FILE *fp = fopen(file, "rb");  //開啟接收的檔案
			if(fp == NULL)
			{
				/*Client does not have this file*/
				puts("Cannot find this file!");
				send(clientSocket,"0",2,0);//對server講話
				continue;
				//exit(0);
			}

			send(clientSocket,"1",2,0);
			if(buffer[1]=='0')
			{
				puts("[Server] You cannot write");
				fclose(fp);
			}
			else if(buffer[1]=='1')	
			{
				/*Client have this file*/
				puts("File transfering...");
				int nCount;
				while( (nCount = fread(buffer, 1, 1024, fp)) > 0 ){//讀檔
					send(clientSocket, buffer, nCount, 0);//傳送檔案
				}
				sleep(10);
				puts("File transfer success!");//傳送＆寫檔成功
				fclose(fp);//關閉file descriptor
				close(clientSocket);
				clientSocket = socket(PF_INET, SOCK_STREAM, 0);
				connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
			}
		}
		else if(buffer[0]=='5')
		{
			printf("No [%s] instruction.\n",action);
		}
		else if(strcmp(action,"bye")==0)
		{
			printf("BYE~~~~~~~~~\n");
			close(clientSocket);
			exit(0);
		}
		
	}

	return 0;
} 
