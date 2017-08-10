/*
#**********************************
#    File Name: chat.c
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-08-04 14:45:38
#Last modified: 2017-08-10 12:58:12
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
#include<time.h>

#define SIZE sizeof(MSGBUF)

int SendRequest(int msg_id, int mtype, int m_type, char *username, int flag){
	//flag为1表示申请加入聊天,为0表示申请退出聊天
	MSGBUF *msgbuf;
	msgbuf = (MSGBUF *)malloc(SIZE);
	msgbuf->mtype = mtype;
	msgbuf->m_type = m_type;
	strcpy(msgbuf->username, username);
	msgbuf->flag = flag;

	if(msgsnd(msg_id, msgbuf, SIZE, 0) == -1){
		printf("msgsnd failer\n");
		return 0;
	}
	return 0;
}

MSGBUF *ReceiveRequest(int msg_id, int mtype){
	MSGBUF *msgbuf;
	//printf("调用函数成功\n");	
	msgbuf = (MSGBUF *)malloc(SIZE);
	if(msgrcv(msg_id, msgbuf, SIZE, mtype, 0) == -1){
		printf("msgrcv failed\n");
		return NULL;
	}
	//printf("msg 接收成功\n");
	//printf("pid:%d\n", msgbuf->pid);		
	return msgbuf;
}

NODE_RETURN *InsertNode(LINKLIST head, char *username){
	int i = 15;		//客户端接收信息端口 起始为15;
	LINKLIST p, p_pre, nnode;
	NODE_RETURN *r_node;
	p_pre = head;		//始终指向p的前驱
	p = head->next;
	while(p && i == p->mtype){		//查找适当的插入位置
		p_pre = p;
		p = p->next;
		++i;
	}
	if(!p){//插入位置在最后
		nnode = (LINKLIST)malloc(sizeof(NODE_LINKLIST));
		nnode->mtype = i;
		strcpy(nnode->username, username);
		nnode->m_type = i;
		//插入节点,在链表末尾
		nnode->next = NULL;
		p_pre->next = nnode;
	}else{
		nnode = (LINKLIST)malloc(sizeof(NODE_LINKLIST));
		nnode->mtype = i;
		strcpy(nnode->username, username);
		nnode->m_type = i;
		//插入节点,在p_pre之后
		nnode->next = p_pre->next;
		p_pre->next = nnode;
	}
	
	r_node = (NODE_RETURN*)malloc(sizeof(NODE_RETURN));
	r_node->head = head;
	r_node->mtype = i;

	return r_node;
}

LINKLIST DeleteNode(LINKLIST head, int mtype){
	LINKLIST p, p_pre;
	p_pre = head;
	p = head->next;
	while(p){
		//printf("p->mtype = %d, mtype = %d\n", p->mtype, mtype);
		if(p->m_type == mtype){
			p_pre->next = p->next;
			free(p);
			return head;
		}
		p_pre = p;
		p = p->next;
	}
	return head;
}


int SendMtype(int msg_id, int mtype){
	MTYPE *s_mtype;
	s_mtype = (MTYPE *)malloc(sizeof(MTYPE));
	s_mtype->mtype = 11;	//发送该消息的类型
	s_mtype->r_mtype = mtype;	//客户端申请的消息类型
	if(msgsnd(msg_id, s_mtype, sizeof(MTYPE), 0) == -1){
		printf("消息类型发送失败\n");
		return -1;
	}
	return 0;
}

int ReceiveMtype(int msg_id, int mtype){
	MTYPE *r_mtype;
	r_mtype = (MTYPE *)malloc(sizeof(MTYPE));
	if(msgrcv(msg_id, r_mtype, sizeof(MTYPE), mtype, 0) == -1){
		fprintf(stderr, "消息类型接收失败\n");
		return -1;
	}
	return r_mtype->r_mtype;
}

int SendMsg(int msg_id, int mtype, int m_type, char *username, char *text){
	MESSAGE message;
	message.mtype = mtype;
	message.m_type = m_type;
	strcpy(message.username, username);
	strcpy(message.text, text);
	strcpy(message.stime, ""); //时间置空
	if(msgsnd(msg_id, &message, sizeof(MESSAGE), 0) == -1){
		printf("message发送失败\n");
		return -1;
	}
	//printf("message发送成功,消息类型%d\n", mtype);
	return 0;
}

MESSAGE *ReceiveMsg(int msg_id, int mtype){
	MESSAGE *message;
	message = (MESSAGE *)malloc(sizeof(MESSAGE));
	if(msgrcv(msg_id, message, sizeof(MESSAGE), mtype, 0) == -1){
		printf("message接收失败\n");
		return NULL;
	}
	return message;
}

int SendHead(int msg_id, int mtype, LINKLIST head){
	NODE_RETURN *node;
	node = (NODE_RETURN *)malloc(sizeof(NODE_RETURN));
	node->mtype = mtype;
	node->head = head;
	if(msgsnd(msg_id, node, sizeof(NODE_RETURN), 0) == -1){
		printf("head发送子进程失败\n");
		return -1;
	}
	return 0;
}

LINKLIST ReceiveHead(int msg_id, int mtype){
	 NODE_RETURN *node;
     node = (NODE_RETURN *)malloc(sizeof(NODE_RETURN));
	    if(msgrcv(msg_id, node, sizeof(NODE_RETURN), mtype, 0) == -1){
		         printf("head接收失败\n");
		         return NULL;
		     }
		     return node->head;
		 }

