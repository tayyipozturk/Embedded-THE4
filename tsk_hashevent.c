/*          Group 18
 * Authors: Ayberk Gokmen - 2380442
 *          Muhammed Tayyip Ozturk - 2380806
 *          Nilufer Tak - 2310506
 *          Muhammed Yakup Demirtas - 2380285
 *
 */
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
extern unsigned char hash_command[19];
extern int hash_flag;
extern int hash_read;
extern int water_flag;
extern int play_flag;
extern int feed_flag;
extern int any_task;
extern int end_flag;
extern unsigned char live;

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
        if(end_flag) {
            TXSTA1bits.TXEN = 0;        //Disable transmission
            DisableAllInterrupts();
            TerminateTask();
        }
        WaitEvent(HASH_EVENT);          //HASH EVENT FIRED
        ClearEvent(HASH_EVENT);
        compute_hash(alert_from_hard, answer_from_hash);        //Call compute_hash() function to calculate hash and gain money
        
        hash_command[0] = '{';                          //Write opening curly brace into hash command
        hash_command[1] = 'H';                          //Write H character into hash command
        for(j = 0; j < 16; j++){                        //Write each character of answer got from compute_hash() into hash command
            hash_command[j+2] = answer_from_hash[j];
        }
        hash_command[18] = '}';                         //Write closing curly brace into hash command
        TXSTA1bits.TXEN = 1;            //Enable transmission to send hash command
        while(hash_flag);               //Wait for transmission of each character of hash_command to TXREG1 to guarantee hash received response
        
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */