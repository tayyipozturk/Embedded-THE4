#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(TASK0) 
{
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	SetRelAlarm(ALARM_TSK0, 100, 50);
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        //TXSTA1bits.TXEN = 1; //enable transmission.
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */