#ifndef __SYSTEM_MESSAGE_H__
#define __SYSTEM_MESSAGE_H__

#include <inttypes.h>

typedef enum
{
    OP_ZERO,
    OP_BUTTON_PRESSED,
    OP_ENCODER_CCW,
    OP_ENCODER_CW,
    OP_MAX
}sys_msg_op_code_e;

typedef struct 
{
    sys_msg_op_code_e op_code;
    uint8_t free;
}sys_msg_t;

typedef struct
{
    sys_msg_t * msg_pnt;
    struct sys_msg_queue_t * next;
}sys_msg_queue_t;


void system_message_module_init();
void put_message(sys_msg_op_code_e opcode);
sys_msg_op_code_e pull_message();

#endif //__SYSTEM_MESSAGE_H__