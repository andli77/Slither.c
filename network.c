#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "cmd.h"
int gSockfd=0;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int webConnect(const char *ip,const char *port)
{

  int portno;
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  portno = atoi(port);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
      error("ERROR opening socket");
  server = gethostbyname(ip);
  if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
      error("ERROR connecting");
  return sockfd;
}
#define HTTP_BUFF_SIZE 1024
char * generateHTTPget(const char *ip,const char *port)
{
  char *httpBuffer = malloc(1024);
  uint16_t pos=0;
  pos += snprintf(httpBuffer,HTTP_BUFF_SIZE-pos,"GET /slither HTTP/1.1\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Host: %s:%s\r\n",ip,port);
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Connection: Upgrade\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Upgrade: websocket\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Origin: http://slither.io\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Sec-Websocket-Key: q4xkcO32u266gldTuKaSOw==\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"Sec-WebSocket-Version: 13\r\n");
  pos += snprintf(&httpBuffer[pos],HTTP_BUFF_SIZE-pos,"\r\n");
return httpBuffer;
}
#define WS_OP_CONTINUES 0x00
#define WS_OP_TEXT      0x01
#define WS_OP_BIN       0x02
#define WS_OP_CLOSE     0x08
#define WS_OP_PING      0x09
#define WS_OP_PONG      0x0A
char * parseWebSocketPkt(char *data,int len,uint32_t *returnLen)
{
  //uint32_t dataLen;
  if((data[0]&0x80)==0)
  {
    printf("Partial packet! \n");
    return 0;
  }
  else
  {
    if((data[0]&0x0F)==WS_OP_BIN)
    {
      printf("WS_OP_BIN ");
    } 
    if((data[1]&0x80))
    {
      printf("DATA MASKED! \n");
      return 0;
    } 
    if((data[1]&0x7F)==0x7E) //16bit len
    {
      *returnLen = ((uint8_t)data[2])*0x100+(uint8_t)data[3];
      printf("l = %d \n",*returnLen);
      return &data[4];
    }
    else //7 bit len
    {
      *returnLen = (data[1]&0x7F);
      printf("l = %d\n",*returnLen);
      return &data[2];
    }
  }
  
}
char * generateWebSocketPkt(char *data, int len)
{
/*
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+*/

  char *wsPkt = malloc(2+len);
  wsPkt[0]=0x80+WS_OP_BIN;
  if(len < 127)
  {  
    wsPkt[1]=len;
  }
  else
  {
  }
    //should support longer msg
  memcpy(&wsPkt[2],data,len);
  return wsPkt;
}
int wsWrite(uint8_t *data,uint16_t len)
{
 char *wsReq;
 int rcWrite;
   wsReq=generateWebSocketPkt((char*)data,len);
    rcWrite=write(gSockfd,wsReq,len+2);
   printf("rcWrite = %d\n",rcWrite);
return rcWrite;
}

void hexDump(uint8_t *buff,int len)
{
  int i=0;
  for(i=0;i<len;i++)
  {
    if(i%16==0)
      printf("\n");
    printf("%02X ",(uint8_t)buff[i]);
  }
  printf("hi!\n");
} 
int main(int argc, char *argv[])
{
    int  rcWrite,rcRead;
    char httpRsp[1024];
    char *httpReq;
    char *wsReq;
    char *srvCmd;
    uint32_t srvLen;
    char cmdBuff[16];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    gSockfd=webConnect(argv[1],argv[2]);
    printf("sockfd = %d\n",gSockfd);
    httpReq = generateHTTPget(argv[1],argv[2]);
    printf("---http---\n%s---http---\n",httpReq);
    rcWrite=write(gSockfd,httpReq,strlen(httpReq));
    free(httpReq);
    printf("write = %d\n",rcWrite);
    rcRead=read(gSockfd,httpRsp,1024);
    printf("read = %d\n",rcRead);
    printf("---resp---\n%s---resp---\n",httpRsp);
    cmdBuff[0]='c';
    wsReq=generateWebSocketPkt(cmdBuff,1);
    rcWrite=write(gSockfd,wsReq,3);
    printf("write = %d\n",rcWrite);
    rcRead=read(gSockfd,httpRsp,1024);
    printf("read = %d\n",rcRead);
    srvCmd = parseWebSocketPkt(httpRsp,rcRead,&srvLen);
    slitherCmd((uint8_t*)srvCmd,srvLen);

    close(gSockfd);
    return 0;
}
