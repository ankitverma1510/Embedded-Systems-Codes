#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <./inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <string.h>

void enable_irq(void);
void systick_init(void);
void write_uart(char *);
void uart0_init();
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void GPIOPortF_Init(void);
void GPIOPortF_Handler(void);
void UART_IntHandler();
void LCD_Init();
void LCD_print(char *);
void LCD_write(unsigned char data, unsigned int control);
void delayMs(int n);
void delayUs(int n);
void change_cycle(int);
void pwm_init(void);
void display_led(int);
void timer_init();
void T0SA_ISR();
void keypad_decode(int , int);


#define clear_display 0x01
#define returnHome 0x02
#define moveCursorRight 0x06
#define moveCursorLeft 0x08
#define shiftDisplayRight 0x1C
#define shiftDisplayLeft 0x18
#define cursorBlink 0x0F
#define cursorOff 0x0C
#define cursorOn 0x0E
#define Function_set_4bit 0x28
#define Function_set_8bit 0x38
#define Entry_mode 0x06
#define Function_8_bit 0x32
#define Set5x7FontSize 0x20
#define FirstRow 0x80


char message[20];
char val[20]="Welcome",*token;
int  dt=0, door=0, status=0, cycle=0, timer, timer_value=30, abrt=0, complete=0;
int digit[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int i=0,a,b,c,d;
int p=0;
int sw=0;
int color[]={0x02,0x04,0x08,0x0E};
int count=1;
int time_cycle[]={15,30,45};
int LED_color=0, LED_blink_state=0 ;
int lcd_printing = 0;

unsigned char key[4][4]={{'1','2','3','4'},
                         {'5','6','7','8'},
                         {'9','A','B','C'},
                         {'D','E','F','G'}
};

int main()
{   // Initializing GPIOs //

    SYSCTL_RCGC2_R |= 0x23;


    SYSCTL_RCGC2_R |= 0x0000003F;
        GPIO_PORTC_DIR_R = 0x00;
        GPIO_PORTC_DEN_R = 0xF0; // enable the GPIO pins for digital function
        GPIO_PORTE_DIR_R = 0x0F; // enable the GPIO pins for the LED as output and remaining as input
        GPIO_PORTE_DEN_R = 0x0F; // enable the GPIO pins for digital function
        GPIO_PORTB_DIR_R = 0xFF;
        GPIO_PORTB_DEN_R = 0xFF; // enable the GPIO pins for digital function

        GPIO_PORTE_ODR_R = 0x0F;
        GPIO_PORTC_PUR_R = 0xF0;


    // Initialization //
    uart0_init();
    write_uart(val);
    GPIOPortF_Init();
    LCD_Init();
    systick_init();
    //LCD_write(clear_display,0);
    strcpy(message,"Systick Init");
    //LCD_print(message);
    delayMs(500);
    strcpy(message,"Welcome");
    LCD_print(message);
    LCD_print("Timer 15/30/45");

    timer_init();
    pwm_init();
    timer=timer_value;

    while(1)
    {
        GPIO_PORTE_DATA_R = 0x00;
        if ((GPIO_PORTC_DATA_R & 0xF0) != 0xF0)
        {
            for(int i=0;i<4;i++)
            {
                GPIO_PORTE_DATA_R = (~(0x01<<i));
                for (int j=0;j<4;j++)
                {
                    if((GPIO_PORTC_DATA_R & (0x10<<j)) == 0)
                        {
                        UARTCharPut(UART0_BASE, key[i][j]);
                        keypad_decode(i,j);
                        break;
                        }
                }

            }
            delayMs(100);
        }


      if(abrt == 1)
      {
          change_cycle(0);
          status = 0;
      }
        else if(status ==1)
        {
            if(timer_value > 0.7*timer)
                change_cycle(1);
            else if (timer_value > 0.2*timer)
                change_cycle(2);
            else if(timer_value > 0)
                change_cycle(3);
            else if (timer_value==0 && abrt ==0)
            {
                change_cycle(0);
                timer_value =-1;
                status=0;
                complete=1;
                strcpy(message,"Completed");
                GPIO_PORTF_DATA_R = 0x0E;
                LCD_print(message);
            }
        }
        else if(status==0)
            change_cycle(0);
     }

return 0;
}

void timer_init()
{
    //for timer 0 A,
    NVIC_PRI4_R = (NVIC_PRI4_R & 0x1FFFFFFF) | 0xE0000000; // priority 7 (not sure why this value)
    NVIC_EN0_R = 0x00080000; // Enable interrupt 19 in NVIC (IRQ number is 19 so it will be 19th bit of NVIC_EN0_R)
    //bit number = IRQ number - 32(n-1)
    SYSCTL_RCGCTIMER_R |= 1; // enable clock to Timer Block 0
    TIMER0_CTL_R = 0; // disable Timer before initialization
    TIMER0_CFG_R = 0x00; // 32-bit option (configuration)
    //TIMER0_CFG_R = 0x04; // 16-bit option (configuration)
    TIMER0_TAMR_R = 0x02; // periodic mode and down-counter
    TIMER0_TAILR_R = 320000; // Timer A interval load value register // 20 ms delay
    TIMER0_IMR_R = 0x01; //Interrupt enabled for timeout of timer A
    TIMER0_ICR_R = 0x1; // clear the TimerA timeout flag
    TIMER0_CTL_R |= 0x01; // enable Timer A after initialization
    __asm (" CPSIE I\n"); //Enable interrupts
}


void keypad_decode(int i, int j)
{
    int p =(i+1)*(j+1);
    if(abrt ==0)
    {
        switch(p)
        {
        case 1:
            if (sw>2)
                sw=0;
            timer_value = time_cycle[sw++];
            timer=timer_value;
            break;
        case 2:
            if (door == 0 && status == 0)
            {
                door = 1;
                LCD_print("Door Closed");
            }
            else if (door == 1 && status ==0)
            {
                door = 0;
                LCD_print("Door Opened");
            }
            else LCD_print("M/C is on");
            break;
        case 3:
            if (door==0)
                if (status==0)
                    {
                        dt=1;
                        LCD_print("Dtrgnt Filled");
                    }
                else
                    LCD_print("M/C is ON");
            else
                LCD_print("Door Close");
            break;
        case 4:
            abrt = 1;
            timer_value=0;
            LCD_print("Abort Pressed");
            break;
        case 16:
            if(!complete)
            {
            if(door==1)
                if(dt==1)
                {
                    status = 1;
                    LCD_print("Running");
                }
                else
                    LCD_print("Fill Dtrgnt");
            else
                LCD_print("Door open");
            break;
            }
        }
    }
    else
        LCD_print("Aborted");
}

void T0SA_ISR()
{
TIMER0_ICR_R = 0x1;
display_led(timer_value);
}

void display_led(int value)
{
    a=value/1000;
    int temp=value%1000;
    b=temp/100;
    temp=temp%100;
    c=temp/10;
    d=temp%10;
    GPIO_PORTE_DATA_R |= 0x01<5;
    //for (int j=0; j<150;j++)
    {
    GPIO_PORTA_DATA_R=0x10;
    GPIO_PORTB_DATA_R=digit[d];
    delayMs(4);
    GPIO_PORTA_DATA_R=0x20;
    GPIO_PORTB_DATA_R=digit[c];
    delayMs(1);
    /*GPIO_PORTA_DATA_R=0x40;
    GPIO_PORTB_DATA_R=digit[b];
    delayMs(1);
    GPIO_PORTA_DATA_R=0x80;
    GPIO_PORTB_DATA_R=digit[a];
    delayMs(1);*/
    }
}

void systick_init(void)
{
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CURRENT_R = 0;
    /* Configure SysTick */
    NVIC_ST_RELOAD_R = 16000000-1;  /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
    enable_irq();                   /* global enable interrupt */

}


void pwm_init(void)
{
    /* Enable Peripheral Clocks */
    SYSCTL_RCGCPWM_R |= 1;       /* enable clock to PWM */
    SYSCTL_RCGCGPIO_R |= 0x38;   /* enable clock to PORTE */
    SYSCTL_RCC_R &= ~0x00100000; /* no pre-divide for PWM clock */
    /* Enable port PE4 for PWM  */
    GPIO_PORTE_AFSEL_R |= 0x10;      /* PE4 uses alternate function */
    GPIO_PORTE_PCTL_R &= ~0x000F0000; /* make PE4 PWM output pin */
    GPIO_PORTE_PCTL_R |= 0x00040000;
    GPIO_PORTE_DEN_R |= (1<<4);       /* pin digital */
    GPIO_PORTE_DIR_R |= (1<<4);
    PWM0_2_CTL_R = 0;            /* stop counter */
    PWM0_2_GENA_R = 0x0000008C;
                                 /* clear when match PWMCMPA */
    PWM0_2_LOAD_R = 16000;       /* set load value for 1kHz (16MHz/16000) */
    PWM0_2_CMPA_R = 15999;       /* set duty cycle to min */
    PWM0_2_CTL_R = 1;            /* start timer */
    PWM0_ENABLE_R = (1<<4);        /* start PWM0 ch4 */
}

void change_cycle(int a)
{
    switch(a)
    {
    case 0:
        PWM0_2_CMPA_R = 15999;
        break;
    case 1:
        PWM0_2_CMPA_R = 10999;
        break;
    case 2:
        PWM0_2_CMPA_R = 5999;
        break;
    case 3:
        PWM0_2_CMPA_R = 0;
        break;
    }

}

void LED_blink(int LED_num)
{

    GPIO_PORTF_DATA_R = color[LED_color];
    delayMs(500);
    GPIO_PORTF_DATA_R = 0;
    delayMs(500);
}


void uart0_init()
{
    // UART 0 Initialization //
       SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

       GPIOPinConfigure(GPIO_PA0_U0RX);
       GPIOPinConfigure(GPIO_PA1_U0TX);
       GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

       UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
       UART0_IM_R|=0x10;
       UART0_LCRH_R &=~0x10; //FIFO Disabled
       NVIC_PRI1_R |= (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000; /*  priority 5 */
       NVIC_EN0_R |= 0x00000020;        /*  Enable interrupt 5 in NVIC */


}

void write_uart(char *str)
{
    UARTCharPut( UART0_BASE, 10 );  //entering return character
    UARTCharPut( UART0_BASE, 13 );  // for carriage return
    int len = strlen(str);
    for (int i=0; i<len; i++)
    {

        UARTCharPut( UART0_BASE, str[i] );
    }
    UARTCharPut( UART0_BASE, 10 );  //entering return character
    UARTCharPut( UART0_BASE, 13 );  // for carriage return
}

void GPIOPortF_Init(void)
{

    SYSCTL_RCGC2_R |= 0x00000020;   /* 1) activate clock for Port F */

    GPIO_PORTF_LOCK_R = 0x4C4F434B; /* 2) unlock GPIO Port F */
    GPIO_PORTF_CR_R = 0x1F;         /* allow changes to PF4-0 */
    GPIO_PORTF_AMSEL_R = 0x00;      /* 3) disable analog on PF */
    GPIO_PORTF_PCTL_R = 0x00000000; /* 4) PCTL GPIO on PF4-0 */
    GPIO_PORTF_DIR_R = 0x0E;        /* 5) PF4,PF0 in, PF3-1 out */
    GPIO_PORTF_AFSEL_R = 0x00;      /* 6) disable alt funct on PF7-0 */
    GPIO_PORTF_PUR_R = 0x11;        /* enable pull-up on PF0 and PF4 */
    GPIO_PORTF_DEN_R = 0x1F;        /* 7) enable digital I/O on PF4-0 */

    GPIO_PORTF_IS_R |= 0x11;       /*  PF4 PF0 is level-sensitive */
    GPIO_PORTF_IBE_R &= ~0x11;      /*  PF4 PF0 is not both edges */
    GPIO_PORTF_IEV_R &= 0x11;      /*  PF4 PF0 level event */
    GPIO_PORTF_ICR_R = 0x11;        /*  Clear flag4 flag0 */
    GPIO_PORTF_IM_R |= 0x11;        /*  arm interrupt on PF4 PF0*/
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00A00000; /*  priority 5 */
    NVIC_EN0_R = 0x40000000;        /*  Enable interrupt 30 in NVIC */

    EnableInterrupts();             /* Enable global Interrupt flag (I) */
}


void GPIOPortF_Handler(void)
{
    volatile int readback;
    int abort_sw = GPIO_PORTF_MIS_R & 0x10; //sw1 for abort
    int pause_sw = GPIO_PORTF_MIS_R & 0x01; //sw2 for pause or resume

    GPIO_PORTF_ICR_R = 0x11;        /* clear PF4 PF0 int */

    if (abort_sw==16)
    {

        strcpy(val,"abort_pressed");
        write_uart(val);
        abrt=1;
        status=0;
        timer_value=0;
        LCD_print(val);
        delayMs(100);
    }
    else if (pause_sw==1  && status == 0)
    {
        strcpy(val,"status_changed");
        write_uart(val);
        if (abrt==1)
            strcpy(message,"Aborted");
        else
            if(dt == 1)
            {
                if(door == 1)
                {
                    status = 1;
                    strcpy(message,"Running");
                }
                else
                    strcpy(message,"Door Open");
            }
            else
                strcpy(message,"Fill Dtgnt");
            LCD_print(message);
            delayMs(100);
    }
    else if(pause_sw==1 && status == 1)
    {
        status = 0;
        strcpy(message,"Pause");
        LCD_print(message);
        delayMs(100);
    }
    readback = GPIO_PORTF_ICR_R;    /* a read to force clearing of interrupt flag */
    readback = readback;            /* suppress compiler warning "unused variable" */
}


void UART_IntHandler()
{
    UART0_ICR_R |=0x10;
    val[p] = UARTCharGet(UART0_BASE);
    UARTCharPut(UART0_BASE, val[p]);
               if (val[p] == 13) // checking whether enter button is pressed
               {
                   val[p]='\0';
                   UARTCharPut( UART0_BASE, 10 );  //entering return character
                   UARTCharPut( UART0_BASE, 13 );  // for carriage return
                   UARTCharPut( UART0_BASE, 'E' );
                   UARTCharPut( UART0_BASE, 'n' );
                   UARTCharPut( UART0_BASE, 't' );
                   UARTCharPut( UART0_BASE, 'e' );
                   UARTCharPut( UART0_BASE, 'r' );
                   UARTCharPut( UART0_BASE, 'e' );
                   UARTCharPut( UART0_BASE, 'd' );
                   UARTCharPut( UART0_BASE, 10 );
                   UARTCharPut( UART0_BASE, 13 );
                   p=-1;

                   token = strtok(val, " ");
                   if (!strcmp(token, "set"))
                   {
                       token = strtok(NULL, " ");
                       if (!strcmp(token, "timer"))
                       {
                           token = strtok(NULL, " ");
                           int value = atoi(token);
                           timer_value = value;
                           timer = timer_value;
                           LCD_print("Timer Changed");
                       }
                   }
                   else if (!strcmp(token, "door"))
                   {
                       token = strtok(NULL, " ");
                       if (!strcmp(token, "open"))
                           {
                           if (status==0)
                           {
                               door = 0;
                               strcpy(message,"Door Opened");
                           }
                           else
                           {
                               strcpy(message,"m/c on");
                           }
                           LCD_print(message);
                           }
                       if (!strcmp(token, "close"))
                           {
                           door = 1;
                           strcpy(message,"Door Closed");
                           LCD_print(message);
                           }
                   }
                   else if (!strcmp(token, "detergent"))
                   {
                       token = strtok(NULL, " ");
                       if (!strcmp(token, "fill"))
                       {
                           if (abrt==1)
                           strcpy(message,"Aborted");
                           else if(door==0)
                           {
                               if (status == 0)
                                   {
                                   dt = 1;
                                   strcpy(message,"Dtrgnt Filled");
                                   }
                               else
                                   strcpy(message,"M/C is ON");
                           }
                           else
                               strcpy(message,"Door Close");
                           LCD_print(message);
                       }
                   }
                   else if (!strcmp(token, "abort"))
                   {
                       abrt = 1;
                       status = 0;
                       timer_value = 0;
                       strcpy(message,"Abort");
                       LCD_print(message);
                   }
                   else if (!strcmp(token, "pause"))
                   {
                       if (abrt==1)
                           strcpy(message,"Aborted");
                       else
                           {
                           status = 0;
                           strcpy(message,"Pause");
                           }
                       LCD_print(message);
                   }
                   else if (!strcmp(token, "resume"))
                   {
                       if (abrt==1)
                           strcpy(message,"Aborted");
                       else if(dt == 1)
                           if(door == 1)
                           {
                               status = 1;
                               strcpy(message,"Running");
                           }
                           else
                               strcpy(message,"Door Open");
                       else
                           strcpy(message,"Fill Dtgnt");
                       LCD_print(message);
                   }
               }

               p++;

}


void delayMs(int n)
{
        int i, j;
        for(i = 0 ; i < n; i++)
            for(j = 0; j < 3180; j++) {}   /* do nothing for 1 ms */
}

void LCD_write(unsigned char data, unsigned int control)
{
    lcd_printing =1;
    GPIO_PORTE_DATA_R &= ~0x01<5;
    GPIO_PORTA_DATA_R = 0x00;
    GPIO_PORTA_DATA_R |= (control << 6);
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R |= (0x01 << 7);
    GPIO_PORTA_DATA_R = 0x00 ;
    if ((data < 4) & (control == 0))
        delayMs(2);
    else
        delayUs(40);
    lcd_printing =0;
}

void LCD_Init()
{
    SYSCTL_RCGCGPIO_R |= 0x03;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0xFF;

    // LCD CONTROL PIN INIT
    SYSCTL_RCGCGPIO_R |= 0x10;
    GPIO_PORTE_DEN_R |= 0xFF;
    GPIO_PORTE_DIR_R |= 0xFF;

    // initialize PA7,6 as output
    GPIO_PORTA_DEN_R |= 0xF0;
    GPIO_PORTA_DIR_R |= 0xF0;
    LCD_write(0x38,0);  /* select 5x7 font size and 2 rows of LCD */
    LCD_write(0x06,0);
    LCD_write(0x0F,0);
    LCD_write(0x01,0);

    LCD_write(clear_display,0);
    LCD_write(FirstRow,0);
    strcpy(val,"START");
    LCD_print(val);
    delayMs(500);
}

void LCD_print(char *str)
{   GPIO_PORTE_DATA_R &= ~0x01<5;
    LCD_write(clear_display,0);
    int len =strlen(str);
           for( i=0;i<len;i++)
           {
               LCD_write(str[i],1);
               delayMs(1);
           }

}

void delayUs(int n)
    {
        int i, j;
        for(i = 0 ; i < n; i++)
            for(j = 0; j < 3; j++) {}   // do nothing for 1 us
    }

void SysTick_Handler(void)
{
    if (!abrt)
    {
        if (status==1)
        {
            GPIO_PORTF_DATA_R ^= 0x08;

            if (count ==1)
            {
                count =0;

                if(timer_value >0)
                    timer_value=timer_value-1;

            }
            count=count+1;
        }
    }
    else
        GPIO_PORTF_DATA_R = 0x02;
}

/* global enable interrupts */
void inline enable_irq(void)
{
    __asm  ("    CPSIE  I\n");
}

void DisableInterrupts(void)
{
    __asm ("    CPSID  I\n");
}

void EnableInterrupts(void)
{
    __asm  ("    CPSIE  I\n");
}

void WaitForInterrupt(void)
{
    __asm  ("    WFI\n");
}
