#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern void configure_interrupt(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(SENDTASK) 
{
    
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    while(1) {
        WaitEvent(SEND_EVENT);
        ClearEvent(SEND_EVENT);
        TXSTA1bits.TXEN = 1; //enable transmission.
        if(send_place_to_read != send_place_to_write) {
            TXREG1 = send_buffer[send_place_to_read++%32];
        }
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
