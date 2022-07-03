#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
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
        WaitEvent(FEED_EVENT); //FEED EVENT FIRED
        ClearEvent(FEED_EVENT);
        WaitEvent(FEED_EVENT); //FEED EVENT FIRED
        ClearEvent(FEED_EVENT);
        money-=80;
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = feed_string[i];
        }
        TXSTA1bits.TXEN = 1; //enable transmission.
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
