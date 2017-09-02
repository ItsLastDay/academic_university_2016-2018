#ifndef __PIT_H__
#define __PIT_H__

static const int PIT_CMD_PORT = 0x43,
             PIT_DATA_PORT = 0x40;

void init_pit();

#endif
