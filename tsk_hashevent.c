#include "common.h"
#include <stdio.h>
#include <string.h>

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char send_buffer[32];
extern unsigned char alert_from_hard[9];
extern int send_place_to_write;
extern int send_place_to_read;
extern unsigned char answer_from_hash[17];
extern int hash_flag ;
char test[19] = "{H0cb3a65a0ec027bd}";

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
	while(1) {
        WaitEvent(HASH_EVENT); //enable status check, when start even fired
        ClearEvent(HASH_EVENT);
        //transmitCharAndHello('x');
        compute_hash(alert_from_hard, answer_from_hash);
        
        send_buffer[send_place_to_write++%32] = '{';
        send_buffer[send_place_to_write++%32] = 'H';
        
        for(j = 0; j < 16; j++){
            send_buffer[send_place_to_write++%32] = answer_from_hash[j];
        }
        
        send_buffer[send_place_to_write++%32] = '}';
        
        TXSTA1bits.TXEN = 1; //enable transmission.
        hash_flag = 0;
        while(send_place_to_write != send_place_to_read);
        j = 0;
        //ChainTask(STATUSCHECK_ID);
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */

