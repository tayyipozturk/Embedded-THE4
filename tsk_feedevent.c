#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern int feed_flag;
extern int any_task;
extern int end_flag;
char feed_string[] = "{F}";
extern void configure_interrupt(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(FEEDTASK) 
{
    int i;
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	while(1) {
        if(end_flag) {
            TerminateTask();
        }
        WaitEvent(FEED_EVENT); //FEED EVENT FIRED
        ClearEvent(FEED_EVENT);
        SuspendAllInterrupts();
        /*if(feed_flag == 0) {
            EnableAllInterrupts();
            continue;
        }*/
        money-=80;
        
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = feed_string[i];
        }
        //feed_flag = 0;
        EnableAllInterrupts();
        WaitEvent(BUFFER_BLOCK);
        ClearEvent(BUFFER_BLOCK);
        TXSTA1bits.TXEN = 1; //enable transmission.
        while(send_place_to_write > send_place_to_read);
        SuspendAllInterrupts();
        any_task = 0;
        EnableAllInterrupts();
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */