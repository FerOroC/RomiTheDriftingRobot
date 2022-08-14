//Motor pins
#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15

//Motor directions
#define L_DIR_FWD  LOW 
#define R_DIR_FWD  LOW
#define L_DIR_BWD HIGH
#define R_DIR_BWD HIGH

//States 
#define STATE_INITIAL   0
#define STATE_CALIBRATE 1
#define STATE_GO        2
#define STATE_STOP      3

//Encoder pins
#define E1_A_PIN   7
#define E1_B_PIN  23
#define E0_A_PIN  26

//Physicial metrics
//#define wheelCircumference 221.472
#define wheelCircumference 218
#define distancePerCount 0.01538
#define PI 3.1415926535897932384626433832795

//gravyT
#define Gravity 9.80665
