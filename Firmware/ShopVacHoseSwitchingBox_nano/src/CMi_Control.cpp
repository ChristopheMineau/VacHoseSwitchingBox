#include  "CMi_Control.h"

HoseSwitchControl::HoseSwitchControl() : 
        state(State::STOPPED), 
        currentPos(Pos::INTERMEDIATE),
        desiredPos(Pos::POS1),
        pos1Led(Led(LED_POS_1)),
        pos2Led(Led(LED_POS_2)),
        pos3Led(Led(LED_POS_3)),
        selectButton(Button(SELECT_BUTTON)),
        startRunningTime(0) {}

void HoseSwitchControl::init() {
  desiredPos = Pos::POS0;
  goToDesiredPos();
}

void HoseSwitchControl::update(int irCode) {
  inputUpdate(irCode);
  updateDisplay();
}

void HoseSwitchControl::goToDesiredPos(){
  unsigned int countCopy;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    countCopy = encoderPulseCount;
  }
  if ((state == State::STOPPED) and (desiredPos != currentPos)) {
    startRunningTime = millis();
    state = State::RUNNING ;
    switch(desiredPos) {
      case(Pos::POS0):
        pulseToGo = 9999;
        break;
      case(Pos::POS1):
        pulseToGo = POS1_COUNT;
        break;
      case(Pos::POS2):
        pulseToGo = POS2_COUNT;
        break;
      case(Pos::POS3):
        pulseToGo = POS3_COUNT;
        break; 
      default:
        break;      
    }
    if (pulseToGo != 9999) {
      if (pulseToGo < countCopy) // We have passed the target, need to add a full turn
        pulseToGo = pulseToGo + ONE_REVOLUTION_COUNT - countCopy - STOPPING_INERTIA;
      else
        pulseToGo = pulseToGo - countCopy - STOPPING_INERTIA;
    }

    digitalWrite(MOTOR, HIGH);
    currentPos = Pos::INTERMEDIATE;
    DEBUG_PRINTLN();
    DEBUG_PRINT("Encoder before moving : ");
    DEBUG_PRINTLN(countCopy); 
    DEBUG_PRINT("Going to Pos : ");
    DEBUG_PRINT(desiredPos);
    DEBUG_PRINT(" Pulse to Go : ");
    DEBUG_PRINTLN(pulseToGo); 
  }
}

void HoseSwitchControl::posReached(){  // beware called by interrupt, once decount reached
  int countCopy;
  digitalWrite(MOTOR, LOW);
  state = State::STOPPED ;             // this will stop the call by interrupt
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    countCopy = countBeforeStopping;
  }
  DEBUG_PRINTLN();
  DEBUG_PRINT(" ### Reached Pos : ");
  DEBUG_PRINTLN(desiredPos);
  DEBUG_PRINT("Encoder before stopping : ");
  DEBUG_PRINTLN(countCopy);
  if (desiredPos == Pos::POS0) {
    DEBUG_PRINTLN("**** POS 0 (Hall sensor) ***");  
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      postCalibrationTime = millis();
    }
  }
  currentPos = desiredPos;
}

void HoseSwitchControl::inputUpdate(int irCode) {
  static Pos previousDesiredPos = Pos::POS3;


  // handle post Calibration delay to go to startup POS = 1
  if (postCalibrationTime != 0) {
    if ((millis() - postCalibrationTime) > POST_CAIBRATION_DELAY) {
      desiredPos = Pos::POS1;
      goToDesiredPos();
      postCalibrationTime = 0;
    }
  }

  // handle irCode if any
  if (irCode != 0xFF and state == State::STOPPED) {
    switch (irCode) {
      case 0x4:     //  button 4   on PHILIPS VCR remote control
        desiredPos = Pos::POS1;
        break;
      case 0x5:     //  button 5  on PHILIPS VCR remote control
        desiredPos = Pos::POS2;
        break;
      case 0x6:     //  button 6  on PHILIPS VCR remote control
        desiredPos = Pos::POS3;
        break;
    }
    goToDesiredPos();
  }   
  
  // handle faceplate button
  if (selectButton.getState() and state == State::STOPPED) {
    DEBUG_PRINTLN();
    DEBUG_PRINTLN("Switch button Hit.");
    switch(previousDesiredPos) {
      case(Pos::POS1):
        desiredPos = Pos::POS2;
        break;
        
      case(Pos::POS2):
        desiredPos = Pos::POS3;
        break;
        
      case(Pos::POS3):
        desiredPos = Pos::POS1;   
        break;

      default:
        break;
    }
    previousDesiredPos = desiredPos;
    goToDesiredPos();   
  } 
  
}

void HoseSwitchControl::updateDisplay() {

  switch(state) {
    case(State::STOPPED):
      
      switch(currentPos) {
        case(Pos::POS0):
          pos1Led.reset();
          pos2Led.reset();
          pos3Led.reset();
          break;
        case(Pos::POS1):
          pos1Led.set();
          pos2Led.reset();
          pos3Led.reset();
          break;
        case(Pos::POS2):
          pos1Led.reset();
          pos2Led.set();
          pos3Led.reset();
          break;
        case(Pos::POS3):
          pos1Led.reset();
          pos2Led.reset();
          pos3Led.set();
          break;
        default:
          break;
      }
      break;
    
    case(State::RUNNING):
      roundRobin(pos1Led, pos2Led, pos3Led);
      break;
      
    case(State::STALLED):
      synchBlink(pos1Led, pos2Led, pos3Led);
      break;
  }
  
  pos1Led.update();
  pos2Led.update();
  pos3Led.update();
}

void HoseSwitchControl::roundRobin(Led& l1, Led& l2, Led& l3) {
  static unsigned long lastBlinkTime = 0;
  static byte lastBlinkLed = 4;

  if ((millis()-lastBlinkTime)>BLINK_TIME) {
    switch(lastBlinkLed) {
      case(1):
          l1.reset();
          l2.set();
          l3.reset();
          lastBlinkLed = 2;
          break;
      case(2):
          l1.reset();
          l2.reset();
          l3.set();
          lastBlinkLed = 3;
          break;
      case(3):
          l1.reset();
          l2.set();
          l3.reset();
          lastBlinkLed = 4;
          break; 
      case(4):
          l1.set();
          l2.reset();
          l3.reset();
          lastBlinkLed = 1;
          break;             
    }
    lastBlinkTime = millis();
  }
}

void HoseSwitchControl::synchBlink(Led& l1, Led& l2, Led& l3) {
  static unsigned long lastBlinkTime = 0;
  if (not lastBlinkTime) {
    l1.set();
    l2.set();
    l3.set();
  }
  if ((millis()-lastBlinkTime)>BLINK_TIME) {
    l1.toggle();
    l2.toggle();
    l3.toggle();
    lastBlinkTime = millis();
  }
}

bool HoseSwitchControl::checkRunningDuration() {
  if (state != State::RUNNING) return false;
  if ((millis() - startRunningTime) > STUCK_TIME_LIMIT) return true;
  return false;
}


// Error Handler
void HoseSwitchControl::stuckError() {   //  0 Pos is not reached
  DEBUG_PRINTLN("### Stuck Error. Motor does not turn or Hall effect sensor down ...");
  state = HoseSwitchControl::State::STALLED;
  digitalWrite(MOTOR, LOW);
  DEBUG_::displayStatus(true);
  while (1) {                  // Dead end ...
    updateDisplay();
    delay(100);
  } 
}
