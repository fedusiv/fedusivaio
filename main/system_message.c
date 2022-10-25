#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "system_message.h"

sys_msg_t sys_msg_storage[10];
SemaphoreHandle_t xSemaphore = NULL;

void system_message_module_init()
{
    for(int i = 0; i < 10; i++)
    {
        sys_msg_storage[i].free = 1;
    }
    xSemaphore = xSemaphoreCreateBinary();
}

void put_message(sys_msg_op_code_e opcode)
{
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        for(int i = 0; i < 10; i++)
        {
            if(sys_msg_storage[i].free)
            {
               sys_msg_storage[i].free = 0;
               sys_msg_storage[i].op_code = opcode;
               break;
            }
        }
        xSemaphoreGive( xSemaphore );
    }
}

sys_msg_op_code_e pull_message()
{
    sys_msg_op_code_e op_code = OP_ZERO;
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        for(int i = 0; i < 10; i++)
        {
            if( !(sys_msg_storage[i].free) )
            {
               sys_msg_storage[i].free = 1;
               op_code = sys_msg_storage[i].op_code;
               break;
            }
        }
        xSemaphoreGive( xSemaphore );
    }
    return op_code;
}
