#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char send_buffer[32];
extern int send_place_to_write;
char status_check_string[] = "{C}";
extern void configure_interrupt(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

TASK(STATUSCHECK) 
{
    int i;
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    WaitEvent(START_EVENT); //enable status check, when start even fired
    ClearEvent(START_EVENT);
	SetRelAlarm(STATUSALARM, 100, 50); //check status every 50ms
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = status_check_string[i];
        }
        TXSTAbits.TXEN = 1;
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */
