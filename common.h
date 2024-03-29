/*          Group 18
 * Authors: Ayberk Gokmen - 2380442
 *          Muhammed Tayyip Ozturk - 2380806
 *          Nilufer Tak - 2310506
 *          Muhammed Yakup Demirtas - 2380285
 *
 */
#ifndef _COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80

#define START_EVENT       0x01
#define FEED_EVENT        0x02
#define WATER_EVENT       0x03
#define PLAY_EVENT        0x04
#define SEND_EVENT        0x05
#define RECEIVE_EVENT     0x06
#define HASH_EVENT        0x07
#define BUFFER_BLOCK      0x08
#define LCD_EVENT         0x09

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define STATUSCHECK_ID       1
#define FEEDTASK_ID          2
#define WATERTASK_ID         3
#define PLAYTASK_ID          4
#define SENDTASK_ID          5
#define RECEIVETASK_ID       6
#define HASHTASK_ID          7
#define LCD_ID               8
#define TASK0_ID             9
#define LCD_ALARM_ID         10

/* Priorities of the tasks */
#define STATUSCHECK_PRIO    3
#define FEEDTASK_PRIO       8
#define WATERTASK_PRIO      7
#define PLAYTASK_PRIO       9
#define HASHTASK_PRIO       10
#define LCDTASK_PRIO        1
#define TASK0_PRIO          2

#define STATUSALARM          0
#define SENDALARM            1
#define LCD_ALARM            2
#define ALARM_TSK0           3


typedef enum{
    IDLE,
    ACTIVE_HARD,
    ACTIVE_EASY,
    END
} simulation_state;

/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
 /* transmits data using serial communication */
void transmitData();
void dataReceived();
void configure_interrupt();
void check_data();
void dataReceived();
void sendChar();

/* Invoked when receive interrupt occurs; meaning that data is received */

#endif

/* End of File : common.h */