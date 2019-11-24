//===================================================//
//                        RX                         //
//===================================================//

//===================================================//
//                    INITIALIZE                     //
//===================================================//

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

const uint64_t pipes[2] = {0xDCBAABCD29LL, 0x455d53754CLL};//// Address of PTX and PRX (Payload) {0xABCDABCD71LL, 0x544d52687CLL }; 

int interrupt_time = 10; // In millisenconds (Do not overdo it with too short interrupt time) (MUST BE THE SAME OF PTX)
byte Int_cnt = 0; // Interrupt counter (MUST BE THE SAME OF PTX)
byte Int_TX_cnt = 5; // Set up the number of interrupts count equal to the transmitter interrupts count that trig the data transmission (MUST BE THE SAME OF PTX)
volatile boolean fired = false; //interrupt indicator

unsigned long last_rx_time = 0;

bool fhss_on = false; // Syncronization flag with PTX (at setup time must be false)

// Channel hopping schema (MUST BE THE SAME OF PTX)
byte fhss_schema[]= {12,20}; //{11, 46, 32, 49, 2, 19, 3, 33, 30, 14, 9, 13, 6, 1, 34, 39, 44, 43, 54, 24, 42, 37, 31};
byte ptr_fhss_schema = 0; // Pointer to the fhss_schema array

typedef struct{ // Struct of data received from PTX
  char var_value1;
  char var_valu2;
}
A_t;

typedef struct{ // Struct of data to send to PTX
  int var1_value;
  int var2_value;
  int var3_value;
  int var4_value;
  int var5_value;
  int var6_value;
}
B_t;

A_t data_RX1;
A_t data_RX2;
B_t data_TX;

void setup()
{
  Serial.begin(115200);
  printf_begin();

  radio.begin();
  radio.setPALevel(RF24_PA_MAX); // RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dBM), RF24_PA_MAX (0dBm)

  radio.setRetries(4,9); //(delay, count) -> delay 0=250us ... 15=4000us. count How many retries before giving up, max 15
  
  radio.setAutoAck(1);  //enable auto-acknowlede packets
  radio.enableAckPayload(); //Enable custom payloads on the acknowledge packets
  radio.enableDynamicPayloads(); // Enable dynamically-sized payloads

  radio.setDataRate(RF24_250KBPS); //@param speed RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps

  radio.setChannel(fhss_schema[ptr_fhss_schema]); //@param channel Which RF channel to communicate on, 0-125

  radio.openWritingPipe(pipes[0]); //open a pipe for writing [0] is the addr for writing
  radio.openReadingPipe(1,pipes[1]); //open a piper for reading

//  Setup some initial data value
  //data_RX1.var_value=0;
  //data_RX2.var_value=0;

  data_TX.var1_value=0;
  data_TX.var2_value=0;
  data_TX.var3_value=0;
  data_TX.var4_value=0;
  data_TX.var5_value=0;
  data_TX.var6_value=0;

  radio.startListening();
  radio.printDetails();
  
  // Setup interrupt every interrupt_time value (MUST BE THE SAME OF PTX)
  // CTC mode with clk/8 prescaler
  TCCR1A = 0;
  TCCR1B = 1<<WGM12 | 1<<CS11;
  TCNT1 = 0;         // reset counter
  OCR1A =  (interrupt_time*2000)-1;       // compare A register value
  TIFR1 |= _BV (OCF1A);    // clear interrupt flag
  TIMSK1 = _BV (OCIE1A);   // interrupt on Compare A Match
  last_rx_time = millis();
}

void loop()
{
  if(fired) { // When the interrupt occurred, we need to perform the following task
    fired=false;  // Reset fired flag
    Int_cnt++;  // Increment Interrupts counter
    if(Int_cnt > Int_TX_cnt) Int_cnt=0; // To avoid Interrupts counter become bigger than Int_TX_cnt

//    This is the first part of the algoritm triks
//    If we are not synced with PTX, we stay in listening mode on current channel until the PTX transmit on this channel
//    (remeber that PTX still clycling continuosly throught all channels of fhss schema, even we are or not synced with its)
//    If we are synced with PTX we can do hopping channel following fhss schema

    if(Int_cnt==(Int_TX_cnt-1) && fhss_on==true) {  // Only if we are synced with PTX and if it's time to perform channel change (10ms before expected data from PTX)
      ptr_fhss_schema++;  // Increment pointer of fhss schema array to perform next channel change
      if(ptr_fhss_schema >= sizeof(fhss_schema)) ptr_fhss_schema=0; // To avoid fhss schema array indexing overflow
      radio.setChannel(fhss_schema[ptr_fhss_schema]); // Change channel
      write_ackpayload(); // Prepare the data to be sent back to the PTX (In this case, in advance of 10ms on expected trasmission from PTX)
    }
  }

//  The following code serves to declare "out of sync" of the receiver if we don't receive data for a time needed to cover the entire channels sequency schema (plus a little extra delay)
//  and change channel in case this one is pertrubated for a long time
//  In this mode I'm able to resyncronize PTX and PRX in any case (Reset of PTX, reset of PRX, channels perturbation, ecc.) and very quickly
  if((millis() - last_rx_time) > ((((sizeof(fhss_schema))+5)*10*Int_TX_cnt))) { 
    last_rx_time = millis();
    fhss_on=false;
    ptr_fhss_schema++;
    if(ptr_fhss_schema >= sizeof(fhss_schema)) ptr_fhss_schema=0;
    radio.setChannel(fhss_schema[ptr_fhss_schema]);
  }

  if(radio.available()) { // Ok, we received valid data
//    This is the other part of the algoritm trik
//    PTX transmit when its interrupt occurs, 
//    so we can reset the PRX interrupt counter to align the interrupt time with PTX
//    and we do this every time we receive data. In this manner we stay forever synchronized, 
//    even if there are different clock drift
    TCNT1 = 0; // Reset Interrupt counter 
    fhss_on=true; // Now we can follow the fhss schema (we are synced with PTX channel and with interrupt time of PTX)
    last_rx_time = millis();  // Update received time
    radio.read(&data_RX1,sizeof(data_RX1));
    
//    Print received data
//    Serial.print("Data from TX station : ");
    Serial.print("data_RX.var1_value");
//    Serial.print(", ");
    Serial.print("data_RX.var2_value");
//    Serial.print(", ");
//    Serial.println(fhss_schema[ptr_fhss_schema]); 
    Int_cnt = 0;
    Serial.flush()
  } else {
    // Put here your code to retrive data to send back
    // I setup some value as example
    
  }
}

void write_ackpayload()
{
//    Write data in Ack Payload that we want to sent back to PTX.
//    REMEBER, this data will be transmitted next time we receive new data from PTX
//    BUT; if we have more than 3 pending AckPayload (for any reason), the nRF24l01 will go in hang state !!!!
//    Form page 31 of nRF24L01 datasheet 
//    "If the TX FIFO (PRX) contains more than one payload to a PTX, 
//    payloads are handled using the first in – first out principle. 
//    The TX FIFO (PRX) is blocked if all pending payloads are addressed to a PTX where the 
//    link is lost. In this case, the MCU can flush the TX FIFO (PRX) by using the 
//    FLUSH_TX command."
    radio.flush_tx(); // First, empty TX FIFO
    radio.writeAckPayload(1,&data_TX,sizeof(data_TX));  // Than, write data in TX FIFO
}

ISR (TIMER1_COMPA_vect)
{
  fired = true;
}
