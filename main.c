#include "FreeRTOS.h"
#include "task.h"
#include"queue.h"
#include<stdint.h>
#include"tm4c123gh6pm.h"
#include"functions.h"

void SystemInit(){}
#define SWITCHTASKSTACKSIZE        128
QueueHandle_t queue;

void task1(void* pvParameters)
{
    int temperature;
    while(1)
        {
            temperature = read_temp();
            xQueueSend(queue,(void*) &temperature,pdMS_TO_TICKS(500));
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
}
void task2(void* pvParameters)
{
    int received;
    while(1)
            {

                vTaskDelay(pdMS_TO_TICKS(3000));
                xQueueReceive(queue,&received,pdMS_TO_TICKS(500));
                int tens=(received/10);
                int units=received%10;
                printString("temperature is : ");
                printChar('0'+tens);
                printChar('0'+units);
                printString(" celcius\n\r");
            }
}


int main(void)
{

    portA_init();

    ADC_init();
    queue = xQueueCreate(3,sizeof(int));
    TaskHandle_t  First_handle,Second_handle;


    xTaskCreate(task1, "First",128, NULL,1, &First_handle);
    xTaskCreate(task2, "Second",128, NULL,2, &Second_handle);

    vTaskStartScheduler();
    while(1)
        {


        }
	return 0;
}
