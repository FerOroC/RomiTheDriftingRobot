#ifndef _KINEMATICS_H
#define _KINEMATICS_H

//Class definition ---------------------------

class kinematics_c {
  public:
    float left_speed;
    float right_speed;

    float left_mps;
    float right_mps;

    float distance;

    float timestamp;
    float interval;
    
    float last_leftM_count;
    float last_rightM_count;
    float theta;
    float delta_theta;
    
    kinematics_c();
    
    void update(); 
    void get_speed();   
    void calculate_slip();
    
}; 

//Class constructor ---------------------------------

kinematics_c::kinematics_c() {
  distance = 0;
  theta = 0;
} 

//Class functions----------------------------------

void kinematics_c::update() {

  float distance_left=(left_mps)*(interval/1000000);
  float distance_right=(right_mps)*(interval/1000000);

  delta_theta=(distance_right-distance_left)/(2*7.45);
  theta+=delta_theta;
  if (theta > 2 * PI){
    theta -= 2*PI;
  }
  distance = (distance_left+distance_right)/2;

}

void kinematics_c::get_speed(){
  unsigned long new_time = micros();
  interval = new_time - timestamp;
  timestamp = micros();
  
  left_speed =  ((LeftMotor.count_e - last_leftM_count)/interval)*10000;
  right_speed = ((RightMotor.count_e - last_rightM_count)/interval)*10000;
  last_leftM_count = LeftMotor.count_e;
  last_rightM_count = RightMotor.count_e;
  left_mps=(left_speed*100)*distancePerCount;
  right_mps=(right_speed*100)*distancePerCount;
}

#endif

//Class instances ----------------------------------

kinematics_c Kinematics;
