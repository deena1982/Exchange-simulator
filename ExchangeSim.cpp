#include "ExchangeSim.h"
using namespace std;


int main(int argc, char* argv[])
{
	int listenfd  = 0;
	int slistenFileDes = 0;
	struct sockaddr_in server_address;

	char sendBuff[10240];
	char readBuff[10240];
	int mType, rtype, connectionClosed=0;
	u64 len=0;
	MessageHeader hdr;
	Logon logonMsg;

	signal (SIGINT, signal_callback);
	listenfd  = socket (AF_INET,SOCK_STREAM,0);
	memset (&server_address, '0', sizeof(server_address));
	memset (sendBuff, '0', sizeof(sendBuff));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port htons(atoi(argv[1]));

	if ((bind(listenfd, (struct sockaddr*)&server_address,sizeof(server_address))) < 0)
	{
		cout<<"Unable to bind to input port";
		perror("Bind");
		exit(1);
	}

	//listen to 10 connections
	listen (listenfd, 10);

	cout<<"Waiting for Connection Gateway request port\n"<<ntohs(server_address.sin_port)<<"::"<<atoi(argv[1])<<"\n";
	connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);
	ConnectionClosed =  0;
	cout<<"reading Connection data.....\n";
	len = read(connfd,readBuff,size(readBuff));

	if (len <= 0)
	{ cout<<"Error reading from connection";
	exit(1);
	}

	hdr.wrap(readBuff, 2,0, len);
	u16 msgId = hdr.templateId();
	cout<<"Message Template id="<<msgId<<"\n";


	while (!connectionClosed)
	{
		switch(msgId)

		case 100:
		{
			if ((write(connectionFileDes, sendBuff, pktlen) < pktlen))
			{
				cout<<"Unable to send Login Response";
				exit(1);
			}

			processSession((char*) ReadBuff, sizeoff(ReadBuff));
			break;
		}

		default:
		{

			cout<<"recieved unknown message";
			close(connfd);
			cout<<"connection closed\n";
			connectionClosed=1;
			break;
		}


	}

}

int processSession(char* readBuff, int buffSize)
{
	pthread_t heartBeadThread;
	char sendBuff[10240];
	int len = 0;
	MessageHeader hdr;

	if (pthread_create(&heartBeadThread, NULL,heartBeadThread, NULL) !=0)
	{
		cout<<"Unable to create a readerThread thread.....\n";
		exit(2);
	}

	while(1)
	{
		cout<<"waiting for data...";
		len = read(connfd, ReadBuff,buffsize);
		if( len <= 0)
		{
			cout<<"Error reading from channel\n";
			exit(1);
		}

		hdr.wrap(ReaderBuff, 2, 0, len);
		u16 msgId = hdr.templateId();

		switch(msgId)
		{
			case 106:
			{
				cout<<"heartbeat message received!\n";
				break;
			}

			default :
			{
				cout<<"unknown msg received\n";
			}
		}
	}
	return 0;
}

void prepareLoginAck(char* sendBuff, int buffSize)
{
	try {
		LogonAck logonAck;
		MessageHeader hdr;

		hdr.wrap (sendBuff ,2,0,buffSize)
				.blockLength(logonAck.sbeBlockLenght())
				.templateId(logonAck.sbeTemplateId())
				.schemaId(logonAck.sbeSchemaId())
				.version(logonAck.sbeSchemaVersion());

		logonAck.wrapForEncode(sendBuff, 2 + MessageHeader::encodedLength(),\
				sizeof(sendBuff) - 2- MessageHeader::encodedLength());
		*((u16 *)(sendBuff)) = 2 + MessageHeader::encodedLength() + LogonAck::sbeBlockLenght();
	}
	catch (...)

	{}
}

void *HeartbeatThread(void*)
{
	Heartbeatmsg HB;
	cout<<"HB thread created";
	memset(&HB,'\0',sizeof(HB));

	HB.setHBMsgFields(10,0,106,0,0);

	while (1)
	{
		sleep(1);
		write(connfd, &HB, sizeof(HB));
	}
}
