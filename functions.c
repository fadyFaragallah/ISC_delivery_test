#include <stdint.h>
#include"functions.h"
#include"tm4c123gh6pm.h"
void portF_init(void)
    {
        int volatile delay;
        SYSCTL_RCGCGPIO_R |=0x20;  //enable clock for port f
        delay=0; // delay to allow port f to start as it needs 2 clock cycles
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
        GPIO_PORTF_CR_R = 0x1F;
        GPIO_PORTF_AMSEL_R = 0x00;
        GPIO_PORTF_PCTL_R = 0x00000000;
        GPIO_PORTF_DIR_R = 0x0E;
        GPIO_PORTF_AFSEL_R = 0x00000000;
        GPIO_PORTF_PUR_R = 0x11;
        GPIO_PORTF_DEN_R = 0x1F;
    }
void systick_init(void)
{
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 0x00FFFFFF;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x05;
}

void portA_init(void)
{
    SYSCTL_RCGCGPIO_R |=1<<0;
    SYSCTL_RCGCUART_R |=1<<0;    //enabling uart at port A
    GPIO_PORTA_AFSEL_R=0x03;
    GPIO_PORTA_PCTL_R |=(1<<0) | (1<<4);
    GPIO_PORTA_DEN_R |=(1<<0) | (1<<1);
    //setting uart function on port A
    UART0_CTL_R=~1;
    // baud rate = 9600 so integet part = (16*10^6)/(16*9600)  and floating part = (remainder*64)+0.5
    UART0_IBRD_R=104;
    UART0_FBRD_R=11;
    UART0_LCRH_R =(1<<5) | (1<<6);
    UART0_CTL_R =(1<<0) | (1<<8) | (1<<9);
    UART0_CC_R=0x00;
}

// reads 1 byte from the uart transmitter
char readChar(void)
{
    while((UART0_FR_R & 0x0010) !=0){}
    return (char)(UART0_DR_R & 0xFF);
}

// sends 1 byte through the uart receiver
void printChar(char c)
{
    while((UART0_FR_R & 0x0020)!=0){}
    UART0_DR_R = c;
}

//sends a sequence of characters using printChar function
void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}
void ADC_init(void)
{
    int volatile delay;
    SYSCTL_RCGCADC_R |=0x1;
    delay =2;
    delay =1;
    delay =0; //to make delay to allow adc to be initialized
    ADC0_ACTSS_R &=~0x8;
    ADC0_EMUX_R &= ~(0xF000);
    ADC0_SSMUX3_R = 0;
    ADC0_SSCTL3_R |= 0xE;
    ADC0_ACTSS_R |=0x8;
}

int read_temp(void)
{
    int result;
    ADC0_PSSI_R |= 0x8;
    while((ADC0_RIS_R&0x8)==0);
    result=(147.5-(247.5*ADC0_SSFIFO3_R)/4096.0);
    ADC0_ISC_R = 0x8;
    return result;
}

