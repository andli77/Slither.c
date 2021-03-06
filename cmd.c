#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "network.h"
uint8_t miniMap[800];  //80x80 bits x/y
uint8_t * decodeSecret(char *secret)
{
  uint8_t *result = malloc(24);
  int globalValue = 0;
  for (int i = 0; i < 24; i++)
  {
    int value1 = secret[17 + i * 2];
    if (value1 <= 96)
    {
      value1 += 32;
    }
    value1 = (value1 - 98 - i * 34) % 26;
    if (value1 < 0)
    {
      value1 += 26;
    }
    int value2 = secret[18 + i * 2];
    if (value2 <= 96)
    {
      value2 += 32;
    }
    value2 = (value2 - 115 - i * 34) % 26;
    if (value2 < 0)
    {
      value2 += 26;
    }
    int interimResult = (value1 << 4) | value2;
    int offset = interimResult >= 97 ? 97 : 65;
    interimResult -= offset;
    if (i == 0)
    {
      globalValue = 2 + interimResult;
    }
    result[i] = (char) ((interimResult + globalValue) % 26 + offset);
    globalValue += 3 + interimResult;
  }
  return result;

}
uint8_t * slitherSetUsername(char *name,uint8_t skin,uint16_t *outLen)
{
  uint8_t *buffer = malloc(strlen(name)+4);
  buffer[0]='s';
  buffer[1]=10;
  buffer[2]=skin;
  buffer[3]=strlen(name);
  memcpy(&buffer[4],name,buffer[3]);
  *outLen = buffer[3]+4; 
  return buffer;
}

