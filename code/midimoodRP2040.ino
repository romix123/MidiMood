/*
 * MidiMood
 * 
 * Ronald Leenes – ronaldleenes@icloud.com
 * 29 augustus 2022
 * 
 * A Minimoog inspired MIDI Control Surface built around a Raspberry PI Pico, 2 CD74HC4067M multiplexers, 26 potentiometers and 11 switches
 * and 3.5" midi in and out (TRS-a)
 * 
 * Compile for Arduino Mbed OS RP2040 boards - rapsberry pi pico
 * 
 * based on Gustavo Silveira's midimood, 2019.
 *    see  http://www.musiconerd.com

  This version is meant for the PCB design v2. Software has migrated to Control Surface
    https://github.com/tttapa/Control-Surface

the pinout is as follows

Mux 1
  Arduino pico    mux 1   mux 2  mux 3
    S0      6          10      10      10
    S1      9          11      11      11
    S2      13          14      14      14
    S3      14          13      13      14  
    sig1    26          COM     
    sig2    27                  COM 
    sig3    28                          COM
    E                   gnd     gnd     gnd

    LED     18

    MIDI_RX 1
    MIDI_TX 0
    SER_TX  16
    SER_RX  17

    SDA     4
    SCL     5

    MISO    8
    MOSI    11
    SCK     10
    SS      12
    
 potentiometers and switches
      conn
      GND
      TX
      RX
      D5
 */
#include <Control_Surface.h>
//https://github.com/tttapa/Control-Surface

//#include <Arduino_Helpers.h> // Include the Arduino Helpers library
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <MIDI_Outputs/CCPotentiometer.hpp>
#include <MIDI_Outputs/CCButtonLatching.hpp>

//using namespace MIDI_Notes;
USBMIDI_Interface usbmidi;

HardwareSerialMIDI_Interface serialmidi {Serial1, 31250};

// Create a MIDI pipe factory to connect the MIDI interfaces to Control Surface
BidirectionalMIDI_PipeFactory<3> pipes;

int LedPin=18; 
CD74HC4067 mux1 = {                                                                            
  26 ,              // analog pin
  {6,9,13,14}, // Address pins S0, S1, S2, S3                                                                                                                                                                                                                                                            
  // 7, // Optionally, specify the enable pin
};

CD74HC4067 mux2 = {                                                                            
  27 ,              // analog pin
  {6,9,13,14}, // Address pins S0, S1, S2, S3                                                                                                                                                                                                                                                            
  // 7, // Optionally, specify the enable pin
};

CD74HC4067 mux3 = {                                                                            
  28 ,              // digital pin
  {6,9,13,14}, // Address pins S0, S1, S2, S3                                                                                                                                                                                                                                                            
  // 7, // Optionally, specify the enable pin
};

//bool PowerOn = 0;
boolean previousPowerState = 0;
int i = 0;
const MIDIAddress note = MIDI_Notes::C(4); // C(4) is middle C
 

CCPotentiometer Potmeters[] = {
  { mux1.pin(0), 0x14},   // rv1  mod 
  { mux1.pin(1), 0x15},   // rv2  tune
  { mux1.pin(2), 0x16},   // rv3  glide  
  { mux1.pin(3), 0x17},   // rv4  01 range
  { mux1.pin(4), 0x18},   // rv5  02 range
  { mux1.pin(5), 0x19},   // rv6  03 range
  { mux1.pin(6), 0x1A},   // rv7  osc 2
  { mux1.pin(7), 0x1B},   // rv8  osc 3
  
  { mux1.pin(8), 0x1C},   // rv16  noise
  { mux1.pin(9), 0x1D},   // rv15 EI vol
  { mux1.pin(10), 0x1E},  // rv14 03 vol                                                                                                                                                                              
  { mux1.pin(11), 0x1F},  // rv13 02 vol  
  { mux1.pin(12), 0x55},  // rv12 01 vol
  { mux1.pin(13), 0x56},  // rv11 03 wave
  { mux1.pin(14), 0x57},  // rv10 02 wave
  { mux1.pin(15), 0x58},  // rv9  01 wave
  
  { mux2.pin(0), 0x59},   // rv17 cutoff
  { mux2.pin(1), 0x5A},   // rv18 emphasis
  { mux2.pin(2), 0x66},   // rv19 contour
  { mux2.pin(3), 0x67},   // rv20 attack
  { mux2.pin(4), 0x68},   // rv21 decay
  { mux2.pin(5), 0x69},   // rv22 sustain
  { mux2.pin(6), 0x6A},  // rv23 volume
  
//  { mux2.pin(7), 0x70},  // J7   
//  { mux2.pin(8), 0x71},  // J8 
//  { mux2.pin(9), 0x71},  // J9
//  { mux2.pin(10), 0x71},  // J10  
 
  { mux2.pin(11), 0x6B},  // rv24 attack  
  { mux2.pin(12), 0x6C},  // rv25 decay 
  { mux2.pin(13), 0x6D},  // rv26 sustain  
};
//
CCButton  Rockers[]= {
{ mux3.pin(0), 0x40},   // sw1  controller
{ mux3.pin(1), 0x41},   // sw2  osc 3
{ mux3.pin(2), 0x42},   // sw3  vol 1  
{ mux3.pin(3), 0x43},   // sw4  vol 2
{ mux3.pin(4), 0x44},   // sw5  vol 3
{ mux3.pin(5), 0x45},   // sw6  vol 4
{ mux3.pin(6), 0x72},   // sw7  vol 
{ mux3.pin(7), 0x50},   // sw8  noise 
{ mux3.pin(8), 0x51},   // sw9  filter
{ mux3.pin(9), 0x52},   // sw10 keyboard 1
{ mux3.pin(10), 0x53},  // sw11 keyboard 2
{ mux3.pin(11), 0x78},  // sw12 power
//////  { mux3.pin(12), 0x52},  // j11            find other code    
// { mux3.pin(13), 0x41},  // j12            find other code
////
};

void updateInputs(){
   for (i = 0; i < 26; i++) {
    Potmeters[i].forcedUpdate();
    delay(10);
   }
   for (i = 0; i < 10; i++) {
   // Rockers[i].update();
    if (Rockers[i].getButtonState() == 0){
     usbmidi.sendControlChange(Rockers[i].getAddress(), 127);
     serialmidi.sendControlChange(Rockers[i].getAddress(), 127);
    } else { 
     usbmidi.sendControlChange(Rockers[i].getAddress(), 0);
     serialmidi.sendControlChange(Rockers[i].getAddress(), 0);
    }
    delay(10);
   } 
}

void setup() { 
 pinMode(LedPin, OUTPUT);  
 pinMode(9, INPUT_PULLUP);

 //this is supposed to suppress powerglitches on Aref
 pinMode(26, INPUT);
 pinMode(23, OUTPUT);
 digitalWrite(23, HIGH);
 
  // Manually connect the MIDI interfaces to Control Surface
  Control_Surface | pipes | usbmidi;
  Control_Surface | pipes | serialmidi;
  usbmidi | pipes | serialmidi;
  // Initialize Control Surface _after_ connecting the interfaces
  Control_Surface.begin();
}
  
void loop() { 
  Control_Surface.loop(); 

  if (!Rockers[11].getButtonState()) {      // powerOn?
       if (previousPowerState == 0) {       // we were turned on, so report all inputs over midi
        updateInputs();
        previousPowerState = 1;
       }
       digitalWrite(LedPin, HIGH);
  } else {
      digitalWrite(LedPin, LOW);
      previousPowerState = 0;
  }
}
