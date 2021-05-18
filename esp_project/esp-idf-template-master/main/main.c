#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "includes/esp32-rotary-encoder-master/include/rotary_encoder.h"



void app_main(void)
{
    int i = 0;
    while (1) {
        printf("[%d] Hello world! Hamish was here (:\n", i);
        i++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
