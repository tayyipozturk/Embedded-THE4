#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern int water_flag;
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
        WaitEvent(WATER_EVENT); //WATER EVENT FIRED
        ClearEvent(WATER_EVENT);
        //DisableAllInterrupts();
        WaitEvent(BUFFER_BLOCK);
        money-=30;
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = water_string[i];
        }
        water_flag = 0;
        ClearEvent(BUFFER_BLOCK);
        //ResumeAllInterrupts();
        TXSTA1bits.TXEN = 1; //enable transmission.
        while(send_place_to_write != send_place_to_read);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
