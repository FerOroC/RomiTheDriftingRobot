//Class definition ---------------------------------

class motor_c {
  
  public:
    int pin_dir;
    int pin_pwr;
    float power;
    bool dir;
    volatile long count_e;
    volatile bool oldE_A; 
    volatile bool oldE_B;
    
    motor_c( int pin1, int pin2 );
    
    void stop_motor();
    void move_motor(int newpwr, bool newdir);
    void increase_power(int diff);
    void decrease_power(int diff);
    void operate();
    void forward();
    void backward();
    void reset();
};

//Class constructor ---------------------------------

motor_c::motor_c( int pin1, int pin2 ) {
   pin_pwr = pin1;
   pin_dir = pin2;
   power = 0;
   dir = LOW;

   pinMode( pin_pwr, OUTPUT );
   pinMode( pin_dir, OUTPUT );
   digitalWrite( pin_dir, dir);
   analogWrite( pin_pwr, power );
};

//Class functions----------------------------------

void motor_c::stop_motor(){
  power=0;
  operate();
}

void motor_c::move_motor(int newpwr, bool newdir) {
  power = newpwr;
  dir = newdir;
  operate();
}

void motor_c::increase_power(int diff){
  power += diff;
  operate();
}

void motor_c::decrease_power(int diff){
  power -= diff;
  operate();
}

void motor_c::operate() {
  digitalWrite( pin_dir, dir);
  analogWrite( pin_pwr, power );
}

void motor_c::forward() {
  count_e = count_e + 1;
}

void motor_c::backward() {
  count_e = count_e - 1;
}

void motor_c::reset() {
  count_e = 0;
}

//Class instances ---------------------------------

motor_c LeftMotor(L_PWM_PIN, L_DIR_PIN);
motor_c RightMotor(R_PWM_PIN, R_DIR_PIN);
