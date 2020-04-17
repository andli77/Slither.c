#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
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
int slitherCmd(uint8_t *srvCmd,uint32_t srvLen)
{
  uint8_t* rPtr;
  switch(srvCmd[2])
  {
    case '6':
      rPtr = decodeSecret((char*)srvCmd);
      wsWrite(rPtr,24);
    break;
    case 'a'://	Initial setup
    break;
case 'e'://	Snake rotation counterclockwise (?dir ang ?wang ?sp)
case 'E'://	Snake rotation counterclockwise (dir wang ?sp)
case '3'://	Snake rotation counterclockwise (dir ang wang | sp)
case '4'://	Snake rotation clockwise (dir ang? wang ?sp)
case '5'://	Snake rotation clockwise (dir ang wang)
case 'h'://	Update snake last body part fullness (fam)
case 'r'://	Remove snake part
case 'g'://	Move snake
case 'G'://	Move snake
case 'n'://	Increase snake
case 'N'://	Increase snake
case 'l'://	Leaderboard
case 'v'://	dead/disconnect packet
case 'W'://	Add Sector
case 'w'://	Remove Sector
case 'm'://	Global highscore
case 'p'://	Pong
case 'u'://	Update minimap
case 's'://	Add/remove Snake
case 'F'://	Add Food
case 'b'://	Add Food
case 'f'://	Add Food
case 'c'://	Food eaten
case 'j'://	Update Prey
case 'y'://	Add/remove Prey
case 'o'://	Verify code response
case 'k'://	Kill (unused in the game-code)


    default:
      printf("unknown command %02X '%c'\n",srvCmd[2],srvCmd[2]);
    break;
  }
 return 0;
}
