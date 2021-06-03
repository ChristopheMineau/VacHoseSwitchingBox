#ifndef CMI_CONTROL_H
#define CMI_CONTROL_H
#include <Arduino.h>
#include <util/atomic.h>
#include  "CMi_Constants.h"
#include  "CMi_Buttons.h"
#include  "CMi_Leds.h"


/*******************************************************************************************/
// Acces to Globals
extern volatile int pulseToGo; 
extern volatile int countBeforeStopping;   
extern volatile int encoderPulseCount;

namespace DEBUG_ {
  void displayStatus(bool forceDisplay=false);
}

class HoseSwitchControl {
  public:
    enum State
     {
        STOPPED, //0
        RUNNING, //1
        STALLED, //2
     };
     enum Pos
     {
        POS0,         //0
        POS1,         //1
        POS2,         //2
        POS3,         //3
        INTERMEDIATE  //4 
     };
    State state;     // motor turned on or not or fault (stalled) detected
    Pos currentPos;         // current Position
    Pos desiredPos;  // position where we want to go
    Led pos1Led;          // need to update it during dead end loops, so public
    Led pos2Led;          // need to update it during dead end loops, so public
    Led pos3Led;          // need to update it during dsoft start loop, so public    
    
    HoseSwitchControl();
    void init();
    void update(int irCode);
    void posReached();

    bool checkRunningDuration();
    void stuckError();
    
  private:
    Button selectButton;
    void goToDesiredPos();
    void inputUpdate(int irCode); 
    void updateDisplay();
    void roundRobin(Led& l1, Led& l2, Led& l3);  // blink the leds one after the other
    void synchBlink(Led& l1, Led& l2, Led& l3);  // blink the leds all together
    unsigned long startRunningTime;    
};


#endif  // CMI_CONTROL_H
