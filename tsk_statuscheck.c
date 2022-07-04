#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char send_buffer[32];
extern int hash_flag;
extern int send_place_to_write;
extern int send_place_to_read;
char status_check_string[] = "{C}";
extern int end_flag;
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
    //WaitEvent(BUFFER_BLOCK);
    //ClearEvent(BUFFER_BLOCK);
	SetRelAlarm(STATUSALARM, 100, 50); //check status every 50ms
	while(1) {
        if(end_flag) {
            TerminateTask();
        }
        /*if(hash_flag == 1) {
            ChainTask(HASHTASK_ID);
        }*/
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        SuspendAllInterrupts();
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = status_check_string[i];
        }
        EnableAllInterrupts();
        TXSTAbits.TXEN = 1;
        //while(send_place_to_write > send_place_to_read);
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */