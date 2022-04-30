
/**
 ** UART functions
 **/
#define HW_REG(x) (*((volatile uint32_t *)(x)))

#define UART_PARTITY_NONE   0
#define UART_PARTITY_ODD    1
#define UART_PARTITY_EVEN   2
#define UART_PARTITY_MARK   3
#define UART_PARTITY_SPACE  4

#define GPIO_Port_A 0x40004000
#define GPIO_Port_B 0x40005000
#define GPIO_Port_C 0x40006000
#define GPIO_Port_D 0x40007000
#define GPIO_Port_E 0x40024000
#define GPIO_Port_F 0x40025000

#define UART0_BASE 0x4000C000
#define UART1_BASE 0x4000D000
#define UART2_BASE 0x4000E000
#define UART3_BASE 0x4000F000
#define UART4_BASE 0x40010000
#define UART5_BASE 0x40011000
#define UART6_BASE 0x40012000
#define UART7_BASE 0x40013000

#define UART0_Rx 0
#define UART0_Tx 1
#define UART1_Rx 4
#define UART1_Tx 5
#define UART2_Rx 6
#define UART2_Tx 7
#define UART3_Rx 6
#define UART3_Tx 7
#define UART4_Rx 4
#define UART4_Tx 5
#define UART5_Rx 4
#define UART5_Tx 5
#define UART6_Rx 4
#define UART6_Tx 5
#define UART7_Rx 0
#define UART7_Tx 1


/* OFFSET Values */
#define UART_OFFSET_DR_R        0x000
#define UART_OFFSET_RSR_R       0x004
#define UART_OFFSET_ECR_R       0x004
#define UART_OFFSET_FR_R        0x018
#define UART_OFFSET_ILPR_R      0x020
#define UART_OFFSET_IBRD_R      0x024
#define UART_OFFSET_FBRD_R      0x028
#define UART_OFFSET_LCRH_R      0x02C
#define UART_OFFSET_CTL_R       0x030
#define UART_OFFSET_IFLS_R      0x034
#define UART_OFFSET_IM_R        0x038
#define UART_OFFSET_RIS_R       0x03C
#define UART_OFFSET_MIS_R       0x040
#define UART_OFFSET_ICR_R       0x044
#define UART_OFFSET_DMACTL_R    0x048
#define UART_OFFSET_9BITADDR_R  0x0A4
#define UART_OFFSET_9BITAMASK_R 0x0A8
#define UART_OFFSET_PP_R        0xFC0
#define UART_OFFSET_CC_R        0xFC8

/* OFFSET Values */
#define GPIO_PORT_OFFSET_DATA_R  0x000
#define GPIO_PORT_OFFSET_DIR_R   0x400
#define GPIO_PORT_OFFSET_IS_R    0x404
#define GPIO_PORT_OFFSET_IBE_R   0x408
#define GPIO_PORT_OFFSET_IEV_R   0x40C
#define GPIO_PORT_OFFSET_IM_R    0x410
#define GPIO_PORT_OFFSET_RIS_R   0x414
#define GPIO_PORT_OFFSET_MIS_R   0x418
#define GPIO_PORT_OFFSET_ICR_R   0x41C
#define GPIO_PORT_OFFSET_AFSEL_R 0x420
#define GPIO_PORT_OFFSET_DR2R_R  0x500
#define GPIO_PORT_OFFSET_DR4R_R  0x504
#define GPIO_PORT_OFFSET_DR8R_R  0x508
#define GPIO_PORT_OFFSET_ODR_R   0x50C
#define GPIO_PORT_OFFSET_PUR_R   0x510
#define GPIO_PORT_OFFSET_PDR_R   0x514
#define GPIO_PORT_OFFSET_SLR_R   0x518
#define GPIO_PORT_OFFSET_DEN_R   0x51C
#define GPIO_PORT_OFFSET_LOCK_R  0x520
#define GPIO_PORT_OFFSET_CR_R    0x524
#define GPIO_PORT_OFFSET_AMSEL_R 0x528
#define GPIO_PORT_OFFSET_PCTL_R  0x52C
    
/* 
 * initialize UART based on the port number, baudrate and parity type
 * 
 * returns 0 if no error and opened properly else error no indicating error type
 * */
int UART_Init(
    int iPort,          /* UART port number as per chip */
    int iBaudRate,      /* baudrate not all may be supported */
    int iParity_NOEMS); /* parity enable 0- none, 1-odd, 2-even, 3-mark, 4-space */

/* 
 * UART write - send chars through UART using buffer and count passed as parameters. If the FIFO is 
 * full return back with number of bytes that was successfully added to FIFO (Non blocking)
 * 
 * returns number of bytes actually transferred 
 * */

int UART_Write(
    int iPort,						/* port identifier */
    int iBytes,						/* max number of bytes that should be written */
    unsigned char *pcBytes);		/* buffer to be used to write into FIFO */

/*
 * UART Read a character from UART FIFO (non blocking). IF fifo does not have sufficient bytes then function 
 * reads available bytes and returns the count of bytes that was read
 * 
 * return number of bytes actually read
 * */

int UART_Read(
    int iPort,						/* port identifier */
    int iBytes,						/* max number of bytes that should be read */
    unsigned char *pcBytes);		/* buffer to be used to read into FIFO */
	
/*
 * UART BytesInRx - returns number of bytes available in RX fifo that can be read
 * 
 * return number of bytes in the RX fifo
 * */

int UART_BytesInRx(
    int iPort);					/* port identifier */
