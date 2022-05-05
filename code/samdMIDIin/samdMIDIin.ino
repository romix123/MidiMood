/**
 * SAMD21 SaaS controller
 *  by Ronald Leenes, romix@macuser.nl
 *  version 0.1, October 2021
 * 
 *  This code is half of the SAMD21/ESP32 pcm5102 synth module.
 *  It handles USB midi in, reads 8 pots and can display messages on a 128x32 oled screen. Each pot has a neopixel, finally there is a NeoPixel that 
 *  can be used as a status display or whatever.
 *  
 *  The SMAD21 code requires slightly modified Adafruit SAMD QT PY configuration files.
 *    In your Arduino15/packages/adafruit/hardware/samd/1.7.3/variants/qtpy_m0 change the following files:
 *      variant.cpp
 *        after the definition of the USB Pins add these two lines:
           { PORTA, 14, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // D22 Neopix
           { PORTA, 27, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // E
 * 
 *      in variant.h
 *        change teh pin def to
 *        // Number of pins defined in PinDescription array
            #define PINS_COUNT           (23u)
            #define NUM_DIGITAL_PINS     (23u)
         in // LEDS 
          add
            #define PIN_NEOIXEL2     (22u)

          The modded files are in this github repository

          To put the SAMD21 in programming mode, connect to the board using a usb cable in the keyboard/host port and press the reset button above the 
          Neopixel twice. The NeoPixel will light up green if it is in programming mode.(it turns red if something is not ok).
          The board registers as cu.usbmodem14401 on my machine.

          In Music operation, connect an OTG cable to the keyboard port, plug in a MIDI keyboard in this cable and connect usb power to the power in port.
          
 *        
 * TODO:
 *        allow for a flexible pot to midi CC code mapping
 *        clean up code
 *        fix MIDI keyboard recognition

 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#define NEOPIXEL      11
Adafruit_NeoPixel  pxl(1, NEOPIXEL, NEO_GRB + NEO_KHZ800);

#include <cppQueue.h>             // fifo queue
#define  MAX_LENGTH    64
#define  EOT_CHAR    ';'   // Contents will be written to serial upon End Of Transmission character reception
// Instantiate queue (with possible overwriting to ensure treatment of EOT_CHAR even if message longer than queue)
cppQueue  q(sizeof(char), MAX_LENGTH, FIFO, true);


/*
 * MIDI host macros
 */
 
#include <usbh_midi.h>
#include <usbhub.h>
#define Is_uhd_in_received0(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT0) == USB_HOST_PINTFLAG_TRCPT0)
#define Is_uhd_in_received1(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT1) == USB_HOST_PINTFLAG_TRCPT1)
#define uhd_ack_in_received0(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT0
#define uhd_ack_in_received1(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT1
#define uhd_byte_count0(p)                        usb_pipe_table[p].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT
#define uhd_byte_count1(p)                        usb_pipe_table[p].HostDescBank[1].PCKSIZE.bit.BYTE_COUNT
#define Is_uhd_in_ready0(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_BK0RDY) == USB_HOST_PSTATUS_BK0RDY)  
#define uhd_ack_in_ready0(p)                       USB->HOST.HostPipe[p].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY
#define Is_uhd_in_ready1(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_BK1RDY) == USB_HOST_PSTATUS_BK1RDY)  
#define uhd_ack_in_ready1(p)                       USB->HOST.HostPipe[p].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK1RDY
#define uhd_current_bank(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_CURBK) == USB_HOST_PSTATUS_CURBK)  
#define Is_uhd_toggle(p)                          ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_DTGL) == USB_HOST_PSTATUS_DTGL)  
#define Is_uhd_toggle_error0(p)                   usb_pipe_table[p].HostDescBank[0].STATUS_PIPE.bit.DTGLER
#define Is_uhd_toggle_error1(p)                   usb_pipe_table[p].HostDescBank[1].STATUS_PIPE.bit.DTGLER

USBHost UsbH;
USBHub Hub(&UsbH);
USBH_MIDI  Midi(&UsbH);
bool doPipeConfig = false;
bool usbConnected = false;

//SAMD21 datasheet pg 836. ADDR location needs to be aligned. 
uint8_t bufBk0[64] __attribute__ ((aligned (4))); //Bank0
uint8_t bufBk1[64] __attribute__ ((aligned (4))); //Bank1


// On SAMD boards where the native USB port is also the serial console, use

#define SerialDebug Serial1 //SERIAL_PORT_MONITOR
//#else
#define MidiOut Serial1
//#endif
#define DEBUG  0

#if DEBUG == 1
#define debug(x) Serial1.print(x)
#define debugln(x) Serial1.println(x)
#define debugh(x,y) Serial1.println(x,y)
#else
#define debug(x)
#define debugln(x)
#define debugh(x,y)
#endif


 uint32_t pixelOff = 0;
 uint32_t nowT = 0;
 bool receivedNote = false;

// Things to do at bootup
void setup() {
  MidiOut.begin(115200);
  initUSBHost();

}


