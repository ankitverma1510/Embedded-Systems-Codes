#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>


int UART_Init(int, int, int);

unsigned char Rx;

int UART_Read(int, int, unsigned char *);
int UART_Write(int,int, unsigned char *);
void NOEMS(int, int);

void delayMs(int n);
int main(void){
    UART_Init(0,115200,0);
    delayMs(100);           /* wait for output line to stabilize */


    while(1){
        UART_Read(0,1,&Rx);
        UART_Write(0,1,&Rx);
    }
}


int UART_Init(int iPort, int iBaudRate, int iParity_NOEMS)
{
    SYSCTL_RCGCUART_R = 0x01<<iPort;   //enable clock signal to uart0

    NOEMS(iPort, iParity_NOEMS);

    switch(iPort)
    {
    case 0:


        /* GPIOA configuration */
        SYSCTL_RCGCGPIO_R = 0x01;   //Enable clock signal to GPIOA
        UART0_CTL_R = 0x00;       //disable UARTO
        /* BAUD RATE configuration    */
        UART0_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
        UART0_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
        UART0_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
        UART0_CTL_R = 0x301;       //Enable UART0, TXE, RXE
        GPIO_PORTA_DEN_R = 0x03;          //Enable PORT A1-0 DIGITAL pins
        GPIO_PORTA_AFSEL_R = 0x03;        // Use PA0 and PA1 alternate function
        GPIO_PORTA_PCTL_R = 0x11;         //Configure PA0 and PA1 for UART
        GPIO_PORTA_AMSEL_R = 0x00;        //Disable analog functionality
        break;

    case 1:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x02;   //Enable clock signal to GPIOB
            UART1_CTL_R = 0x00;       //disable UART1
            /* BAUD RATE configuration    */
            UART1_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART1_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART1_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART1_CTL_R = 0x301;       //Enable UART1, TXE, RXE
            GPIO_PORTB_DEN_R = 0x03;          //Enable PORT B1-0 DIGITAL pins
            GPIO_PORTB_AFSEL_R = 0x03;        // Use PB0 and PB1 alternate function
            GPIO_PORTB_PCTL_R = 0x11;         //Configure PB0 and PB1 for UART
            GPIO_PORTB_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 2:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x08;   //Enable clock signal to GPIOD
            UART0_CTL_R = 0x00;       //disable UART2
            /* BAUD RATE configuration    */
            UART2_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART2_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART2_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART2_CTL_R = 0x301;       //Enable UART2, TXE, RXE
            GPIO_PORTD_DEN_R = 0x03<<6;          //Enable PORT D6-7 DIGITAL pins
            GPIO_PORTD_AFSEL_R = 0x03<<6;        // Use PD6 and PD7 alternate function
            GPIO_PORTD_PCTL_R = 0x11;         //Configure PD6 and PD7 for UART
            GPIO_PORTD_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 3:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x04;   //Enable clock signal to GPIOC
            UART3_CTL_R = 0x00;       //disable UARTO
            /* BAUD RATE configuration    */
            UART3_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART3_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART3_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART3_CTL_R = 0x301;       //Enable UART0, TXE, RXE
            GPIO_PORTC_DEN_R = 0x03<<6;          //Enable PORT C6-7 DIGITAL pins
            GPIO_PORTC_AFSEL_R = 0x03<<6;        // Use PC6 and PC7 alternate function
            GPIO_PORTC_PCTL_R = 0x11;         //Configure PC6 and PC7 for UART
            GPIO_PORTC_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 4:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x04;   //Enable clock signal to GPIOC
            UART4_CTL_R = 0x00;       //disable UART4
            /* BAUD RATE configuration    */
            UART4_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART4_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART4_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART4_CTL_R = 0x301;       //Enable UART4, TXE, RXE
            GPIO_PORTC_DEN_R = 0x03<<4;          //Enable PORT C5-4 DIGITAL pins
            GPIO_PORTC_AFSEL_R = 0x03<<4;        // Use PC5 and PC4 alternate function
            GPIO_PORTC_PCTL_R = 0x11;         //Configure PC5 and PC4 for UART
            GPIO_PORTC_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 5:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x10;   //Enable clock signal to GPIOE
            UART5_CTL_R = 0x00;       //disable UART5
            /* BAUD RATE configuration    */
            UART5_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART5_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART5_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART5_CTL_R = 0x301;       //Enable UART5, TXE, RXE
            GPIO_PORTE_DEN_R = (0x03<<4);          //Enable PORT E5-4 DIGITAL pins
            GPIO_PORTE_AFSEL_R = (0x03<<4);        // Use PE5 and PE4 alternate function
            GPIO_PORTE_PCTL_R = 0x11;         //Configure PE5 and PE4 for UART
            GPIO_PORTE_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 6:


            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x08;   //Enable clock signal to GPIOD
            UART6_CTL_R = 0x00;       //disable UART6
            /* BAUD RATE configuration    */
            UART6_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART6_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART6_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART6_CTL_R = 0x301;       //Enable UART6, TXE, RXE
            GPIO_PORTD_DEN_R = (0x03<<4);          //Enable PORT D5-4 DIGITAL pins
            GPIO_PORTD_AFSEL_R = (0x03<<4);        // Use PD5 and PD4 alternate function
            GPIO_PORTD_PCTL_R = 0x11;         //Configure PD5 and PD4 for UART
            GPIO_PORTD_AMSEL_R = 0x00;        //Disable analog functionality
            break;

    case 7:

            /* GPIOA configuration */
            SYSCTL_RCGCGPIO_R = 0x01;   //Enable clock signal to GPIOE
            UART7_CTL_R = 0x00;       //disable UART7
            /* BAUD RATE configuration    */
            UART7_IBRD_R = 1000000/iBaudRate;    // 1MHz/115200 = 8.680
            UART7_FBRD_R = (int)((1000000.0/115200 - 1000000/115200)*64 +0.5);      //Fractional Part = (0.680 * 64) + 0.5 = 44
            UART7_LCRH_R |= 0x60;       //8bit , no parity , 1 stop bit
            UART7_CTL_R = 0x301;       //Enable UART7, TXE, RXE
            GPIO_PORTE_DEN_R = 0x03;          //Enable PORT E1-0 DIGITAL pins
            GPIO_PORTE_AFSEL_R = 0x03;        // Use PE0 and PE1 alternate function
            GPIO_PORTE_PCTL_R = 0x11;         //Configure PE1 and PE0 for UART
            GPIO_PORTE_AMSEL_R = 0x00;        //Disable analog functionality
            break;
    }
        return 0;
}


