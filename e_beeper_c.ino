class beeper_c {
  
  public:
    beeper_c() { };

    void beep_initial();
    void beep_line();
    void beep_stop();
};

void beeper_c::beep_initial(){
  for (int i = 0; i < 5; i++){
    analogWrite(6,40);
    delay(50);
    analogWrite(6,0);
    delay(950);
  }
}

void beeper_c::beep_line(){
   analogWrite(6,40);
}

void beeper_c::beep_stop(){
   analogWrite(6,0);
}

beeper_c Beeper;
