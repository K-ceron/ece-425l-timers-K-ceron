/*
 * @file main.c
 *
 * @brief Main source code for the Timers program.
 *
 * This file contains the main entry point and function definitions for the Timers program.
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board Push Buttons (SW2 - SW5)
 *	- EduBase Board Seven-Segment Display
 *
 * @note The following timers will be used to demonstrate periodic interrupts: Timer 0A and Timer 1A.
 *
 * @author Kevin Ceron
 */

#include "TM4C123GH6PM.h"
#include "GPIO.h"
#include "Timer_0A_Interrupt.h"
#include "Timer_1A_Interrupt.h"
#include "SysTick_Delay.h"
#include "Seven_Segment_Display.h"

//Declare the function prototype for the user defined function for Timer 0A
void Timer_0A_Periodic_Task(void);

void Timer_1A_Periodic_Task(void);

//Initializes a static global variable for Timer 0A to keep track of eleapsed time in ms
static uint32_t Timer_0A_ms_elapsed = 0;


static uint32_t Timer_1A_ms_elapsed = 0;

//Initialize a static global variable to store the returned toggle rate (in ms)
static uint32_t toggle_rate_ms = 0;

//Initialize a static uint8_t global variable that will be incrememnted
// in the Timer 0A periodic task
static uint8_t count = 0;


int main(void)
{
	RGB_LED_Init();
	
	//Initialize the LEDs on the EduBase board (Port B)
	EduBase_LEDs_Init();
	
	EduBase_Button_Init();
	
	//Initialize Timer 0A to generate periodic interrupts every 1 ms
	Timer_0A_Interrupt_Init(&Timer_0A_Periodic_Task);
	
	
	Timer_1A_Interrupt_Init(&Timer_1A_Periodic_Task);
	//Initialize the SysTick timer used to provide blocking delay functions
	SysTick_Delay_Init();
	
	//Initialize the Seven-Segment Disply module on the EduBase Board
	Seven_Segment_Display_Init();
	

	while(1)
	{
		uint8_t button_status = Get_EduBase_Button_Status();
		toggle_rate_ms = Change_Counter_Speed(button_status);
		Seven_Segment_Display(count);
		
	}
}

/**
*@brief
*
*@param None
*
*@return
*/
void Timer_0A_Periodic_Task(void)
{
	Timer_0A_ms_elapsed++;
	
	if(Timer_0A_ms_elapsed >= toggle_rate_ms)
	{
		Timer_0A_ms_elapsed = 0;
		GPIOB ->DATA = GPIOB -> DATA ^ 0x01;
		
		if (count >= 15)
		{
			count = 0;
		}
		else
		{
			count = count + 1;
		}
	}
}

void Timer_1A_Periodic_Task(void)
{
	Timer_1A_ms_elapsed++;
	
	if(Timer_1A_ms_elapsed >= 1000)
	{
		Timer_1A_ms_elapsed = 0;
		GPIOF ->DATA = GPIOF ->DATA ^ 0x08;
	}
}