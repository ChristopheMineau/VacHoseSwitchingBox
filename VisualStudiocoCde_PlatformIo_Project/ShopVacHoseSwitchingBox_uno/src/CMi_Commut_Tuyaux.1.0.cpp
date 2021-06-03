#define VERSION "1.0"
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include  "CMi_Constants.h" /// !! Check this file for activating DEBUG prints
#include <IRremote.h>  // https://github.com/Arduino-IRremote/Arduino-IRremote   Version3.1.0
#include  "CMi_Control.h"

// Globals
volatile int encoderPulseCount = 0;    // encoder pulses count variable
volatile int pulseToGo = 0;            // encoder count to be reached
volatile int countBeforeStopping = 0;      // saved encoder count befor stopping
HoseSwitchControl hoseSwitch = HoseSwitchControl();

// Interrupt Service Routines
void encoderInterrupt() {      // encoder rising edge detected
  encoderPulseCount++;
  if (digitalRead(POS_SENSOR) == HIGH) {
        // To calculate the number of pulses per rev and inertia, reset twice and look at the traces
      if (encoderPulseCount > 100) {   // in order to reset only when we enter the magnetic field
        countBeforeStopping = encoderPulseCount;
        encoderPulseCount = 0; 
        if (pulseToGo == 9999) {     // desired Pos = Pos0 reference point
          hoseSwitch.posReached();
        }
      }
  }

  if (pulseToGo != 9999) {     
    pulseToGo --;
    if (pulseToGo == 0) hoseSwitch.posReached();
  }
  
}



///// BEGIN //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP ////
void setup() {

// Just to know which program is running on my Arduino
  Serial.begin(115200);
  Serial.println(F("START " __FILE__ " from " __DATE__ " version " VERSION));

#ifndef DEBUG
  Serial.end();
#endif   // DEBUG

  // Set up pins
  // Input digital pins
  pinMode(SELECT_BUTTON, INPUT_PULLUP); // Select button (INPUT_PULLUP)
  pinMode(POS_SENSOR, INPUT);           // Hall effect POS 0 sensor (active LOW)
  pinMode(ENCODER_A, INPUT);            // Motor digital encoder A
  pinMode(REMOTE_RECEIVER, INPUT);      // IR Remote control receiver

  // Output digital pins
  pinMode(LED_POS_1, OUTPUT);       // set Position indicator led pins
  pinMode(LED_POS_2, OUTPUT);
  pinMode(LED_POS_3, OUTPUT);
  pinMode(REMOTE_SEND, OUTPUT);     // IR Remote control emitter
  pinMode(MOTOR, OUTPUT);           // Motor driver transistor base
 
  // Remote control 
  IrReceiver.begin(REMOTE_RECEIVER, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
                                     
  // set up Encoder interrupt IRQ0 on pin 2.
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderInterrupt, RISING); //IRQ 0

  // Init Control object
  hoseSwitch.init();
}
///// END //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP //// SETUP ////





namespace DEBUG_ {
  void displayStatus(bool forceDisplay=false) {
    unsigned  int countCopy;
    unsigned  int pulseToGoCopy;
#ifndef DEBUG                    // See CMi_Constants.h for activating the DEBUG flag
    return;
#endif   // DEBUG
    static unsigned long lastTime = 0;
    if (((millis() - lastTime )> 3000) or forceDisplay) {
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        countCopy = encoderPulseCount;
        pulseToGoCopy = pulseToGo;
      }
      DEBUG_PRINTLN();
      DEBUG_PRINT("hoseSwitch state = ");
      DEBUG_PRINTLN(hoseSwitch.state);
      DEBUG_PRINT("desiredPos = ");
      DEBUG_PRINTLN(hoseSwitch.desiredPos); 
      DEBUG_PRINT("currentPos = ");
      DEBUG_PRINTLN(hoseSwitch.currentPos);
      DEBUG_PRINT("encoderCount = ");
      DEBUG_PRINTLN(countCopy);
      DEBUG_PRINT("pulseToGo = ");
      DEBUG_PRINTLN(pulseToGoCopy);
      lastTime = millis();
    }  
  }
}

int checkRemoteControl() {
  int irCode = 0xFF;
  static int currentlyReceivingCode = 0xFF;
  int receivedCode;
  static unsigned long lastIrChangeTime = 0;
  
  if (IrReceiver.decode()) {
    IrReceiver.resume(); // Enable receiving of the next value
    receivedCode = IrReceiver.decodedIRData.command; 
    
    if (receivedCode != currentlyReceivingCode) {           // reject repetitions of the same code
      if ((IrReceiver.decodedIRData.protocol == RC5) and   // Protocol used by the Philips VCR remote control
          ((receivedCode == 0x4) or
            (receivedCode == 0x5) or
            (receivedCode == 0x6))) {
#ifdef DEBUG
          IrReceiver.printIRResultShort(&Serial);  // FOR DEBUG 
          DEBUG_PRINTLN(IrReceiver.decodedIRData.flags);
#endif   // DEBUG

            currentlyReceivingCode = receivedCode;
            irCode = receivedCode;
            lastIrChangeTime = millis();
        }   
    }     
  }
  if (millis() - lastIrChangeTime > DEBOUNCE_TIME)
     currentlyReceivingCode = 0xFF;
  
  return irCode;
}




///// BEGIN //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP ////
void loop() {
  DEBUG_::displayStatus();
  
  // Get the last Faceplate and remote control actions and update the controller
  hoseSwitch.update(checkRemoteControl());
   
  // diagnose a possible fault
  if ((hoseSwitch.state == HoseSwitchControl::State::RUNNING) and hoseSwitch.checkRunningDuration())
    hoseSwitch.stuckError();
}
///// END //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP //// LOOP ////
