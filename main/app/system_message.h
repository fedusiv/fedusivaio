#ifndef __SYSTEM_MESSAGE_H__
#define __SYSTEM_MESSAGE_H__

#include <stdint.h>
#include "types.h"
#include "op_codes.h"

#define SYS_MSG_DATA_SIZE 4

typedef enum
{
    MSG_DST_IM,
    MSG_DST_APP,
    MSG_DST_AU,
    MSG_DST_GUI,
    MSG_DST_MAX
}sys_msg_destination_e;

typedef struct _sys_msg_t sys_msg_t;
struct _sys_msg_t{
    sys_msg_op_code_e op_code;
    uint8_t data[SYS_MSG_DATA_SIZE]; 
    sys_msg_t * next_pnt;
};


void system_message_module_init(); // initialization of module, creates queue managment
void create_message(sys_msg_op_code_e opcode, u8 * data, sys_msg_destination_e destination, u16 time_ms); 
void pull_message(sys_msg_destination_e from, sys_msg_t ** msg);
void relese_message(sys_msg_t * msg); // release memory, which can be used for other messages

#endif //__SYSTEM_MESSAGE_H__
