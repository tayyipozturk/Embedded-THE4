#include "common.h"
#include "LCD.h"

extern const char string_pool[2][16];
/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Counter Counter_list[] = 
  {
   /*******************************************************************
    * -------------------------- First counter ------------------------
    *******************************************************************/
   {
     {
       200,                                /* maxAllowedValue        */
        10,                                /* ticksPerBase           */
       100                                 /* minCycle               */
     },
     0,                                    /* CounterValue           */
     0                                     /* Nbr of Tick for 1 CPT  */
   }
  };

Counter Counter_kernel = 
  {
    {
      65535,                              /* maxAllowedValue        */
          1,                              /* ticksPerBase           */
          0                               /* minCycle               */
    },
    0,                                    /* CounterValue           */
    0                                     /* Nbr of Tick for 1 CPT  */
  };

AlarmObject Alarm_list[] = 
  {
   /*******************************************************************
    * -------------------------- First task ---------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     STATUSCHECK_ID,                       /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   },
   /*******************************************************************
    * -------------------------- Second task ---------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     SENDTASK_ID,                       /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   }
 };

#define _ALARMNUMBER_          sizeof(Alarm_list)/sizeof(AlarmObject)
#define _COUNTERNUMBER_        sizeof(Counter_list)/sizeof(Counter)
unsigned char ALARMNUMBER    = _ALARMNUMBER_;
unsigned char COUNTERNUMBER  = _COUNTERNUMBER_;
unsigned long global_counter = 0;

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Resource Resource_list[] = 
  {
   {
      10,                                /* priority           */
       0,                                /* Task prio          */
       0,                                /* lock               */
   }
  };
  
#define _RESOURCENUMBER_       sizeof(Resource_list)/sizeof(Resource)
unsigned char RESOURCENUMBER = _RESOURCENUMBER_;

/**********************************************************************
 * ----------------------- TASK & STACK DEFINITION --------------------
 **********************************************************************/
#define DEFAULT_STACK_SIZE      256
DeclareTask(STATUSCHECK);
DeclareTask(FEEDTASK);
DeclareTask(WATERTASK);
DeclareTask(PLAYTASK);
DeclareTask(SENDTASK);
DeclareTask(RECEIVETASK);
DeclareTask(HASHTASK);
//DeclareTask(LCDTASK);

// to avoid any C18 map error : regroup the stacks into blocks
// of 256 bytes (except the last one).
#pragma		udata      STACK_A   
volatile unsigned char stack0[256];
#pragma		udata      STACK_B
volatile unsigned char stack1[256];
#pragma		udata      STACK_C
volatile unsigned char stack2[256];
#pragma		udata      STACK_D
volatile unsigned char stack3[256];
#pragma		udata      STACK_E
volatile unsigned char stack4[256];
//#pragma		udata      STACK_F
//volatile unsigned char stack5[256];
#pragma     udata
/**********************************************************************
 * ---------------------- TASK DESCRIPTOR SECTION ---------------------
 **********************************************************************/
#pragma		romdata		DESC_ROM
const rom unsigned int descromarea;
/**********************************************************************
 * -----------------------------  task 0 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task0 = {
	STATUSCHECK_PRIO,                       /* prioinit from 0 to 15       */
	stack0,                           /* stack address (16 bits)     */
	STATUSCHECK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	STATUSCHECK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack0)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 1 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task1 = {
	FEEDTASK_PRIO,                       /* prioinit from 0 to 15       */
	stack1,                           /* stack address (16 bits)     */
	FEEDTASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	FEEDTASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack1)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 2 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task2 = {
	WATERTASK_PRIO,                       /* prioinit from 0 to 15       */
	stack2,                           /* stack address (16 bits)     */
	WATERTASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	WATERTASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack2)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 3 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task3 = {
	PLAYTASK_PRIO,                       /* prioinit from 0 to 15       */
	stack3,                           /* stack address (16 bits)     */
	PLAYTASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	PLAYTASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack3)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 4 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task4 = {
	HASHTASK_PRIO,                       /* prioinit from 0 to 15       */
	stack4,                           /* stack address (16 bits)     */
	HASHTASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	HASHTASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack4)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -------------------------------  LCD  ------------------------------
 **********************************************************************/
//rom_desc_tsk rom_desc_task5 = {
//    LCDTASK_PRIO,                       /* prioinit from 0 to 15       */
//    stack5,                      /* stack address (16 bits)     */
//    LCDTASK,                            /* start address (16 bits)     */
//    READY,                          /* state at init phase         */
//    LCDTASK_ID,                         /* id_tsk from 0 to 15         */
//    sizeof(stack5)               /* ctx address   (16 bits)     */
//};


/**********************************************************************
 * --------------------- END TASK DESCRIPTOR SECTION ------------------
 **********************************************************************/
rom_desc_tsk end = {
	0x00,                              /* prioinit from 0 to 15       */
	0x0000,                            /* stack address (16 bits)     */
	0x0000,                            /* start address (16 bits)     */
	0x00,                              /* state at init phase         */
	0x00,                              /* id_tsk from 0 to 15         */
	0x0000                             /* stack size    (16 bits)     */
};

volatile rom unsigned int * taskdesc_addr = (&(descromarea)+1);
	
/* End of File : taskdesc.c */