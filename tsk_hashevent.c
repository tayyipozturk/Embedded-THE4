#include "common.h"
#include <stdio.h>
#include <string.h>

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern unsigned char alert_from_hard[9];
extern unsigned char answer_from_hash[17];
extern unsigned char hash_command[19];
extern int hash_flag;

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/
// INPUT: inp must be NULL terminated char array //
// OUTPUT: out must be a big enough (17 bytes) char array //
void compute_hash(unsigned char *inp, unsigned char *out) {
  unsigned long hash_L;
  unsigned long hash_H;
  unsigned long tmp, tmp_L, tmp_H;
  unsigned char c, *cp;
  unsigned int i;
  cp = inp;
  for (i = 0; i < 2000; i++) {
    hash_L = 5381;
    hash_H = 0;
    while ((c = *cp++)) {
      // hash_HL << 5 //
        tmp_H = (hash_H << 5) + ((hash_L & 0xF8000000) >> 27);
        tmp_L = hash_L << 5;
        // tmp_HL + hash_HL //
        tmp = hash_L + tmp_L;
        if ((tmp < hash_L) || (tmp < tmp_L)) // 4 byte unsigned int overflow !
          tmp_H += 1;
        tmp_L = tmp;
        tmp_H = hash_H + tmp_H;
        // tmp_HL + c //
        tmp = tmp_L + c;
        if ((tmp < c) || (tmp < tmp_L)) // 4 byte unsigned int overflow !
          tmp_H += 1;
        tmp_L = tmp;
        // update hash_HL with tmp_HL //
        hash_L = tmp_L;
        hash_H = tmp_H;
    }
    sprintf(out, "%08lx%08lx", hash_H, hash_L);
    /* left trim */
    cp = out;
  }
}

TASK(HASHTASK) 
{
    int j;
    PIE1bits.RC1IE = 1;	// enable USART receive interrupt
    EnableAllInterrupts();
	while(1) {
        WaitEvent(HASH_EVENT); //enable status check, when start even fired
        ClearEvent(HASH_EVENT);
        //transmitCharAndHello('x');
        compute_hash(alert_from_hard, answer_from_hash);
        SuspendAllInterrupts();
        hash_command[0] = '{';
        hash_command[1] = 'H';
        
        for(j = 0; j < 16; j++){
            hash_command[j+2] = answer_from_hash[j];
        }
        hash_command[18] = '}';
        EnableAllInterrupts();
        TXSTA1bits.TXEN = 1; //enable transmission.
        while(hash_flag);
        SuspendAllInterrupts();
        hash_flag = 0;
        EnableAllInterrupts();
        j = 0;
        //ChainTask(STATUSCHECK_ID);
        
    }
    
	TerminateTask();
    j = 0;
}


/* End of File : tsk_task0.c */