/*
#**********************************
#    File Name: client.c
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-07-31 11:53:12
#Last modified: 2017-08-10 12:59:47
#**********************************
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include"chat.h"

#define MAX_TEXT 512
#define TRUE 1
#define FALSE 0

int main(){
	int ret = -1;
	int msg_type = 10, msg_rtype = 11, msg_stype = -1, msg_sndtype = 12;
	int msg_id = -1;
	MESSAGE *message;
	int add_flag = 1, exit_flag = 0;
	key_t key;
	int run_flag = TRUE;
	char text[300];
	char *msgpath = "~/Desktop", username[15];
	key = ftok(msgpath, 1);
	pid_t pid;
	pid = getpid();
	
	//创建管道的相关参数,管道读写两端必须是同步的,不能解决此问题,尝试下共享内存
	//int result = -1;
	//int fd[2];
	//int *write_fd = &fd[1];
	//int *read_fd = &fd[0];
	
	//printf("进程id:%d\n",getpid());
	//共享内存,共享内存也不行,这个要求 必须
	//int *shms, *shmc, shmid;
	//shmid = shamget(key, 4, IPC_CREAT | 0604);

	
	msg_id = msgget(key, 0666 | IPC_CREAT);
	if(msg_id == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		return 0;
	}
	printf("Enter your username:");	//输入名字
	scanf("%s", username);
	getchar();	//过滤回车
	//printf("msg_id:%d\n", msg_id);
	ret = SendRequest(msg_id, msg_type, FALSE, username, add_flag);//发送请求
	if(-1 == ret){
		printf("请求失败\n");
		return 0;
	}

	msg_stype = ReceiveMtype(msg_id, msg_rtype);
	if (msg_stype == -1){
		printf("加入聊天失败\n");
		return 0;
	}
	printf("------成功加入聊天室------\n");
	
	//创建管道
	//result = pipe(fd);
	//if(-1 == result){
	//	printf("创建管道失败\n");
	//	return -1;
	//}

	pid_t jpid;
	jpid = fork();
	if(jpid < 0){
		printf("fork()进程失败\n");
		return - 1;
	}else if(jpid == 0){
		while(run_flag){
			//close(*read_fd); //关闭读端
			fgets(text, 300, stdin);
			if(text[strlen(text) -1] == '\n'){	//过滤掉结尾的回车,机智的方法
				text[strlen(text) -1] = '\0';
			}
			//退出命令cmd exit
			if(0 == (strcmp(text, "cmd exit"))){
				SendRequest(msg_id, msg_type, msg_stype, username, exit_flag);
				run_flag = FALSE;
				//write(*write_fd, &run_flag, sizeof(int));
				//printf("输入框run_flag:%d\n", run_flag);
				//return 0;
			}else{
			//发送消息模块
			//printf("%s的发送pid:%d\n", username, pid);
			SendMsg(msg_id, msg_sndtype, msg_stype, username, text);
			}
		}
	}else{
		while(run_flag){
			//close(*write_fd); //关闭写端
			message = NULL;
			message = ReceiveMsg(msg_id, msg_stype);
			if(message == NULL){
				printf("消息接收失败\n");
			}
			//printf("发消息的类型%d:当前消息类型%d\n", message->m_type, msg_stype);	//信息类型:本类型	
			if(message->m_type != msg_stype){
				if(message->m_type > 10){
					printf("\n%s(%d):\n\t%s\n\n", message->username, message->m_type, message->text);
				}
				if(message->m_type == 0){
					printf("\n%s\n", message->text);
				}
			}
			if(message->m_type == 1){
				run_flag = FALSE;
			}
			//read(*read_fd, &run_flag, sizeof(int));
			//printf("run_flag:%d\n", run_flag);
		}	
		//return 0;
	}

	return 0;
}













