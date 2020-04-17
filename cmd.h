#ifndef SLITHER_H
#define SLITHER_H

int slitherCmd(uint8_t *srvCmd,uint32_t srvLen);
uint8_t * slitherSetUsername(char *name,uint8_t skin,uint16_t *cmdLen);
#endif
