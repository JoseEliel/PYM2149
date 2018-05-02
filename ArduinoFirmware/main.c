#include <util/delay.h>
#include "ym2149.h"
#include "uart.h"
#include <stdlib.h> 
#include <stdio.h>

// Notes
// Np = 2e6 / (16 * Fn)
#define C (4000000 / (16*130.81))
#define D (4000000 / (16*146.83))
#define E (4000000 / (16*164.81))
#define F (4000000 / (16*174.61))
#define G (4000000 / (16*196.00))
#define A (4000000 / (16*220.00))
#define B (4000000 / (16*246.94))


#define CS (4000000 / (16*138.59))
#define DS (4000000 / (16*155.56))
#define FS (4000000 / (16*185.00))
#define GS (4000000 / (16*207.65))
#define AS (4000000 / (16*233.08))


#define C2 (2000000 / (16*130.81))
#define D2 (2000000 / (16*146.83))
#define E2 (2000000 / (16*164.81))
#define F2 (2000000 / (16*174.61))
#define G2 (2000000 / (16*196.00))
#define A2 (2000000 / (16*220.00))
#define B2 (2000000 / (16*246.94))


#define C2S (2000000 / (16*138.59))
#define D2S (2000000 / (16*155.56))
#define F2S (2000000 / (16*185.00))
#define G2S (2000000 / (16*207.65))
#define A2S (2000000 / (16*233.08))

#define C3 (1000000 / (16*130.81))
#define D3 (1000000 / (16*146.83))
#define E3 (1000000 / (16*164.81))
#define F3 (1000000 / (16*174.61))
#define G3 (1000000 / (16*196.00))
#define A3 (1000000 / (16*220.00))
#define B3 (1000000 / (16*246.94))

#define C3S (1000000 / (16*138.59))
#define D3S (1000000 / (16*155.56))
#define F3S (1000000 / (16*185.00))
#define G3S (1000000 / (16*207.65))
#define A3S (1000000 / (16*233.08))

#define C4 (500000 / (16*130.81))
#define D4 (500000 / (16*146.83))
#define E4 (500000 / (16*164.81))
#define F4 (500000 / (16*174.61))
#define G4 (500000 / (16*196.00))
#define A4 (500000 / (16*220.00))
#define B4 (500000 / (16*246.94))

#define C4S (500000 / (16*138.59))
#define D4S (500000 / (16*155.56))
#define F4S (500000 / (16*185.00))
#define G4S (500000 / (16*207.65))
#define A4S (500000 / (16*233.08))

#define C5 (250000 / (16*130.81))

int deltaf(int freq, int val);


int main() {
  unsigned int i;
  unsigned int data[50] = {C , CS, D, DS, E, F, FS, G, GS, A, AS, B, 
                          C2 , C2S, D2, D2S, E2, F2, F2S, G2, G2S, A2, A2S, B2, 
                          C3 , C3S, D3, D3S, E3, F3, F3S, G3, G3S, A3, A3S, B3, 
                          C4 , C4S, D4, D4S, E4, F4, F4S, G4, G4S, A4, A4S, B4, C5,0};
  int delta;
  int deltaOut;
  int volume=12;
  const char* input;
  set_ym_clock();
  set_bus_ctl();
  initUART();


  // reset registers
  for (i=0; i<16; i++) {
    send_data(i, 0);
  }
  send_data(7, 0xf8); // 7 decides what is turned on
  send_data(8, volume); // 8, 9 and 10 decide if envelope is On or off in each channel
  send_data(9, volume);
  send_data(10, volume); //RA
  send_data(11, 1000 & 0xff); // RB Sets freq of envelope
  send_data(12, 1000 >> 8); // RC Sets freq of envelope (rough)
  send_data(13, 0x0E); // Sets Shape of envelope
  delta=0;// Spot on Freq
  for/*ever*/(;;) {
      input = readString();
      i= atoi(input); //This integer encodes everything. The 10^2 digit says channel (1 to 3) or modifications (5 onwards)
      if(i<=16){
        volume=i;
      }

      if(i>=500){
        if(i==500){//Turn Envelope Off Envelope
            send_data(8, volume);
            send_data(9, volume);
            send_data(10, volume);
        }
        else if(i>627){ //Set Freq phase (detune)
          delta=-(i-692); //This makes the wheel work, it is centered at 692
          //data[49]=-delta; // This makes it so that the silence is still 0 freq even with phase.
         char snum[12];
         snprintf(snum, sizeof snum, "%d", delta);
         writeString(snum);
         writeString("\n");
        }
        else{//Freq of envelope + turn it on.
        send_data(8, 0x10);
        //send_data(9, 0x10);
        //send_data(10, 0x10);
        send_data(11, ((i-500)*6 + (627-i)*60)/3 & 0xff); // RB
        send_data(12, ((i-500)*6 + (627-i)*60)/3 >> 8); // RC
        }
      }

      //Channel A
      if(i>=100 && i<200){
       deltaOut=deltaf(data[i-100],delta);
       
       char snum[12];
       snprintf(snum, sizeof snum, "%d", deltaOut);
       writeString(snum);
       writeString("\n");
       
       send_data(0, (data[i-100]+ deltaOut) & 0xff);
       send_data(1, (data[i-100] + deltaOut) >> 8);
          }
      //Channel B
      if(i>=200 && i<300){
       deltaOut=deltaf(data[i-200],delta); 
       send_data(2, (data[i-200]+ deltaOut) &  0xff);
       send_data(3, (data[i-200]+ deltaOut) >> 8);
          }
      //Channel C
      if(i>=300 && i<400){
       deltaOut=deltaf(data[i-300],delta); 
       send_data(4, (data[i-300]+ deltaOut) & 0xff);
       send_data(5, (data[i-300]+ deltaOut) >> 8);
          }
      //Three Channels at once NOT IMPLEMENTED
      // if(i>=400 && i<500){
      //  send_data(0, (data[i-400]+delta) & 0xff);
      //  send_data(1, (data[i-400]+delta) >> 8);
      //  send_data(2, (data[i-400]+delta) >> 1 & 0xff);
      //  send_data(3, (data[i-400]+delta) >> 9);
      //  send_data(4, (data[i-400]+delta) >> 2 & 0xff);
      //  send_data(5, (data[i-400]+delta) >> 10);
      // }

      //writeString(input);
      //_delay_ms(200.);
  }

  return 0;
}


int deltaf(int freq, int val) {
   /* local variable declaration */
   int deltaint;
   float deltafloat;
   if (val>=0){
   deltafloat=(float)freq*(float)val/64;
   deltaint=(int)deltafloat;
   }
   else{
   deltafloat=(float)freq*(float)val/126;
   deltaint=(int)deltafloat;
   }
   return deltaint; 
}

