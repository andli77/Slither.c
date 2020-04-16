#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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
int slitherCmd(uint8_t *srvCmd,uint32_t srvLen)
{
  uint8_t* rPtr;
  switch(srvCmd[2])
  {
    case '6':
      rPtr = decodeSecret((char*)srvCmd);
      wsWrite(rPtr,24);
    break;
    default:
      printf("unknown command %02X '%c'\n",srvCmd[2],srvCmd[2]);
    break;
  }
 return 0;
}
