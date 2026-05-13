/**
*@file Timer_0A_Interrupt.h
*
*@brief Header file for the Timer_0A_Interrupt driver.
*
*This file contains the function definitions for the Timer_0A_Interrupt driver.
*It uses the timer 0A module to generate periodic interrupts.
*
*@note Timer 0A has been configured to generate periodic interrupts every 1 ms
*for the Timers lab. This driver assums that the system clock's frequency is 50 Mhz.
*
*Refer to Table 2-9 (Interrupts) on pages 104 - 106 from the TM4C123G Microcontoller Datasheet.
*to view the Vector Number, Interrupt request (IQR) Number, and Vector Address
*for each peripheral.
*
*@author Kevin Ceron
*/

#include "TM4C123GH6PM.h"

//Declare a pointer to the user-defined task
extern void (*Timer_0A_Task)(void);

/**
 *@brief Inizializes the Timer 0A peripheral to generate periodic interrupts.
 *
 *This function inizializes the TIMER 1A peripheral to generate periodic interrupts for executing a user-defined task.
 *It configures Timer 0A with a 1ms intercal usinf the 50MHz system clock source.
 *The provided task function will be executed whenever Timer 0A generated an interrupt
 *The priority level is set to 1
 *
 *@param task A pointer to the user-defined function to be executed upon Timer 0A interrupt.
 *
 *@return None
 */
void Timer_0A_Interrupt_Init(void(*task)(void));

/**
*@brief The interrupt service routine (ISR) for Timer 0A.
*
*This function is the interrupt  service routine (ISR) for the Timer 0A peripheral.
*It Checks the Timer 0A time-out interrupt fla execued the user-deined task function if the flag is set.
*After executing the task function, it acknowledges the Timer 0A interrupt and clears it.
*
*@param None
*
*@return None
*/
void TIMER0A_HANDLER(void);