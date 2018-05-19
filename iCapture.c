#include "Board_accelerometer.h"

#include <board.h>
#include "utils.h"
#include "fsl_os_abstraction.h"
#include "fsl_dspi_master_driver.h"
#include "fsl_clock_manager.h"
#include "matrixControl.h"


void put(char *ptr_str);
void uart_putchar(char ch);
uint8_t uart_getchar ();

int displayState = 0; // global variable to keep track of which state the display is on

int switchPressed = 0; // global variable to keep track of whether the switch has been pressed 

int main(){
	
	LED_Initialize();  //on board LED used for debug purposes
	initStructs(); //initalize the value in the structs

	//hardware_init(); //<--- I removed the uart for putty
	CLOCK_SYS_EnablePortClock(PORTE_IDX);
	CLOCK_SYS_EnablePortClock(PORTA_IDX);
  CLOCK_SYS_EnablePortClock(PORTB_IDX);
	CLOCK_SYS_EnablePortClock(PORTC_IDX);
	
	// initialize the SW2 button as an interrupt
	PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK; // 
	PORTC->PCR[6] |= PORT_PCR_IRQC(1001) | PORT_PCR_MUX(1);
	PTC -> PDDR &= ~GPIO_PDDR_PDD(1<<4);  
	NVIC_EnableIRQ(PORTC_IRQn); 
	PORTC->PCR[6] |= (1<<9);
	PORTC->ISFR = (1 << 6); // clear flag
	
	//I2C Setup
	Accelerometer_Initialize();
	
	//LED Matrix setup
	initBoard();
	clearBoard();
	
	//Enable UART
	SIM_SCGC4 |= SIM_SCGC4_UART3_MASK; /*Enable the UART clock*/
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable the PORTC clock*/
	PORTC_PCR16 |= PORT_PCR_MUX(3);
	PORTC_PCR17 |= PORT_PCR_MUX(3);
	UART3_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK ); /*Disable Tx and Rx*/
	UART3_C1 = 0; /*Default settings of the register*/
	uint16_t ubd = (uint16_t)((21000*1000)/(115200 * 16)); /*<-- BAUD RATE CHANGED Calculate baud settings */
	uint8_t temp = UART3_BDH & ~(UART_BDH_SBR(0x1F)); /*Save the value of UART0_BDH except SBR*/
	UART3_BDH = temp | (((ubd & 0x1F00) >> 8));
	UART3_BDL = (uint8_t)(ubd & UART_BDL_SBR_MASK);
	UART3_C2 |=(UART_C2_TE_MASK |UART_C2_RE_MASK); /* Enable receiver and transmitter*/
	
	while(1){
		
		
		// if the mode has been changed, clear the board
		if(switchPressed == 1) {
			clearBoard();
			switchPressed = 0;
		}
		
		uint8_t test = 0;
		test = uart_getchar();
		
		if(displayState){
			// displayState = 1, i.e. mostRecentAnswer state
			if(test == 65) {
				writeA();
			}			
			else if(test == 66) writeB();
			else if(test == 67) writeC();
			else if(test == 68) writeD();
			else if(test == 69) {
				writeE();
			}
		}
		else{
			// displayState = 0, i.e. barGraph state
			if(test == 65) {
				ABarPlus();
				ABarIncrement();
			}		
			else if(test == 66) 
			{
				BBarPlus();
				BBarIncrement();
			}
			else if(test == 67) 
			{
				CBarPlus();
				CBarIncrement();
			}
			else if(test == 68){ 
				DBarPlus();
				DBarIncrement();
			}
			else if(test == 69) {
				EBarPlus();
				EBarIncrement();
			}
		}
	}
	
	return 0;
}

void put(char *ptr_str)
{
	while(*ptr_str)
	uart_putchar(*ptr_str++);
}

void uart_putchar (char ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART3_S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART3_D = (uint8_t)ch;
}

uint8_t uart_getchar ()
{
/* Wait until character has been received */
while (!(UART3_S1 & UART_S1_RDRF_MASK) && !switchPressed);
/* Return the 8-bit data from the receiver */
return UART3_D;
}


void PORTC_IRQHandler(void)
{
		/*
	This is the IRQ handler for SW2. It simply
	toggles the display state between aggregation mode
	and most recent answer mode.
	*/
	
	if(displayState == 0) {
		
		displayState = 1;
	}
	else {
		displayState = 0;
	}
	
	switchPressed = 1;
	
	PORTC->ISFR = (1 << 6); 
	NVIC_ClearPendingIRQ(PORTC_IRQn); // clear interrupt flag
	
}