void updateMiniMap(uint8_t *data,uint32_t len)
{
	const uint8_t getMask[7]={0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	const uint8_t setMask[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

	uint32_t i;
  uint8_t p;
  uint32_t currentByte=0;
  uint8_t currentBit=0;
	uint16_t pixelCnt=0;
	memset(miniMap,0,800);
	for(i=0;i<len;i++)
	{
		//printf("%02X ",data[i]);
		if(data[i]&0x80)
		{
	//		printf("clear %d pixels\n",data[i]&0x7F);
			currentByte += (data[i]&0x7F)/8;
			currentBit += (data[i]&0x7F)%8;
			if(currentBit > 7)
			{
				currentByte++;
				currentBit -=8;
			}
			pixelCnt +=data[i]&0x7F;
		}
		else
		{
//		  printf("set 7 pixel %02X\n",data[i]&0x7F);
			for(p=0;p<7;p++)
			{
				if(data[i]&getMask[p])
				{
					miniMap[currentByte]|=setMask[currentBit];
				}
				currentBit++;
  	    if(currentBit > 7)
	      {
	        currentByte++;
	        currentBit -=8;
	      }
			}
			pixelCnt +=7;
		}
	}
	printf("totPixels = %d currentByte = %d currentBit = %d",pixelCnt,currentByte,currentBit);
/*  for(i=0;i<800;i++)
	{
		if(i%10==0)
			printf("\n");
		printf("%02X",miniMap[i]);		
	}*/
}
uint32_t gameRadius  = 16384;
uint16_t mscps       = 300;
uint16_t sectorSize = 480;
//a s p g W F
int slitherCmd(uint8_t *srvCmd,uint32_t srvLen)
{
  uint8_t* rPtr;
  uint32_t i,j;
	static clock_t lastPing = 0;
	uint8_t cmdBuff[16];
	clock_t now = clock();
	if(now-lastPing > CLOCKS_PER_SEC/10)
	{
    printf("PING!!!!!!!!\n");
		lastPing = now;
		cmdBuff[0]=251; //ping
		wsWrite(cmdBuff,1);
	}
	printf("time %04X cmd = '%c' len = %d ",srvCmd[0]*0x100+srvCmd[1],srvCmd[2],srvLen);
  switch(srvCmd[2])
  {
    case '6':
      rPtr = decodeSecret((char*)srvCmd);
      wsWrite(rPtr,24);
    break;
    case 'a'://	Initial setup
      gameRadius = srvCmd[3]*0x10000+srvCmd[4]*0x100+srvCmd[5];
      mscps = srvCmd[6]*0x100+srvCmd[7];
      sectorSize = srvCmd[8]*0x100+srvCmd[9];
      printf("gameRadius = %d mscps = %d sector_size = %d",gameRadius,mscps,sectorSize); 
    break;
		case 'e'://	Snake rotation counterclockwise (?dir ang ?wang ?sp)
    break;
		case 'E'://	Snake rotation counterclockwise (dir wang ?sp)
    break;
		case '3'://	Snake rotation counterclockwise (dir ang wang | sp)
    break;
		case '4'://	Snake rotation clockwise (dir ang? wang ?sp)
    break;
		case '5'://	Snake rotation clockwise (dir ang wang)
    break;
		case 'h'://	Update snake last body part fullness (fam)
    break;
		case 'r'://	Remove snake part
    break;
		case 'g'://	Move snake absolute
			printf("Move snake abs %04X X = %d Y=%d ",srvCmd[3]*0x100+srvCmd[4],srvCmd[5]*0x100+srvCmd[6],srvCmd[7]*0x100+srvCmd[8]);
    break;
		case 'G'://	Move snake relative
			printf("Move snake rel %04X X = %d Y=%d ",srvCmd[3]*0x100+srvCmd[4],(int8_t)(srvCmd[5]-128),(int8_t)(srvCmd[6]-128));
    break;
		case 'n'://	Increase snake
    break;
		case 'N'://	Increase snake
    break;
		case 'l'://	Leaderboard
			printf("Leader board own=%d total=%d/",srvCmd[4]*0x100+srvCmd[5],srvCmd[6]*0x100+srvCmd[7]);
			for(i=8;i<srvLen;)
			{
				for(j=0;j<srvCmd[i+6];j++)
				{
					if(srvCmd[i+7+j] < 32 || srvCmd[i+7+j] > 126)
						srvCmd[i+7+j]='_';
				}
				printf(" %.*s %d %d /",srvCmd[i+6],&srvCmd[i+7],srvCmd[i]*0x100+srvCmd[i+1],srvCmd[i+2]*0x10000+srvCmd[i+3]*0x100+srvCmd[i+4]);
				i+=srvCmd[i+6]+6;
			}
    break;
		case 'v'://	dead/disconnect packet
			printf("!!!!!!!!!!!!!!!!!!Player dead!!!!!!!!!!!!!!!! %d ",srvCmd[3]);
    break;
		case 'W'://	Add Sector
			printf("Add sector X=%d Y=%d ",srvCmd[3],srvCmd[4]);
    break;
		case 'w'://	Remove Sector
			printf("Remove sector X=%d Y=%d ",srvCmd[3],srvCmd[4]);
    break;
		case 'm'://	Global highscore
    break;
		case 'p'://	Pong
      printf("Pong");
    break;
		case 'u'://	Update minimap
			printf("miniMap");
			updateMiniMap(&srvCmd[3],srvLen-3);
    break;
		case 's'://	Add/remove Snake
			if(srvLen == 6) //remove snake
      {
        printf("Remove snake %04X dead = %d",srvCmd[3]*0x100+srvCmd[4],srvCmd[5]);
			}
			else //len should be more than 33
			{
        uint32_t snakeX = srvCmd[18]*0x100+srvCmd[19]*0x100+srvCmd[20];
				uint32_t snakeY = srvCmd[21]*0x100+srvCmd[22]*0x100+srvCmd[23];
        printf("Add snake %04X X = %d Y=%d ",srvCmd[3]*0x100+srvCmd[4],snakeX,snakeY);
        if(srvCmd[24]>0)
				{
					srvCmd[25+srvCmd[24]]=0;
					printf("n=%s",&srvCmd[25]);
				}
      }
    break;
		case 'F'://	Add Food
			i=3;
			printf("Add 'old' food");
			while(i<srvLen)
			{
				printf(" c = %d X = %d Y= %d s = %d/",srvCmd[i],srvCmd[i+1]*0x100+srvCmd[i+2],srvCmd[i+3]*0x100+srvCmd[i+4],srvCmd[i+5]);
				i+=6;
			}
    break;
		case 'b'://	Add Food
			i=3;
      printf("Add 'dead/turbo' food");
      while(i<srvLen)
      {
        printf(" c = %d X = %d Y= %d s = %d/",srvCmd[i],srvCmd[i+1]*0x100+srvCmd[i+2],srvCmd[i+3]*0x100+srvCmd[i+4],srvCmd[i+5]);
        i+=6;
      }
    break;
		case 'f'://	Add Food
			i=3;
      printf("Add 'spawn' food");
      while(i<srvLen)
      {
        printf(" c = %d X = %d Y= %d s = %d/",srvCmd[i],srvCmd[i+1]*0x100+srvCmd[i+2],srvCmd[i+3]*0x100+srvCmd[i+4],srvCmd[i+5]);
        i+=6;
      }
    break;
		case 'c'://	Food eaten
			if(srvLen >= 9)
			{
				printf("Food taken Snake %04X X = %d Y= %d ",srvCmd[7]*0x100+srvCmd[8],srvCmd[3]*0x100+srvCmd[4],srvCmd[5]*0x100+srvCmd[6]);
			}
			else if(srvLen == 7)
			{
				printf("Food taken X = %d Y= %d ",srvCmd[3]*0x100+srvCmd[4],srvCmd[5]*0x100+srvCmd[6]);
			}
    break;
		case 'j'://	Update Prey¨
//			printf("Prey move
    break;
		case 'y'://	Add/remove Prey
    break;
		case 'o'://	Verify code response
    break;
		case 'k'://	Kill (unused in the game-code)
    break;
    default:
      printf("unknown command %02X '%c'",srvCmd[2],srvCmd[2]);
    break;
  }
  printf("\n");
 return 0;
}
