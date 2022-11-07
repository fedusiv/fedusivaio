#ifndef __SYSTEM_MESSAGE_H__
#define __SYSTEM_MESSAGE_H__

#include <inttypes.h>
#include <stdint.h>

typedef enum
{
    OP_ZERO,
    OP_BUTTON_PRESSED,
    OP_ENCODER_CCW,
    OP_ENCODER_CW,
    OP_MAX
}sys_msg_op_code_e;

typedef enum
{
    MSG_DST_IM,
    MSG_DST_APP,
    MSG_DST_AU,
    MSG_DST_GUI,
    MSG_DST_MAX
}sys_msg_destination_e;

typedef struct 
{
    sys_msg_op_code_e op_code;
    uint16_t data;
}sys_msg_t;

typedef struct
{
    sys_msg_t * msg_pnt;
    struct sys_msg_queue_t * next;
}sys_msg_queue_t;


void system_message_module_init();
void create_message(sys_msg_op_code_e opcode, uint16_t data, sys_msg_destination_e destination);
sys_msg_op_code_e pull_message();

#endif //__SYSTEM_MESSAGE_H__
