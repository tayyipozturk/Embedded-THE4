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
extern char send_buffer[32];
extern int hash_flag;
extern int send_place_to_write;
char status_check_string[] = "{C}";
extern int end_flag;
extern unsigned char live;
extern void configure_interrupt(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

TASK(STATUSCHECK) 
{
    int i;
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    WaitEvent(START_EVENT); //enable status check, when start event fired
    ClearEvent(START_EVENT);
	SetRelAlarm(STATUSALARM, 100, 50); //check status every 50ms
	while(1) {
        if(end_flag) {                      //End command condition
            TXSTA1bits.TXEN = 0;
            DisableAllInterrupts();
            TerminateTask();
        }
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        for(i = 0; i < 3; i++){             //Send each character in status_check_string command to simulation to restore health
            send_buffer[send_place_to_write++%32] = status_check_string[i];
        }
        TXSTAbits.TXEN = 1;                 //Enable transmission 
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */