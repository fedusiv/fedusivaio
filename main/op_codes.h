#ifndef __OP_CODES_H__
#define __OP_CODES_H__

#include <inttypes.h>

typedef enum
{
    OP_ZERO,
    OP_BUTTON_ACTION,
    OP_ENCODER_ACTION,
    OP_PLAY_NOTE,
    OP_MAX
}sys_msg_op_code_e;

typedef enum
{
    INPUT_OP_ZERO,
    INPUT_OP_BUTTON_PRESSED,
    INPUT_OP_BUTTON_RELEASED,
    INPUT_OP_ENCODER_CW,
    INPUT_OP_ENCODER_CCW,
    INPUT_OP_MAX
}input_msg_op_code_e;


// ****** Data structures ********* //

typedef struct _input_action_t input_action_t;
struct _input_action_t
{
    uint8_t id; // id which button or encoder was provided
    input_msg_op_code_e opcode; // type of actiob was done
}__attribute__ ((packed));

#endif // __OP_CODES_H__
