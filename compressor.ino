/* Arduino Audio compressor 
 *  
 *  Audio level is sampled from A0, and used to adjust the audio level with a MCP41100 digital pot
 *  
 *  (c) A.G.Doswell 23rd January 2021
 *  
 */

#include <SPI.h>

byte address = 0x11; // address of the pot
int CS = 10; // chip select pin of the pot

int Avg = 20; //average no of samples taken, effects the response time of the compressor.
int Target = 600; // this sets the "threshold" of the compressor.
int Gain = 0; // initial pot position * 10
int Attack = 2; // controls the rate at which the gain is reduced
int Decay = 1; // controls the rate at which the gain is increased.
unsigned long Sample = 0;
int GainPos;

void setup() {
  pinMode (CS, OUTPUT);
  SPI.begin(); 
}

void loop() {

  for (int i = 0; i <= Avg; i++) { // collect the level of the audio
    Sample = Sample + analogRead (A0);
  }
  Sample = Sample / (Avg + 1);
  if (Sample >= Target) { // if the audio is louder than the target, lower the gain.
    Gain = Gain - Attack;
  }
  if (Sample < Target) { // if the audio is quieter than the target, increase the gain.
    Gain = Gain + Decay;
  }
  if (Gain > 2550) { // error trapping
    Gain = 2550;
  }
  if (Gain < 0) {
    Gain = 0;
  }
  GainPos = Gain / 10; // the position of the pot wiper is gain/10
  digitalPotWrite(GainPos); //write the pot wiper position 
}

int digitalPotWrite(int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}
