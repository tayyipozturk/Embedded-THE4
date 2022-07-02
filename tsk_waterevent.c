#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char send_buffer[32];
extern int send_place_to_write;
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
        WaitEvent(WATER_EVENT); //WATER EVENT FIRED
        ClearEvent(WATER_EVENT);
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = water_string[i];
        }
        TXSTA1bits.TXEN = 1; //enable transmission.
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
