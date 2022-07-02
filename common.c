#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char hello[6] = "hello";
unsigned char hello_ind = 0;
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
char rcvd_chr = 0;
simulation_state state = IDLE;

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/
void transmitCharAndHello(char chr){
    hello_chr = chr;
    TXSTAbits.TXEN = 1;
}

void transmitData(){
    if(hello_ind <=4){
        TXREG1 = hello[hello_ind++];
    }
    else if(hello_ind == 5){
        TXREG1 = hello_chr;
        hello_ind++;
    }
    else{
        hello_ind = 0;
        TXSTA1bits.TXEN = 0;
    }
}

void startTransmission()
{
    TXSTA1bits.TXEN = 1; //enable transmission.
}

void check_data()
{
    int i = 0;
    TXSTA1bits.TXEN = 0; //disable transmission.
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
        if(command == 'S') { //status data came
            hunger = recieve_buffer[recieve_place_to_read++%32];
            happy = recieve_buffer[recieve_place_to_read++%32];
            thirst = recieve_buffer[recieve_place_to_read++%32];
            if(hunger <= 40) {
                SetEvent(FEEDTASK_ID, FEED_EVENT);
                money -= 80;
            }
            if(thirst <= 70) {
                SetEvent(WATERTASK_ID, WATER_EVENT);
                money -= 30;
            }
            if(happy <= 20) {
                SetEvent(PLAYTASK_ID, PLAY_EVENT);
                money -= 150;
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
    if(send_place_to_read != send_place_to_write){
        TXREG1 = send_buffer[send_place_to_read++%32];
    }
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
