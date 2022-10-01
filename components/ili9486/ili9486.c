#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "../../main/gpio_config.h"

static spi_device_handle_t spi_handle;

void ili9486_spi_init()
{
    
    esp_err_t ret;

    spi_bus_config_t buscfg={
        .miso_io_num=SPI_MISO_DISPLAY,
        .mosi_io_num=SPI_MOSI_DISPLAY,
        .sclk_io_num=SPI_CLK_DISPLAY,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=480*2+8
    };

    spi_device_interface_config_t spi_dev_cfg = 
    {
        .spics_io_num = SPI_CS_DISPLAY,
        .clock_speed_hz = 10 * 1000 * 1000,
        .mode = 0,
        .queue_size = 7
    };
    // Initialize spi bus
    ret = spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    // attach spi to screen
    ret = spi_bus_add_device(SPI3_HOST, &spi_dev_cfg, &spi_handle);
    ESP_ERROR_CHECK(ret);

}


void screen_init()
{

}
