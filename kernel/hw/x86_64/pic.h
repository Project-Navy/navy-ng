#pragma once 

#define MASTER_CMD      0x20
#define MASTER_DATA     0x21
#define MASTER_OFFSET   0x20

#define SLAVE_CMD       0xa0
#define SLAVE_DATA      0xa1
#define SLAVE_OFFSET    0x28

void pic_init(void);
void pic_sendEOI(int);