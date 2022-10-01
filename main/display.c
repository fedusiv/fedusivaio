#include <esp_system.h>

#include "config.h" 
#include "ili9486.h"

void xDisplayTask(void * taskParameters)
{

    ili9486_spi_init();
    for(;;)
    {
        
    }
}
