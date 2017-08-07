/*
#**********************************
#    File Name: chat.h
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-08-04 14:27:54
#Last modified: 2017-08-05 20:40:36
#**********************************
*/

#ifndef _CHAT_H_
#define _CHAT_H_
//#include <sys/types.h>
//#include <unistd.h>
typedef struct{
	int mtype;
	pid_t pid;
	char username[15];
}MSGBUF;

typedef struct MESSAGE{
	int mtype;
	pid_t pid;
	char username[15];
	char text[300];
	char stime[20];
}MESSAGE;


typedef struct{
	int mtype;
	int r_mtype;
}MTYPE;

typedef struct NODE_LINKLIST{
	int mtype;	//client接收的message消息类型
	char username[15];
	pid_t pid;
	struct NODE_LINKLIST *next;
}NODE_LINKLIST, *LINKLIST;

typedef struct NODE_RETURN{
	int mtype;
	struct NODE_LINKLIST *head;
}NODE_RETURN;


extern int SendRequest(int, int, pid_t, char*);
extern int SendMtype(int, int);
extern int ReceiveMtype(int, int);
extern int SendMsg(int, int, pid_t, char*, char*);
extern MSGBUF *ReceiveRequest(int, int);
extern MESSAGE *ReceiveMsg(int, int);
extern NODE_RETURN *InsertNode(LINKLIST, char*, pid_t);
extern int SendHead(int, int, LINKLIST);
extern LINKLIST ReceiveHead(int, int);
#endif
