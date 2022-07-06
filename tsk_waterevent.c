/*          Group 18
 * Authors: Ayberk Gokmen - 2380442
 *          Muhammed Tayyip Ozturk - 2380806
 *          Nilufer Tak - 2310506
 *          Muhammed Yakup Demirtas - 2380285
 *
 */
#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern int water_flag;
extern int any_task;
extern int end_flag;
extern unsigned char live;
char water_string[] = "{W}";
extern void configure_interrupt(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(WATERTASK) 
{
    int i;
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	while(1) {
        if(end_flag) {                      //End command condition
            TXSTA1bits.TXEN = 0;            //Disable transmission
            DisableAllInterrupts();
            TerminateTask();
        }
        WaitEvent(WATER_EVENT);             //WATER EVENT FIRED
        ClearEvent(WATER_EVENT);
        water_flag = 1;
        money-=30;
        WaitEvent(BUFFER_BLOCK);            //Play, feed and water tasks are muting each other to prevent send buffer to be corrupted while sending
        ClearEvent(BUFFER_BLOCK);
        for(i = 0; i < 3; i++){             //Send each character in water_string command to simulation to restore health
            send_buffer[send_place_to_write++%32] = water_string[i];
        }
        TXSTA1bits.TXEN = 1;                //Enable transmission
        while(send_place_to_write > send_place_to_read);
        SuspendAllInterrupts();             //Suspend all interrupts to prevent race condition
        any_task = 0;                       //Set any_task running flag to zero
        water_flag = 0;                     //Change water task running flag to zero
        EnableAllInterrupts();              //Enable all interrupts
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */