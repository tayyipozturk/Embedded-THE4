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
int hash_flag = 0;
int water_flag = 0;
int play_flag = 0;
int feed_flag = 0;
int hash_read = 0;
char rcvd_chr = 0;
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

void check_data()
{
    TXSTA1bits.TXEN = 0; //disable transmission.
    while(recieve_buffer[recieve_place_to_read++%32] != '{');
    recieve_place_to_read--;
    if(recieve_buffer[recieve_place_to_read++%32] == '{') { //start of data
        char command = recieve_buffer[recieve_place_to_read++%32];
        if(command == 'G') { //start command given
            unsigned char upperMoney;
            unsigned char lowerMoney;
            recieve_place_to_read++;
            upperMoney = recieve_buffer[recieve_place_to_read++%32];
            lowerMoney = recieve_buffer[recieve_place_to_read++%32];
            recieve_place_to_read++; //pass the "}"
            money = upperMoney;
            money = money << 8;
            money |= lowerMoney;
            if(money != 1190) {
                state = ACTIVE_HARD;
            }
            else {
                state = ACTIVE_EASY;
            }
            SetEvent(STATUSCHECK_ID, START_EVENT);
        }
        if(command == 'A') {
            int i = 0;
            for(i = 0 ; i< 8 ; i++){
                alert_from_hard[i] = recieve_buffer[recieve_place_to_read++%32];
            }
            alert_from_hard[8] = '\0';
            hash_flag = 1;
            SetEvent(HASHTASK_ID, HASH_EVENT);
        }
        if(command == 'M'){
            unsigned char upperMoney;
            unsigned char lowerMoney;
            int earned_money;
            upperMoney = recieve_buffer[recieve_place_to_read++%32];
            lowerMoney = recieve_buffer[recieve_place_to_read++%32];
            recieve_place_to_read++; //pass the "}"
            earned_money = upperMoney;
            earned_money = earned_money << 8;
            earned_money |= lowerMoney;
            money += earned_money;
        }
        if(command == 'S') { //status data came
            hunger = recieve_buffer[recieve_place_to_read++%32];
            happy = recieve_buffer[recieve_place_to_read++%32];
            thirst = recieve_buffer[recieve_place_to_read++%32];
            if(hunger <= 40 && feed_flag == 0) {
                if(money >= 80){
                    feed_flag = 1;
                    SetEvent(FEEDTASK_ID, FEED_EVENT);
                
                }
            }
            if(thirst <= 70 && water_flag == 0) {
                if(money >= 30){
                    water_flag = 1;
                    SetEvent(WATERTASK_ID, WATER_EVENT);
                    
                }
            }
            if(happy <= 20 && play_flag == 0) {
                if(money >= 150){
                    play_flag = 1;
                    SetEvent(PLAYTASK_ID, PLAY_EVENT);
                    
                }
            }
        }
    }
}


/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
    rcvd_chr = RCREG1;
    recieve_buffer[recieve_place_to_write++%32] = rcvd_chr;
    if(rcvd_chr == '}') { //end of data
        check_data();
    }
    rcvd_chr = 0;
}

void sendChar(){
    //TXSTA1bits.TXEN = 1;
    if(hash_flag && hash_read < 19) {
        TXREG1 = hash_command[hash_read++];
    }
    if(hash_read == 19) {
        hash_flag = 0;
        hash_read = 0;
    }
    else if(send_place_to_read != send_place_to_write){
        TXREG1 = send_buffer[send_place_to_read++%32];
    }
    
    /*if(hello_chr == 'x'){
        TXREG1 = hello_chr;
    }*/
    //
    //TXSTA1bits.TXEN = 0;

    //transmitData();
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
