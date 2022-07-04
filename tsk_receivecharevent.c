#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char recieve_buffer[32];
extern char send_buffer[32] ;
extern int recieve_place_to_write ;
extern int recieve_place_to_read ;
extern int send_place_to_write;
extern int send_place_to_read;
extern char rcvd_chr;

extern void check_data(void);
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(RECEIVETASK) 
{
    //configure_interrupt();
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    while(1) {
        WaitEvent(RECEIVE_EVENT);
        ClearEvent(RECEIVE_EVENT);
        rcvd_chr = RCREG1;
        recieve_buffer[recieve_place_to_write++%32] = rcvd_chr;
        if(recieve_buffer[recieve_place_to_read++%32] == '}') { //end of data
            check_data();
        }
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
