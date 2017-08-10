/*
#**********************************
#    File Name: chat.h
#       Author:wozaizhe55
#         mail:
#   Creat Time: 2017-08-04 14:27:54
#Last modified: 2017-08-08 00:15:12
#**********************************
*/

#ifndef _CHAT_H_
#define _CHAT_H_
//#include <sys/types.h>
//#include <unistd.h>
typedef struct{
	int mtype;
	int m_type;
	char username[15];
	int flag;   //表示申请加入/退出聊天的状态:1为加入,0为退出;
}MSGBUF;

typedef struct MESSAGE{
	int mtype;
	int m_type;
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
	int m_type;
	struct NODE_LINKLIST *next;
}NODE_LINKLIST, *LINKLIST;

typedef struct NODE_RETURN{
	int mtype;
	struct NODE_LINKLIST *head;
}NODE_RETURN;


extern int SendRequest(int, int, int, char*, int);
extern int SendMtype(int, int);
extern int ReceiveMtype(int, int);
extern int SendMsg(int, int, int, char*, char*);
extern MSGBUF *ReceiveRequest(int, int);
extern MESSAGE *ReceiveMsg(int, int);
extern NODE_RETURN *InsertNode(LINKLIST, char*);
extern LINKLIST DeleteNode(LINKLIST, pid_t);
extern int SendHead(int, int, LINKLIST);
extern LINKLIST ReceiveHead(int, int);
#endif
