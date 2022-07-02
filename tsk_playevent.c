#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
char play_string[] = "{P}";
extern void configure_interrupt(void);

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(PLAYTASK) 
{
    int i;
    PIE1bits.RC1IE = 1;	// enable USART 
	while(1) {
        WaitEvent(PLAY_EVENT); //PLAY EVENT FIRED
        ClearEvent(PLAY_EVENT);
        WaitEvent(PLAY_EVENT); //PLAY EVENT FIRED
        ClearEvent(PLAY_EVENT);
        money-=150;
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = play_string[i];
        }
        TXSTA1bits.TXEN = 1; //enable transmission.
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
