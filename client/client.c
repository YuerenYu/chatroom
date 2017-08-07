/*
#**********************************
#    File Name: msgsend-03.c
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-07-31 11:53:12
#Last modified: 2017-08-07 13:38:04
#**********************************
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>
#include"chat.h"

#define MAX_TEXT 512
#define TRUE 1
#define FALSE 0


int main(){
	int ret = -1;
	int msg_type = 10, msg_rtype = 11, msg_stype, msg_sndtype = 12;
	int msg_id = -1;
	MESSAGE *message;
	key_t key;
	char text[300];
	char *msgpath = "~/Desktop", username[15];
	key = ftok(msgpath, 1);
	pid_t pid;
	pid = getpid();
	//printf("进程id:%d\n",getpid());
	msg_id = msgget(key, 0666 | IPC_CREAT);
	if(msg_id == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		return 0;
	}
	printf("Enter your username:");	//输入名字
	scanf("%s", username);
	getchar();	//过滤回车
	//printf("msg_id:%d\n", msg_id);
	ret = SendRequest(msg_id, msg_type, pid, username);//发送请求
	if(-1 == ret){
		printf("请求失败\n");
		return 0;
	}

	msg_stype = ReceiveMtype(msg_id, msg_rtype);
	//struct S_MTYPE *r_mtype;
	//r_mtype = (struct S_MTYPE *)malloc(sizeof(struct S_MTYPE));
	//if(msgrcv(msg_id, r_mtype, sizeof(struct S_MTYPE), msg_rtype, 0) == -1){
		//printf("555555555\n");
		//return -1;
	//}
	//msg_stype = r_mtype->r_mtype;
	//printf("发送message消息类型为:%d\n", msg_stype);
	printf("------成功加入聊天室------\n");
	
	pid_t jpid;
	jpid = fork();
	if(jpid < 0){
		printf("fork()进程失败\n");
		return - 1;
	}else if(jpid > 0){
		while(TRUE){
			fgets(text, 300, stdin);
			if(text[strlen(text) -1] == '\n'){	//过滤掉结尾的回车,机智的方法
				text[strlen(text) -1] = '\0';
			}
			//getchar();
			
			//发送消息模块
			SendMsg(msg_id, msg_sndtype, pid, username, text);
		}
	}else{
		while(TRUE){
			message = ReceiveMsg(msg_id, msg_stype);
			if(message->pid != pid){
				printf("\n%s(%d):\n\t%s\n\n", message->username, message->pid, message->text);
			}
		}
	}

	return 0;
}













