#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern int water_flag;
extern int any_task;
extern int end_flag;
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
        if(end_flag) {
            TerminateTask();
        }
        WaitEvent(WATER_EVENT); //WATER EVENT FIRED
        ClearEvent(WATER_EVENT);
        //SuspendAllInterrupts();
        /*if(water_flag == 0) {
            EnableAllInterrupts();
            continue;
        }*/
        
        money-=30;
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = water_string[i];
        }
        //water_flag = 0;
        //EnableAllInterrupts();
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