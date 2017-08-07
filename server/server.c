/*
#**********************************
#    File Name: server.c
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-07-31 12:36:41
#Last modified: 2017-08-07 13:37:41
#**********************************
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include"chat.h"

#define MAX_TEXT 512
#define TRUE 1
#define FALSE 0

//typedef struct NODE{
//	int mtype;
//	char username[15];
//	pid_t pid;
//	struct NODE *next;
//}NODE, *LINKLIST;


int main(){
	int ret = -1;
	int msg_id = -1;
	MSGBUF *msgbuf;
	LINKLIST head, p;
	MESSAGE *message;
	NODE_RETURN *insert_head;
	int msg_type = 10, msg_rtype, msg_rcvtype = 12, msg_ptype =13;
	key_t key;
	char *msgpath = "~/Desktop";
	key = ftok(msgpath, 1);
	//管道,用于父进程向子进程发送链表头结点地址
	int fd[2];
	FILE *fp;
	int *write_fd = &fd[1];
	int *read_fd = &fd[2];
	//printf("test\n");
	msg_id = msgget(key, 0666 | IPC_CREAT);
	if(msg_id == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		return 0;
	}
	/*初始化存储链表  */
	head = (LINKLIST)malloc(sizeof(NODE_LINKLIST));
	head->next = NULL;
	//printf("test2\n");
//	if(msgrcv(msg_id, msgbuf, sizeof(MSGBUF), msg_type, 0) == -1){
//		printf("msgreceive 失败\n");
//	}
	//printf("msg_id:%d\n", msg_id);
	pid_t jpid;
	jpid = fork();
	if(jpid < 0){
		printf("fork()进程失败\n");
		return -1;
	}else if(jpid > 0){
		//父进程监听申请
		while(TRUE){
			msgbuf = ReceiveRequest(msg_id, msg_type);
			if(NULL == msgbuf){
				printf("接收消息失败\n");
				return 0;
			}
			insert_head = InsertNode(head, msgbuf->username, msgbuf->pid);
			head = insert_head->head;
			msg_rtype = insert_head->mtype;
			SendMtype(msg_id, msg_rtype);	//返回申请端口:msg_rtype
			//SendHead(msg_id, msg_ptype, head);
			fp = fopen("./config.txt","w");
			p = head->next;
			int count = 0;
			while(p){
				fprintf(fp,"%d\t", p->mtype);
				p = p->next;
				++count;
			}
			fclose(fp);
			printf("----%s 加入聊天当前共%d人-----\n", msgbuf->username, count);
		}
	}else{
		//子进程监听聊天信息
		int temp;
		while(TRUE){
			message = ReceiveMsg(msg_id, msg_rcvtype);
			printf("%s(%d):\n\t%s\n", message->username, message->pid, message->text);
			//群发消息模块
			fp = fopen("./config.txt", "r");
			while(!feof(fp)){
				//遍历文件里的接收消息类型,转发消息
				fscanf(fp, "%d", &temp);
				if(!feof(fp)){
					SendMsg(msg_id, temp, message->pid, message->username, message->text);
				}
			}
			fclose(fp);
		}
	}

//	while(running){
//		if(msgrcv(msg_id, &data, MAX_TEXT, msg_type, 0) == -1){
//			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
//			return 0;
//		}
//		printf("You wrote: %s\n", data.mtext);
//		if(strncmp(data.mtext, "end", 3) == 0){
//			running = 0;
//		}
//	}
//	if(msgctl(msg_id, IPC_RMID, 0) == -1){
//		fprintf(stderr, "msgctl(IPC_RMID) failer\n");
//		return 0;
//	}
	return 0;
}
