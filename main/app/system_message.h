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

typedef struct _sys_msg_t sys_msg_t;
struct _sys_msg_t{
    sys_msg_op_code_e op_code;
    uint16_t data;
    sys_msg_t * next_pnt;
};


void system_message_module_init();
void create_message(sys_msg_op_code_e opcode, uint16_t data, sys_msg_destination_e destination);
void pull_message(sys_msg_destination_e from, sys_msg_t ** msg);

#endif //__SYSTEM_MESSAGE_H__
