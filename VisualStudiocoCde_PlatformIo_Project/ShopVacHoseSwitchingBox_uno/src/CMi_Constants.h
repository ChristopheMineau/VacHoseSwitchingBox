#ifndef CMI_CONSTANTS_H
#define CMI_CONSTANTS_H

// Pins
#define SELECT_BUTTON    10       // Select button (INPUT_PULLUP)
#define POS_SENSOR       2        // Hall effect POS 0 sensor (active LOW)
#define ENCODER_A        3        // Motor digital encoder A
#define REMOTE_SEND      4        // IR Remote control emitter
#define REMOTE_RECEIVER  5        // IR Remote control receiver

#define LED_POS_1        6
#define LED_POS_2        7
#define LED_POS_3        8
#define MOTOR            9

// Constants
#define ONE_REVOLUTION_COUNT 8400    // Pulse count to turn one full revolution
#define STOPPING_INERTIA 165
#define POS1_COUNT  0                                       // Pulse count to reach POS 1 from POS 0
#define POS2_COUNT  POS1_COUNT + ONE_REVOLUTION_COUNT/3     // Pulse count to reach POS 2 from POS 0  // POS1_COUNT + ONE_REVOLUTION_COUNT/3 
#define POS3_COUNT  5400                                    // Pulse count to reach POS 3 from POS 0  // POS2_COUNT + ONE_REVOLUTION_COUNT/3 =5600

#define DEBOUNCE_TIME 400  // in millis
#define BLINK_TIME 400     // in millis
#define STUCK_TIME_LIMIT 12000 // running time beyond which the motor is considered as stuck

#define DEBUG   // Activates the debug prints, comment for final version
#ifdef DEBUG
  #define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...) void()
  #define DEBUG_PRINTLN(...) void()
#endif //    DEBUG

#endif //    CMI_CONSTANTS_HH