int UART_Write(int iPort, int iBytes, unsigned char *pcBytes)
{
    switch(iPort)
    {
    case 0:
    while ((UART0_FR_R & 0x20) != 0);
    UART0_DR_R = *pcBytes;
    break;

    case 1:
    while ((UART1_FR_R & 0x20) != 0);
    UART1_DR_R = *pcBytes;
    break;

    case 2:
    while ((UART2_FR_R & 0x20) != 0);
    UART2_DR_R = *pcBytes;
    break;

    case 3:
    while ((UART3_FR_R & 0x20) != 0);
    UART3_DR_R = *pcBytes;
    break;

    case 4:
    while ((UART4_FR_R & 0x20) != 0);
    UART4_DR_R = *pcBytes;
    break;

    case 5:
    while ((UART5_FR_R & 0x20) != 0);
    UART5_DR_R = *pcBytes;
    break;

    case 6:
    while ((UART6_FR_R & 0x20) != 0);
    UART6_DR_R = *pcBytes;
    break;

    case 7:
    while ((UART7_FR_R & 0x20) != 0);
    UART7_DR_R = *pcBytes;
    break;
    }

    return 0;
}
int UART_Read(int iPort, int iBytes, unsigned char *pcBytes)
{
    switch(iPort)
    {

    case 0: while((UART0_FR_R & 0x10) != 0);
    *pcBytes = UART0_DR_R;
    break;

    case 1: while((UART1_FR_R & 0x10) != 0);
    *pcBytes = UART1_DR_R;
    break;

    case 2: while((UART2_FR_R & 0x10) != 0);
    *pcBytes = UART2_DR_R;
    break;

    case 3: while((UART3_FR_R & 0x10) != 0);
    *pcBytes = UART3_DR_R;
    break;

    case 4: while((UART4_FR_R & 0x10) != 0);
    *pcBytes = UART4_DR_R;
    break;

    case 5: while((UART5_FR_R & 0x10) != 0);
    *pcBytes = UART5_DR_R;
    break;

    case 6: while((UART6_FR_R & 0x10) != 0);
    *pcBytes = UART6_DR_R;
    break;

    case 7: while((UART7_FR_R & 0x10) != 0);
    *pcBytes = UART7_DR_R;
    break;
    }
    return 0;
}
// delay n milliseconds (16 MHz CPU clock)
void NOEMS(int port, int noems)
{
    switch(port)
    {
        case 0:
            switch(noems)
            {
                case 0:
                    UART0_LCRH_R = 0;
                break;
                case 1:
                    UART0_LCRH_R = 0x02;
                    break;
                case 2:
                    UART0_LCRH_R = 0x06;
                    break;
                case 3:
                    UART0_LCRH_R = 0x82;
                    break;
                case 4:
                    UART0_LCRH_R = 0x86;
                    break;

            }
            break;

        case 1:
            switch(noems)
            {
                case 0:
                    UART1_LCRH_R = 0;
                break;
                case 1:
                    UART1_LCRH_R = 0x02;
                    break;
                case 2:
                    UART1_LCRH_R = 0x06;
                    break;
                case 3:
                    UART1_LCRH_R = 0x82;
                    break;
                case 4:
                    UART1_LCRH_R = 0x86;
                    break;
            }
            break;


        case 2:
           switch(noems)
           {
                case 0:
                    UART2_LCRH_R = 0;
                    break;
                case 1:
                    UART2_LCRH_R = 0x02;
                    break;
                case 2:
                    UART2_LCRH_R = 0x06;
                    break;
                case 3:
                    UART2_LCRH_R = 0x82;
                    break;
                case 4:
                    UART2_LCRH_R = 0x86;
                    break;
           }
           break;

        case 3:
           switch(noems)
           {
                case 0:
                    UART3_LCRH_R = 0;
                    break;
                case 1:
                    UART3_LCRH_R = 0x02;
                    break;
                case 2:
                    UART3_LCRH_R = 0x06;
                    break;
                case 3:
                    UART3_LCRH_R = 0x82;
                    break;
                case 4:
                    UART3_LCRH_R = 0x86;
                    break;
           }
           break;
        case 4:
           switch(noems)
           {
                case 0:
                    UART4_LCRH_R = 0;
                    break;
                case 1:
                    UART4_LCRH_R = 0x02;
                    break;
                case 2:
                    UART4_LCRH_R = 0x06;
                    break;
                case 3:
                    UART4_LCRH_R = 0x82;
                    break;
                case 4:
                    UART4_LCRH_R = 0x86;
                    break;
           }
           break;

        case 5:
           switch(noems)
           {
                case 0:
                    UART5_LCRH_R = 0;
                    break;
                case 1:
                    UART5_LCRH_R = 0x02;
                    break;
                case 2:
                    UART5_LCRH_R = 0x06;
                    break;
                case 3:
                    UART5_LCRH_R = 0x82;
                    break;
                case 4:
                      UART5_LCRH_R = 0x86;
                      break;
           }
           break;


        case 6:
           switch(noems)
           {
                case 0:
                    UART6_LCRH_R = 0;
                    break;
                case 1:
                    UART6_LCRH_R = 0x02;
                    break;
                case 2:
                    UART6_LCRH_R = 0x06;
                    break;
                case 3:
                    UART6_LCRH_R = 0x82;
                    break;
                case 4:
                    UART6_LCRH_R = 0x86;
                    break;
           }
           break;


        case 7:
           switch(noems)
           {
                case 0:
                    UART7_LCRH_R = 0;
                    break;
                case 1:
                    UART7_LCRH_R = 0x02;
                    break;
                case 2:
                    UART7_LCRH_R = 0x06;
                    break;
                case 3:
                    UART7_LCRH_R = 0x82;
                    break;
                case 4:
                    UART7_LCRH_R = 0x86;
                    break;
           }
           break;
    }


}
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++) {}   // do nothing for 1 ms
}
