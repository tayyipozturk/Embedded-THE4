/*          Group 18
 * Authors: Ayberk Gokmen - 2380442
 *          Muhammed Tayyip Ozturk - 2380806
 *          Nilufer Tak - 2310506
 *          Muhammed Yakup Demirtas - 2380285
 *
 */
#include "LCD.h"
#include "common.h"

/**********************************************************************
 * ----------------------- Local Variables ----------------------------
 **********************************************************************/

extern const char welcome_string[2][16];
extern char happy_string[2][16];
extern char hunger_string[2][16];
extern char thirst_string[2][16];
extern int pressed_time;
extern int start;

/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 * 
 * Writes various strings to LCD 
 * 
 **********************************************************************/
TASK(TASK0)
{
    SetRelAlarm(ALARM_TSK0, 100, 350);

    WaitEvent(ALARM_EVENT);
    ClearEvent(ALARM_EVENT);

    ClearLCDScreen();
    again:
    if(pressed_time == 0 && start == 1){            //If program has just been started
        start = 0;
        LcdPrintString(welcome_string[0], 0, 0);
        LcdPrintString(welcome_string[1], 0, 1);
    }
    else if(pressed_time == 1){
        LcdPrintString(hunger_string[0], 0, 0);     //If RB1 is pressed once or remaining of it from modulo is 1
        LcdPrintString(hunger_string[1], 0, 1);
    }
    else if(pressed_time == 2){
        LcdPrintString(happy_string[0], 0, 0);      //If RB1 is pressed twice or remaining of it from modulo is 2
        LcdPrintString(happy_string[1], 0, 1);
    }
    else if(pressed_time == 3){
        LcdPrintString(thirst_string[0], 0, 0);     //If RB1 is pressed three times or remaining of it from modulo is 3
        LcdPrintString(thirst_string[1], 0, 1);
    }
    else{                                           //Calculate remaining of times button has been pressed by processing it
        pressed_time -= 3;
        goto again;
    }
    
    TerminateTask();
}

/* End of File : tsk_task0.c */