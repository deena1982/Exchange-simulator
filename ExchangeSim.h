#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <iostream>
#include <pthread.h>
#include <stdint.h>



#pragma pack(push,1)
//define any structs here

struct Heartbeatmsg
{
	u16 sbeFrameLen;
	u16 sbeBlockLen;
	u16 sbeTemplateId;
	u16 sbeSchemaId;
	u16 sbeHeaderVersion;

	void setHBMsgFields(u16 frameLen, u16 blkLen, u16 tempId, u16 schemeId, u16 hdrVer)
	{
		sbeFrameLen = frameLen;
		sbeBlockLen = blkLen;
		sbeTemplateId = tempId;
		sbeSchemaId = schemeId;
		sbeHeaderVersion = hdrVer;
	}
};
#pragma pack (pop)


int processSession(char *, int);
void prepareLoginAccept(char* sendBuff, int);
void *HeartbeatThread(void*);
void sig_callback(int signum);
int connfd = 0;

