#include "FreeRTOS.h"
#include "task.h"
#include"queue.h"
#include<stdint.h>
#include"tm4c123gh6pm.h"
#include"functions.h"
#include"string.h"
#include <stdbool.h>
#include<stdio.h>

#include"driverlib/eeprom.h"
#include"driverlib/sysctl.h"

void SystemInit(){}
#define SWITCHTASKSTACKSIZE        128
QueueHandle_t queue;
TaskHandle_t  First_handle,Second_handle,password,rst_pass;
uint32_t de_pass;
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

void pass_task(void * pvParameters)
{
  //  printString("working\r\n");

        char read[10];
        while(1)
        {
            uint32_t entered_pass;
            int index=0;
            printString("enter password: ");
            char c = readChar();
            while(c!=13)
                    {
                        printChar(c);
                        read[index] = c;
                        index++;
                        c = readChar();
                    }
                    read[index]='\0';
                    printString("\n\r");
                    sscanf(read, "%d", &entered_pass);

                    if(entered_pass==de_pass)
                    {
                        vTaskDelete(NULL);
                    }
        }

}
void rst_task(void* pvParameters)
{
    char read[10];
            while(1)
            {
                vTaskSuspend(rst_pass);
                uint32_t entered_pass;
                int index=0;
                printString("enter new password: ");
                char c = readChar();
                while(c!=13)
                        {
                            printChar(c);
                            read[index] = c;
                            index++;
                            c = readChar();
                        }
                        read[index]='\0';
                        printString("\n\r");
                        sscanf(read, "%d", &entered_pass);
                        EEPROMProgram(&entered_pass,1000,sizeof(uint32_t));
            }
}
void GPIOF_Handler(void)
{
    GPIO_PORTF_ICR_R |=(1<<0);
    xTaskResumeFromISR(rst_pass);
}

int main(void)
{
    portF_init();
    portA_init();
    ADC_init();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    uint32_t ret = EEPROMInit();
  /*  if(ret == EEPROM_INIT_OK)
        printString("OK \r\n");
    else if(ret == EEPROM_INIT_ERROR)
        printString("not OK \r\n");
        */

    //x is  written to the address of 1000
    uint32_t y;
    EEPROMRead(&y,1000,sizeof(uint32_t));
    // de_pass in location 800
    EEPROMRead(&de_pass,1000,sizeof(uint32_t));
   // printChar((char)(y+'0'));
    //printString("\r\n");
    queue = xQueueCreate(3,sizeof(int));


    xTaskCreate(pass_task,"password",500,NULL,3,&password);
    xTaskCreate(task1, "First",128, NULL,1, &First_handle);
    xTaskCreate(task2, "Second",128, NULL,2, &Second_handle);
    xTaskCreate(rst_task,"rst_password",500,NULL,3,&rst_pass);
    vTaskStartScheduler();
    while(1)
        {


        }
	return 0;
}
