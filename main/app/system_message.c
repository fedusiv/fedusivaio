#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "system_message.h"
#include "op_codes.h"

#define SYS_MSG_ARRAY_SIZE 30

sys_msg_t sys_msg_storage[SYS_MSG_ARRAY_SIZE]; // array in memory to store all messages to communicate through modules
sys_msg_t * sys_msg_array[SYS_MSG_ARRAY_SIZE]; // array to have access to storages, part of mechanism to provide O(1) access to push and pull
sys_msg_t * modules_queue_head[MSG_DST_MAX]; // array of pointers to list for messages to different modules 
uint16_t sys_msg_array_pos; // position, represent from where can be taken pointer and where to put
SemaphoreHandle_t xSemaphore = NULL;


void system_message_module_init()
{
    // init of storage for messages and message manager
    memset(sys_msg_storage, 0, sizeof(sys_msg_t) * SYS_MSG_ARRAY_SIZE);
    sys_msg_array_pos = 0;
    // Set pointers for sys messages array to
    for(uint8_t i = 0; i < SYS_MSG_ARRAY_SIZE; i++)
    {
        sys_msg_array[i] = &sys_msg_storage[i];
    }
    // Set to NULL pointers everything related to queue of modules 
    for(sys_msg_destination_e i = 0; i < MSG_DST_MAX; i++)
    {
        modules_queue_head[i] = NULL;
    }
    // create queue for modules
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive( xSemaphore );
}

void create_message(sys_msg_op_code_e opcode, uint8_t * data, sys_msg_destination_e destination)
{
    sys_msg_t * msg;
    sys_msg_t * queue_msg;
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        // take memory from storage
        msg = (sys_msg_array[sys_msg_array_pos]);
        sys_msg_array_pos++;
        // fill with data
        msg->op_code = opcode;
        // copy data
        memcpy(msg->data, data, sizeof(uint8_t) * SYS_MSG_DATA_SIZE);
        // put messages to destination queue
        msg->next_pnt = NULL;
        if(modules_queue_head[destination] == NULL)
        {
            modules_queue_head[destination] = msg;
        }
        else
        {
            queue_msg = modules_queue_head[destination];
            while(queue_msg->next_pnt != NULL)
            {
                queue_msg = queue_msg->next_pnt;
            }
            queue_msg->next_pnt = msg;
        }
        xSemaphoreGive( xSemaphore );
    }
}

void pull_message(sys_msg_destination_e from, sys_msg_t ** msg)
{
    sys_msg_t * msg_q;
    *msg = NULL; // no messages in queue
    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1 ) == 1 )
    {
        msg_q = modules_queue_head[from];
        if(msg_q != NULL)
        {
           // there is message in queue, removed message from queue
           modules_queue_head[from] = msg_q->next_pnt;
           *msg = msg_q;
        }
        xSemaphoreGive( xSemaphore );
    }
}

void relese_message(sys_msg_t * msg)
{

    if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1000 ) == 1 )
    {
        sys_msg_array_pos--;
        sys_msg_array[sys_msg_array_pos] = msg;
        xSemaphoreGive( xSemaphore );
    }
}
