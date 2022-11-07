#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "system_message.h"

#define SYS_MSG_ARRAY_SIZE 30
#define NO_RELEASE_MSG 0xFF // marking, that there is nowhere to release

sys_msg_t sys_msg_storage[SYS_MSG_ARRAY_SIZE]; // array in memory to store all messages to communicate through modules
sys_msg_queue_t modules_queue_head[MSG_DST_MAX]; // head to list for all modules
SemaphoreHandle_t xSemaphore = NULL;

struct
{
    uint8_t available_pos;
    uint8_t release_pos;
    sys_msg_t * storage;
}sys_msg_manager;

void system_message_module_init()
{
    // init of storage for messages and message manager
    memset(sys_msg_storage, 0, sizeof(sys_msg_t) * SYS_MSG_ARRAY_SIZE);
    sys_msg_manager.available_pos = 0;
    sys_msg_manager.release_pos = NO_RELEASE_MSG;
    // create queue for modules
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive( xSemaphore );
}

void create_message(sys_msg_op_code_e opcode, uint16_t data, sys_msg_destination_e destination)
{
    sys_msg_t * msg;
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        // take memory from storage
        msg = &(sys_msg_manager.storage[sys_msg_manager.available_pos]);
        sys_msg_manager.available_pos++;
        sys_msg_manager.release_pos++;
        // fill with data
        msg->op_code = opcode;
        msg->data = data;
        // put messages to destination
        xSemaphoreGive( xSemaphore );
    }
}

sys_msg_op_code_e pull_message()
{
    sys_msg_op_code_e op_code = OP_ZERO;
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        xSemaphoreGive( xSemaphore );
    }
    return op_code;
}
