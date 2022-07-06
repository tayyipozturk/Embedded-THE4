/*          Group 18
 * Authors: Ayberk Gokmen - 2380442
 *          Muhammed Tayyip Ozturk - 2380806
 *          Nilufer Tak - 2310506
 *          Muhammed Yakup Demirtas - 2380285
 *
 */
#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char hello_chr = 0;
char recieve_buffer[32] = "";
char send_buffer[32] = "";

int recieve_place_to_write = 0;
int recieve_place_to_read = 0;
int send_place_to_write = 0;
int send_place_to_read = 0;
int money = 0;
int hunger = 0;
int thirst = 0;
int happy = 0;
int updatedHunger = 100;
int updatedThirst = 100;
int updatedHappy = 100;
int oldHunger = 100;
int oldThirst = 100;
int oldHappy = 100;
int hash_flag = 0;
int water_flag = 0;
int play_flag = 0;
int feed_flag = 0;
int hash_read = 0;
int end_flag = 0;
char rcvd_chr = 0;
int any_task = 0;
unsigned char first = 1;
unsigned char sending = 0;
unsigned char receiving = 0;
simulation_state state = IDLE;
unsigned char alert_from_hard[9];
unsigned char answer_from_hash[17];
unsigned char hash_command[19];
int hash_sent = 0;


/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/
void transmitCharAndHello(char chr){
    hello_chr = chr;
    TXSTAbits.TXEN = 1;
}

void startTransmission()
{
    TXSTA1bits.TXEN = 1; //enable transmission.
}