void loop() 
{
if (receivedNote && (nowT > pixelOff)) {
       pxl.setPixelColor(0, pxl.Color(0, 0, 0));
       pxl.show();
       receivedNote= false;
    }
    
//Note that Task() polls a hub if present, and we want to avoid polling.
  //So these conditions carry out enumeration only, and then stop running.
  //The idea is that except for enumeration (and release) this loop should 
  //be quiescent. 
  if (doPipeConfig || (!usbConnected && (UsbH.getUsbTaskState() != USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE))) {
    UsbH.Task();
  } else if (usbConnected && (UsbH.getUsbTaskState() != USB_STATE_RUNNING)){
    UsbH.Task();
  }
  
  if (usbConnected && (UsbH.getUsbTaskState() == USB_STATE_RUNNING) ) {
    if ( Midi && (Midi.GetAddress() != Hub.GetAddress()) && (Midi.GetAddress() != 0)) {
      if (doPipeConfig) {
        //There is a chance that a disconnect interrupt may happen in the middle of this
        //and result in instability. Various tests here on usbConnected to hopefully
        //reduce the chance of it.
        uint32_t epAddr = Midi.GetEpAddress();
        doPipeConfig = false;
        uint16_t rcvd;
        while (usbConnected && (USB->HOST.HostPipe[Midi.GetEpAddress()].PCFG.bit.PTYPE != 0x03)) {
          UsbH.Task(); 
          Midi.RecvData(&rcvd,  bufBk0);
        }
        USB->HOST.HostPipe[epAddr].BINTERVAL.reg = 0x01;//Zero here caused bus resets.
        usb_pipe_table[epAddr].HostDescBank[0].ADDR.reg = (uint32_t)bufBk0;
        usb_pipe_table[epAddr].HostDescBank[1].ADDR.reg = (uint32_t)bufBk1;
        USB->HOST.HostPipe[epAddr].PCFG.bit.PTOKEN = tokIN;
        USB->HOST.HostPipe[epAddr].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY; 
        uhd_unfreeze_pipe(epAddr); //launch the transfer
        USB->HOST.HostPipe[epAddr].PINTENSET.reg = 0x3; //Enable pipe interrupts
      }
    }
  } else {
    USB_SetHandler(&CUSTOM_UHD_Handler);
    USB->HOST.HostPipe[Midi.GetEpAddress()].PINTENCLR.reg = 0xFF; //Disable pipe interrupts
  }  

  // push whatever midi commands we have on the fifi queue ou to the ESP32
  if (!q.isEmpty())       // Only if q is not empty
  {
    char snd;
    while (q.pop(&snd))    // While characters are available in q
      {
        MidiOut.write(snd);
      }
      NoteOnFlash();
  }
  
}

void CUSTOM_UHD_Handler(void)
{
  uint32_t epAddr = Midi.GetEpAddress();
  if (USB->HOST.INTFLAG.reg == USB_HOST_INTFLAG_DCONN) {
    SerialDebug.println("Connected");
    doPipeConfig = true;
    usbConnected = true;
  } else if (USB->HOST.INTFLAG.reg == USB_HOST_INTFLAG_DDISC) {
    SerialDebug.println("Disconnected");
    usbConnected = false;
    USB->HOST.HostPipe[epAddr].PINTENCLR.reg = 0xFF; //Disable pipe interrupts
  }
  UHD_Handler();
  uhd_freeze_pipe(epAddr);


  //Both banks full and bank1 is oldest, so process first. 
  if (Is_uhd_in_received0(epAddr) && Is_uhd_in_received1(epAddr) && uhd_current_bank(epAddr)) {
    handleBank1(epAddr);
  }
  if(Is_uhd_in_received0(epAddr)) {
    handleBank0(epAddr);
  } 
  if (Is_uhd_in_received1(epAddr)) {
    handleBank1(epAddr);
  }
  uhd_unfreeze_pipe(epAddr);    
}


unsigned char count;
unsigned char command[2];

void handleBank0(uint32_t epAddr){
  int rcvd = uhd_byte_count0(epAddr);
  bool validMIDI = false;

    for (int i = 0; i < rcvd; i++) {
    unsigned char a = bufBk0[i];
          if(a > 127)
            count = 0;
          else  {
            if(count == 2) {
              if((command[0] & 0xf0) == 0x90) validMIDI = true; // note on
              if((command[0] & 0xf0) == 0x80) validMIDI = true; // note off
              if((command[0] & 0xf0) == 0xb0) validMIDI = true; // control change message
                  if (validMIDI) {
                    q.push(&command[0]);                    //push on fifo
                    q.push(&command[1]);
                    q.push(&a);
                  }
             }
           }
         if(count < 2)
          command[count] = a;
          count++;
         }
    uhd_ack_in_received0(epAddr);
    uhd_ack_in_ready0(epAddr);
 }

void handleBank1(uint32_t epAddr){
  int rcvd = uhd_byte_count1(epAddr);
  bool validMIDI = false;
    for (int i = 0; i < rcvd; i++) {
      
    unsigned char a = bufBk1[i];
          if(a > 127)
            count = 0;
          else  {
            if(count == 2) {
              if((command[0] & 0xf0) == 0x90) validMIDI = true; // note on
              if((command[0] & 0xf0) == 0x80) validMIDI = true; // note off
              if((command[0] & 0xf0) == 0xb0) validMIDI = true; // control change message
                  if (validMIDI) {
                      
                    q.push(&command[0]);                    //push on fifo
                    q.push(&command[1]);
                    q.push(&a);
                  }
             }
           }
         if(count < 2)
          command[count] = a;
          count++;
         }
    uhd_ack_in_received1(epAddr);
    uhd_ack_in_ready1(epAddr);
 }
    
 void initUSBHost(){
  if (UsbH.Init()) {
       while (1); //halt
    }
    USB_SetHandler(&CUSTOM_UHD_Handler);
 }


void MIDI_control_Change(uint8_t chan, uint8_t data1, uint8_t data2){
  q.push(&chan);
  q.push(&data1);
  q.push(&data2);
}

void NoteOnFlash() {
     pxl.setPixelColor(0, pxl.Color(0, 128, 0));
     pxl.show();
     pixelOff = millis()+200;
     receivedNote = true;    
 }
