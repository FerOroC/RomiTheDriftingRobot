//Class definition ---------------------------------

class AccPID_c {
  public:

    AccPID_c(float P, float I, float D); 
    void setGains(float P, float I, float D); 
    void reset();
    float update(float demand, float measurement); 
 
    void printComponents(); 

    float Kp_output = 0;
    float Ki_output = 0;
    float Kd_output = 0;

  private:

    float Kp; //Proportional
    float Ki; //Integral
    float Kd; //Derivative

    float output_signal       = 0;
    float last_error          = 0; 
    float integral_error      = 0; 
    unsigned long last_millis = 0;

};

//Class constructor ---------------------------------

AccPID_c::AccPID_c(float P, float I, float D)
{
  //Store the gains
  setGains(P, I, D);
  //Set last_millis
  reset();
}

//Class functions ---------------------------------

void AccPID_c::printComponents() {
  Serial.print(Kp_output);
  Serial.print(", ");
  Serial.print(Kd_output);
  Serial.print(", ");
  Serial.print(Ki_output);
  Serial.print(", ");
  Serial.println(output_signal);
}

void AccPID_c::reset() {
  last_error      = 0;
  integral_error  = 0;
  Kp_output       = 0;
  Ki_output       = 0;
  Kd_output       = 0;
  last_millis     = millis();
}

void AccPID_c::setGains(float P, float I, float D) {
  Kp = P;
  Kd = D;
  Ki = I;
}

float AccPID_c::update(float power, float demand) {
  long time_now = millis();
  long diff_time = time_now - last_millis;
  last_millis = time_now;
  
  float time_delta = (float)diff_time;

  float error = demand - power;

  float error_delta = (error - last_error)/time_delta;
  last_error = error;

  integral_error += (error * time_delta);

  //Calculate P,I,D Term contributions.
  Kp_output = error * Kp;
  Kd_output = error_delta * Kd; 
  Ki_output = integral_error * Ki; 

  output_signal = Kp_output + Kd_output + Ki_output;

  return output_signal;
}

//Class instances ---------------------------------

AccPID_c LeftAccPID(0.2, 0, 0);
AccPID_c RightAccPID(0.2, 0, 0);
