//Class definition

class controller_c {
  
  public:
    int state;

    unsigned long timestamp;
    unsigned long interval;

    float distance;

    int base_power;
    int left_power;
    int right_power;

    float heading_mag;
    float head_bias;
    float last_heading;
    float heading;

    float last_mag;

    float heading_imu;

    float head_m;
    float left_encoder_acc;
    float right_encoder_acc;
    float imu_acc;

    float head_pid;
    float last_left_speed;
    float last_right_speed;
    float left_pid;
    float right_pid;

    float desired_angle;
    float desired_difference;
    float holder_theta;

    float alpha;
    float mag_alpha;
    
    controller_c();
    void printParams();
    void get_m();
    void get_acc_imu();
    void get_acc_encoders();
    void find_power();
    void turn_left();
    void calibrate_mag();
    void go();
    void control();

};

//Class constructor ----------------------------

controller_c::controller_c() {
  state = STATE_INITIAL;
  left_power = 0;
  right_power = 0;
  holder_theta = 0;
  heading = 0;
  alpha = 0.01;
  mag_alpha = 0.85;
  last_heading = 0;
  distance = 0;
};

// Class functions -------------------------------

void controller_c::get_m(){
   mag.read();
   Mag.calibratedReadings(mag.m.x,'x');
   Mag.calibratedReadings(mag.m.y,'y');
   heading_mag = atan2(Mag.calibrated_Y, Mag.calibrated_X) - head_bias;
   heading_mag = (mag_alpha*heading_mag)+((1-mag_alpha)*last_mag);
   if (heading_mag>PI){
      heading_mag-=2*PI;
   }
   if (heading_mag<-PI){
      heading_mag+=2*PI;
   }
   heading = heading_mag;
   last_mag = heading_mag;
   IMUnit.IMUScalars();
   //Serial.println(heading_mag);
   heading_imu = IMUnit.heading_change;
   heading = (alpha * (IMUnit.heading_change)) + ((1-alpha)*heading_mag);
   last_heading = heading;
}

void controller_c::get_acc_imu(){
  
}

void controller_c::get_acc_encoders(){
  
}

void controller_c::printParams(){
  Serial.println(heading);
  ///Serial.println(head_pid);
//  Serial.print("Left PID: ");
//  Serial.println(left_acc_pid);
//  Serial.print("right PID: ");
//  Serial.println(right_acc_pid);
}

void controller_c::turn_left(){
  RightMotor.move_motor( 20, R_DIR_FWD );
  LeftMotor.move_motor( 20, L_DIR_BWD );
}

void controller_c::calibrate_mag(){
    mag.read();
    Mag.findMax(mag.m.x,'x');
    Mag.findMax(mag.m.y,'y');
    Mag.findMax(mag.m.z,'z');
  
    Mag.findMin(mag.m.x,'x');
    Mag.findMin(mag.m.y,'y');
    Mag.findMin(mag.m.z,'z');
  
    Mag.getOffset('x');
    Mag.getOffset('y');
    Mag.getOffset('z');
  
    Mag.getRange('x');
    Mag.getRange('y');
    Mag.getRange('z');
    
    Mag.getAverage();
  
    Mag.getScale('x');
    Mag.getScale('y');
}

void controller_c::go(){  
  head_pid = HeadPID.update(heading);
  float new_left = Kinematics.left_mps;
  float new_right = Kinematics.right_mps;
  left_encoder_acc = 0.001 * (new_left - last_left_speed)/interval;
  right_encoder_acc = 0.001 * (new_right - last_right_speed)/interval;
  float left_slip = left_encoder_acc - IMUnit.acc;
  float right_slip = right_encoder_acc - IMUnit.acc;
  left_pid = LeftSlipPID.update(left_slip);
  right_pid = RightSlipPID.update(right_slip);

 // base_power = 30;
  if (base_power < 200){
    base_power += 30;
  }
  if(head_pid < 0){
    left_power = base_power - left_pid + head_pid;
    right_power = base_power - right_pid;
  }else{
    left_power = base_power - left_pid;
    right_power = base_power - right_pid - head_pid;
  }

  if(left_power < 0){ left_power = 0; }
  if(right_power < 0){ right_power = 0; }
  
  
//  left_acc_pid = LeftAccPID.update(left_power, 255);
//  right_acc_pid = RightAccPID.update(right_power, 255);
//  left_slip_pid = LeftSlipPID.update(left_encoder_acc, left_imu_acc);
//  right_slip_pid = RightSlipPID.update(right_encoder_acc, right_imu_acc);
  
  //printParams();
//  find_power();
//  if (right_power > 255) {right_power = 255; }
//  if (left_power > 255) {left_power = 255; }
//  if (right_power < 0) {right_power = 0; }
//  if (left_power < 0) {left_power = 0; }
  RightMotor.move_motor(right_power, R_DIR_FWD);
  LeftMotor.move_motor(left_power, L_DIR_FWD);
}

//void controller_c::find_power(){
//  int left_diff;
//  int right_diff;
//  if  (head_pid < 0){
//    left_diff = (left_acc_pid);
//    right_diff = (right_acc_pid);
//  } else {
//    left_diff = (left_acc_pid);
//    right_diff = (right_acc_pid);
//  }
//  left_power += left_diff;
//  right_power += right_diff;
//}

void controller_c::control(){
  float time = millis();
  interval = time - timestamp;;
  timestamp = time;
  Kinematics.get_speed();
  Kinematics.update();
  distance += Kinematics.distance;
  
  switch(state){
    
   case STATE_INITIAL:
    Beeper.beep_initial();
    holder_theta = 0;
    turn_left();
    state = STATE_CALIBRATE;
    break;
   case STATE_CALIBRATE:
    holder_theta += Kinematics.delta_theta;
    if(holder_theta > 2*PI){
      LeftMotor.stop_motor();
      RightMotor.stop_motor();
      delay(1000);
      IMUnit.calibrate();
      mag.read();
      Mag.calibratedReadings(mag.m.x,'x');
      Mag.calibratedReadings(mag.m.y,'y');
      heading = atan2(Mag.calibrated_Y, Mag.calibrated_X);
      head_bias = heading;
      heading = 0;
      get_m();
      HeadPID.reset();
      state = STATE_GO;
      break;
    }
    calibrate_mag();
    turn_left();
    break;
   case STATE_GO:
//    if(distance > 300){
//      LeftMotor.stop_motor();
//      RightMotor.stop_motor();
//      state = STATE_STOP;
//      break;
//    }
//    if(IMUnit.dis > 300){
//      LeftMotor.stop_motor();
//      RightMotor.stop_motor();
//      state = STATE_STOP;
//      break;
//    }
    get_m();
    go();
    printParams();
    break;
    
   case STATE_STOP:
    break;
  }
}

// Class instances -------------------------------

controller_c Controller;
