/*
###########################
!!!!!!! Code for TX !!!!!!!
###########################
*/

// Setup for nRF24L01
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// set Chip-Enable (CE) and Chip-Select-Not (CSN) radio setup pins
#define CE_PIN 9
#define CSN_PIN 10

// create RF24 radio object using selected CE and CSN pins
RF24 radio(CE_PIN,CSN_PIN);

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };  // Address of PTX and PRX
int interrupt_time = 10; // In millisenconds (Do not overdo it with too short interrupt time)
byte Int_cnt = 0; // Interrupt counter
byte Int_TX_cnt = 5; // Setting up the number of interrupts count that trig the data tramission 
volatile boolean fired = false;
    
byte Failed_handshakes = 0;
byte max_failed_attempts = 5;

// Channels hopping schema (Each number MUST BE unique in the sequence)
// It's possible to use non unique number in sequence, but in this case we must send the ptr_fhss_schema value to PRX
// thought the data_TX struct to align ptr_fhss_schema of PRX
// However, I prefer the sequence of unique numbers
byte fhss_schema[]= {12,20}; //{11, 46, 32, 49, 2, 19, 3, 33, 30, 14, 9, 13, 6, 1, 34, 39, 44, 43, 54, 24, 42, 37, 31}; // You can do it as long as you like
byte ptr_fhss_schema = 0; // Pointer to the fhss_schema array

typedef struct{ // Struct of data to send to PRX
  int var1_value;
  int var2_value;
  int var3_value;
  int var4_value;
  byte var5_value;
}
A_t;

typedef struct{ // Struct of data received from PRX
  int var1_value;
  int var2_value;
  int var3_value;
  int var4_value;
  int var5_value;
  int var6_value;
}
B_t;

A_t data_TX;
B_t data_RX;

void setup()
{
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); // RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dBM), RF24_PA_MAX (0dBm)
  
  radio.setRetries(4,9);
  
  radio.setAutoAck(1);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  
  radio.setDataRate(RF24_250KBPS);
  
  radio.setChannel(fhss_schema[ptr_fhss_schema]);
  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  
//  Setup some initial data value
  data_TX.var1_value=1;
  data_TX.var2_value=0;
  data_TX.var3_value=1;
  data_TX.var4_value=0;
  data_TX.var5_value=1;
  
  radio.startListening();
  radio.printDetails();
  
  // Setup interrupt every interrupt_time value
  // CTC mode with clk/8 prescaler
  TCCR1A = 0;
  TCCR1B = 1<<WGM12 | 1<<CS11;
  TCNT1 = 0;         // reset counter
  OCR1A =  (interrupt_time*2000)-1;       // compare A register value 
  TIFR1 |= _BV (OCF1A);    // clear interrupt flag
  TIMSK1 = _BV (OCIE1A);   // interrupt on Compare A Match
}

void loop()
{
  if(fired){ // When the interrupt occurred, we need to perform the following task  
    fired=false;  // Reset fired flag
    Int_cnt++;  // Increment Interrupts counter

    if(Int_cnt > Int_TX_cnt){
      Int_cnt=0; // To avoid Interrupts counter become bigger than Int_TX_cnt
    }

    if(Int_cnt == Int_TX_cnt) { // If it's time to transmit.
      radio.stopListening();
      radio.write( &data_TX, sizeof(data_TX));

      //If we received data in ACK Payload, read and print values.
      if(radio.isAckPayloadAvailable()){ 
        radio.read(&data_RX, sizeof(data_RX));
        Serial.print("Data from RX station : ");
        Serial.print(data_RX.var1_value);
        Serial.print(", ");
        Serial.print(data_RX.var2_value);
        Serial.print(", ");
        Serial.print(data_RX.var3_value);
        Serial.print(", ");
        Serial.print(data_RX.var4_value);
        Serial.print(", ");
        Serial.print(data_RX.var5_value);
        Serial.print(", ");
        Serial.println(data_RX.var6_value);
      }
      else{
        //counter++ if(counter==x) then do FH
        Failed_handshakes++;
        Serial.print("Failed to recieve Handshake...Resending data");
        if(Failed_handshakes==max_failed_attempts){
          
          Failed_handshakes = 0;
          
          ptr_fhss_schema++;  // Increment pointer of fhss schema array to perform next channel change
          
          if(ptr_fhss_schema >= sizeof(fhss_schema)){
            ptr_fhss_schema=0; // To avoid array indexing overflow
          }
          radio.setChannel(fhss_schema[ptr_fhss_schema]); // Change channel    
        }
      }
      radio.startListening();
    } 
  }
}

ISR (TIMER1_COMPA_vect){
  fired = true;
}