void check_data()           //PARSE INCOMING DATA TO ACT
{
    TXSTA1bits.TXEN = 0; //disable transmission.
    while(recieve_buffer[recieve_place_to_read++%32] != '{');
    recieve_place_to_read--;
    if(recieve_buffer[recieve_place_to_read++%32] == '{') { //start of data
        char command = recieve_buffer[recieve_place_to_read++%32];
        HERE:
        if(command == 'G') {                    //Start command given
            unsigned char upperMoney;
            unsigned char lowerMoney;
            recieve_place_to_read++;
            upperMoney = recieve_buffer[recieve_place_to_read++%32];
            lowerMoney = recieve_buffer[recieve_place_to_read++%32];
            recieve_place_to_read++;            //Pass the "}" character for convenience
            money = upperMoney;                 //Left bytes
            money = money << 8;                 //Right bytes
            money |= lowerMoney;                //Calculate starting money
            if(money != 1190) {                 //Set state of the game according to the starting money
                state = ACTIVE_HARD;
            }
            else {
                state = ACTIVE_EASY;
            }
            SetEvent(STATUSCHECK_ID, START_EVENT);//Set STATUSCHECK to start sending check command periodically with help of interrupt
            //SetEvent(STATUSCHECK_ID, BUFFER_BLOCK);
        }
        if(command == 'E') {                    //End condition
            end_flag = 1;
            TXSTA1bits.TXEN = 0;                //Disable transmission
            DisableAllInterrupts();
        }
        if(command == 'A') {                    //Hash indicator is received
            int i = 0;
            for(i = 0 ; i< 8 ; i++){            //Send hash input to the hash input buffer alert_from_hard
                alert_from_hard[i] = recieve_buffer[recieve_place_to_read++%32];
            }
            alert_from_hard[8] = '\0';
            while(any_task);                    //Wait until feed, water and play tasks are finished
            hash_flag = 1;                      //Set hash_flag 1 before running HASHTASK
            SetEvent(HASHTASK_ID, HASH_EVENT);  //Set HASHTASK to enter calculating hash task
            //SetEvent(HASHTASK_ID, BUFFER_BLOCK);
        }
        if(command == 'M'){
            unsigned char upperMoney;
            unsigned char lowerMoney;
            int earned_money;
            upperMoney = recieve_buffer[recieve_place_to_read++%32];
            lowerMoney = recieve_buffer[recieve_place_to_read++%32];
            recieve_place_to_read++;                    //Pass the "}" character for convenience
            earned_money = upperMoney;
            earned_money = earned_money << 8;
            earned_money |= lowerMoney;
            money += earned_money;
        }
        if(command == 'S') {                            //Status data came
            hunger = recieve_buffer[recieve_place_to_read++%32];
            happy = recieve_buffer[recieve_place_to_read++%32];
            thirst = recieve_buffer[recieve_place_to_read++%32];
            if(first || hash_flag){
                first = 0;
                updatedHunger = hunger;
                updatedHappy = happy;
                updatedThirst = thirst;
            }
            else{
                if(updatedHunger - 60 == hunger){       //If hunger parameter is not updated fast by data received from simulation
                    hunger = updatedHunger;
                }
                else{                                   //If hunger parameter is decreased or updated differently
                    updatedHunger = hunger;
                }
                if(updatedThirst - 30 == thirst){       //If thirst parameter is not updated fast by data received from simulation
                    thirst = updatedThirst;
                }
                else{                                   //If thirst parameter is decreased or updated differently
                    updatedThirst= thirst;
                }
                if(updatedHappy - 80 == happy){         //If happy parameter is not updated fast by data received from simulation
                    happy = updatedHappy;
                }
                else{                                   //If happy parameter is decreased or updated differently
                    updatedHappy = happy;
                }
            }
            if(hunger <= 40) {                          //Check if hunger parameter is less than 40 to optimize our money
                if(money >= 80 && any_task == 0 && updatedHunger <= 40){
                    //feed_flag = 1;
                    updatedHunger = hunger + 60;        //Save next hunger value to be received
                    any_task = 1;                       //Some task is running
                    SetEvent(FEEDTASK_ID, FEED_EVENT);  //Set FEEDTASK to continue
                    SetEvent(FEEDTASK_ID, BUFFER_BLOCK);//Do block water and play tasks
                }
            }
            if(thirst <= 70) {                          //Check if thirst parameter is less than 70 to optimize our money
                if(money >= 30 && any_task == 0 && updatedThirst <= 70){
                    //water_flag = 1;
                    updatedThirst = thirst + 30;        //Save next thirst value to be received
                    any_task = 1;                       //Some task is running
                    SetEvent(WATERTASK_ID, WATER_EVENT);//Set WATERTASK to continue
                    SetEvent(WATERTASK_ID, BUFFER_BLOCK);//Do block hunger and play tasks
                }
            }
            if(happy <= 20) {                          //Check if happy parameter is less than 20 to optimize our money
                if(money >= 150 && any_task == 0 && updatedHappy <= 20){
                    //play_flag = 1;
                    updatedHappy = happy + 80;          //Save next happy value to be received
                    any_task = 1;                       //Some task is running
                    SetEvent(PLAYTASK_ID, PLAY_EVENT);  //Set PLAYTASK to continue
                    SetEvent(PLAYTASK_ID, BUFFER_BLOCK);//Do block hunger and water tasks
                }
            }
        }
    }
}

/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
    rcvd_chr = RCREG1;              //Save the character got from RCREG1
    recieve_buffer[recieve_place_to_write++%32] = rcvd_chr; //Write the character to the next position availeble to be written
    if(rcvd_chr == '}') {           //End of data is received
        check_data();               //Call check_data function to determine what to do with incoming command
    }
    rcvd_chr = 0;
}

void sendChar(){
    if(hash_flag && hash_read < 19) {
        TXREG1 = hash_command[hash_read++];
    }
    if(hash_read >= 19) {       //End of hash buffer hash_command is read
        hash_flag = 0;
        hash_read = 0;
    }
    if(send_place_to_read != send_place_to_write){//Check if buffer is read until last write position
        TXREG1 = send_buffer[send_place_to_read++%32];//Write character in specified index of buffer to the TXREG1
    }
}

void configure_interrupt(){
    INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1;	// enable USART transmit interrupt
	PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	PIR1 = 0;			// clear all peripheral flags
	
	INTCONbits.PEIE = 1;// enable peripheral interrupts
	INTCONbits.GIE = 1;	// globally enable interrupts
    TXSTA1 = 0x04;
}

/* End of File : common.c */