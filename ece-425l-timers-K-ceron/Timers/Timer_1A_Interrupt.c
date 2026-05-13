#include "Timer_1A_Interrupt.h"

void(*Timer_1A_Task)(void);

void Timer_1A_Interrupt_Init(void(*task)(void))
{
//Store the user-defined task function for use during interrupt handlin
	Timer_1A_Task = task;
	
	// Set the R0 bit (Bit 1) in the RCGCTIMER register to enable the clock for Timer 1A
	SYSCTL -> RCGCTIMER |= 0x02;
	
	//Clear the TAEN bit (Bit 1) of the GPTMCTL reister to disable Timer 1A
	TIMER1 ->CTL	&=	~0x01;
	
	//Set the bits of the GPTMCFG field (Bits 2 to 0) in the GPTMCFG register
	// 0x4 = Select the 16 bit timer configuration
	TIMER1 -> CFG |= 0x04;
	
	//Set te bits of the TAMR field (Bits 1 to 0 ) in the GPTMTAMR register
	// 0x2 = periodic Timer Mode
	TIMER1 -> TAMR |= 0x02 ;
	
	// Clear the bits of the TAPSR field ( Bits 7 to 0) in the
	// GPTMTAPR register before setting the prescale value
	TIMER1 -> TAPR &= ~0x000000FF;
	
	//Set the prescale value to 50 by setting hte bits of the
	//TAPSR field (Bits 7 to 0) in the GPTMTAPR register
	// New timer clock frequency = (50 Mhz/ 50) = 1 Mhz
	TIMER1 -> TAPR = 50;
	
	//Set the timer interval load value by writing to the
	//TAILR field (Bits 31 to 0) in the GPTMTAILR register
	//Timer 0A Resolution: = (1us *1000) = 1ms
	TIMER1 ->TAILR = (1000 - 1);
	
	//Set the TATOCINT bit (Bit 0) to 1 in the GPTMICR register
	//The TATOCINT bit will be automatically clear when it is set to 1
	TIMER1 -> ICR |= 0x01;
	
	//Enable the Timer 1A interrupt by setting the TATOIM bit (Bit 0) in the GPTMIMR register
	TIMER1 ->IMR |= 0x01;
	
	//Set the priority level to 2 for the Timer 1A interrupt
	// In the Interrupt 23-21 Priority (PRI4) register,
	//the INTD field (Bit 31 to 29) corresponds to the Interrupt Request (IQR) 21
	//Timer 1A has an IRQ of 19
	NVIC ->IPR[5] = (NVIC ->IPR[5] & 0x00FFFFFF) | ( 2<<29);
	
	//Enable IRQ 19 for Timer 1A by setting Bit 19 in the ISER[0] register
	NVIC ->ISER[0]	|= (1<<21);

	//Set the TAEN bit (Bit 0) in the GPTMCTL register to enable Timer 0A
	TIMER1 ->CTL |=0x01;
}

void TIMER1A_Handler(void)
{
//Read the Timer 1A time-out interrupt flag
	if (TIMER1 ->MIS & 0x01)
	{
		//Execute the user-defined function
		(*Timer_1A_Task)();
		
		//Acknowledge the Timer 1A interrupt and clear it
		TIMER1 ->ICR |= 0x01;
	}
}