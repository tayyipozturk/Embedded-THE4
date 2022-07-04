#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern int money;
extern char send_buffer[32];
extern int send_place_to_write;
extern int send_place_to_read;
extern int play_flag;
extern int any_task;
extern int end_flag;
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
        if(end_flag) {
            TerminateTask();
        }
        WaitEvent(PLAY_EVENT); //PLAY EVENT FIRED
        ClearEvent(PLAY_EVENT);
        SuspendAllInterrupts();
        if(any_task == 0) {
            EnableAllInterrupts();
            continue;
        }
        
        //WaitEvent(BUFFER_BLOCK);
        //ClearEvent(BUFFER_BLOCK);
        money-=150;
        for(i = 0; i < 3; i++){
            send_buffer[send_place_to_write++%32] = play_string[i];
        }
        //play_flag = 0;
        WaitEvent(BUFFER_BLOCK);
        ClearEvent(BUFFER_BLOCK);
        EnableAllInterrupts();
        
        TXSTA1bits.TXEN = 1; //enable transmission.
        while(send_place_to_write > send_place_to_read);
        SuspendAllInterrupts();
        any_task = 0;
        EnableAllInterrupts();
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */