#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
//#include "esp_log.h"
//#include "components/tarablessd1306/ssd1306.h"
//#include "components/tarablessd1306/ssd1306_default_if.h"
//#include "components/tarablessd1306/ssd1306_draw.h"
//#include "components/tarablessd1306/ssd1306_font.h"

#define ESP_INR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0
#define CONFIG_LED_PIN 2

// Task tutorial section
TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
void task1 (void *arg)
{
    printf("task2 has name: %s \n",pcTaskGetTaskName(myTask2Handle));
    while(1){
        int c = xTaskGetTickCount();
        printf("[%d] Hello from task1\n",c);
        vTaskDelay(pdMS_TO_TICKS(1000));
        // syspend task 2
        if (c == 300){
            vTaskSuspend(myTask2Handle);
            printf("task2 is suspended! %d \n",eTaskGetState(myTask2Handle));
        }
        // revive task 2
        if (c == 500){
            vTaskResume(myTask2Handle);
            printf("task2 is Resumed! %d \n",eTaskGetState(myTask2Handle));
        }
        // delete task 2
        if (c == 800){
            vTaskDelete(myTask2Handle);
            printf("task2 is deleted! %d \n",eTaskGetState(myTask2Handle));
        }
    }
}
void task2 (void *arg)
{
    for( int i = 0; i<5 ; i++)
    {
        printf("Hello from task2\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
//xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
//xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle, 1);

// Button tutorial section
TaskHandle_t ISR = NULL;
void IRAM_ATTR button_isr_handler(void *args)
{
    xTaskResumeFromISR(ISR);
}
void button_task(void *args)
{
    int c = 0;
    bool led_status = false;
    while(1)
    {
        c++;
        vTaskSuspend(NULL);
        led_status = !led_status;
        gpio_set_level(CONFIG_LED_PIN, led_status);
        printf("[%d] - I loooooveee youuuuuuuuuuuuuuuuu!\n", c);
    }
}

/*
// I2C screen
#define USE_I2C_DISPLAY
//#define USE_SPI_DISPLAY
#if defined USE_I2C_DISPLAY
    static const int I2CDisplayAddress = 0x3C;
    static const int I2CDisplayWidth = 128;
    static const int I2CDisplayHeight = 32;
    static const int I2CResetPin = -1;

    struct SSD1306_Device I2CDisplay;
#endif
#if defined USE_SPI_DISPLAY
    static const int SPIDisplayChipSelect = 15;
    static const int SPIDisplayWidth = 128;
    static const int SPIDisplayHeight = 64;
    static const int SPIResetPin = 5;

    struct SSD1306_Device SPIDisplay;
#endif
void SetupDemo( struct SSD1306_Device* DisplayHandle, const struct SSD1306_FontDef* Font );
void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText );
bool DefaultBusInit( void ) {
    #if defined USE_I2C_DISPLAY
        assert( SSD1306_I2CMasterInitDefault( ) == true );
        assert( SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true );
    #endif

    #if defined USE_SPI_DISPLAY
        assert( SSD1306_SPIMasterInitDefault( ) == true );
        assert( SSD1306_SPIMasterAttachDisplayDefault( &SPIDisplay, SPIDisplayWidth, SPIDisplayHeight, SPIDisplayChipSelect, SPIResetPin ) == true );
    #endif

    return true;
}
void SetupDemo( struct SSD1306_Device* DisplayHandle, const struct SSD1306_FontDef* Font ) {
    SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
    SSD1306_SetFont( DisplayHandle, Font );
}
void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText ) {
    SSD1306_FontDrawAnchoredString( DisplayHandle, TextAnchor_Center, HelloText, SSD_COLOR_WHITE );
    SSD1306_Update( DisplayHandle );
}
*/

void app_main(void)
{
    {
    gpio_pad_select_gpio(CONFIG_BUTTON_PIN);
    gpio_pad_select_gpio(CONFIG_LED_PIN);
    
    gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_LED_PIN,    GPIO_MODE_OUTPUT);
    
    gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);
    
    gpio_install_isr_service(ESP_INR_FLAG_DEFAULT);
    gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);
    
    xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR);
    }
    
/*
    if ( DefaultBusInit( ) == true ) {
        printf( "BUS Init lookin good...\n" );
        printf( "Drawing.\n" );

        #if defined USE_I2C_DISPLAY
            SetupDemo( &I2CDisplay, &Font_droid_sans_fallback_24x28 );
            SayHello( &I2CDisplay, "Hello i2c!" );
        #endif

        #if defined USE_SPI_DISPLAY
            SetupDemo( &SPIDisplay, &Font_liberation_mono_17x30 );
            SayHello( &SPIDisplay, "Hi SPI!" );
        #endif

        printf( "Done!\n" );
    }*/
}
