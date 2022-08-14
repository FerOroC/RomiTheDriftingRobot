#include <Wire.h>
#include <LIS3MDL.h>

LIS3MDL mag;

class Mag_c {
  public:
  
  float mag_sensitivity;
  float max_Reading_X=-1000000.00;
  float max_Reading_Y=-1000000.00;
  float max_Reading_Z=-1000000.00;
  
  float min_Reading_X=1000000.00;
  float min_Reading_Y=1000000.00;
  float min_Reading_Z=1000000.00;
  
  float offset_X;
  float offset_Y;
  float offset_Z;
  
  float range_X;
  float range_Y;
  float range_Z;
  
  float average_Range;
  
  float scale_X;
  float scale_Y;
  
  float calibrated_X;
  float calibrated_Y;
  
  Mag_c( float sensitivity);

  void findMax(float reading, char axis);
  void findMin(float reading, char axis);
  void getOffset(char axis);
  void getRange(char axis);
  void getAverage();
  void getScale(char axis);
  void calibratedReadings(float reading, char axis);

  
//  void calibrate();
//  void readCalibrated();
//  void scaleSensor();
//  void printScaledSensor();
//  void filteredSensor();
//  void scalars();
//
//  float scaled_rot_x;
//  float scaled_rot_z;
//  float scaled_rot_y;
//  float last_rot;
//  float head_bias;
//  float calibrated_rot;
//  float filtered_rot;
//
//  float alpha = 0.9;
//
//  float temp_filter;
//  long last_millis;
//
//  float heading;
};

Mag_c::Mag_c(float sensitivity){
  mag_sensitivity = sensitivity;
};


void Mag_c::findMax(float reading, char axis){
  if (axis=='x'){
    if (reading>max_Reading_X){
      max_Reading_X=reading;
    }
  } else if (axis=='y'){
    if (reading>max_Reading_Y){
      max_Reading_Y=reading;
    }
  } else if (axis=='z'){
    if (reading>max_Reading_Z){
      max_Reading_Z=reading;
    }
  }
}

void Mag_c::findMin(float reading, char axis){
  if (axis=='x'){
    if (reading<min_Reading_X){
      min_Reading_X=reading;
    }
  } else if (axis=='y'){
    if (reading<min_Reading_Y){
      min_Reading_Y=reading;
    }
  } else if (axis=='z'){
    if (reading<min_Reading_Z){
      min_Reading_Z=reading;
    }
  }
}

void Mag_c::getOffset(char axis){
  if (axis=='x'){
    offset_X=(max_Reading_X+min_Reading_X)/2;
  } else if (axis=='y'){
    offset_Y=(max_Reading_Y+min_Reading_Y)/2;
  } else if (axis=='z'){
    offset_Z=(max_Reading_Z+min_Reading_Z)/2;
  }
}

void Mag_c::getRange(char axis){
  if (axis=='x'){
    range_X=(max_Reading_X-min_Reading_X)/2;
  } else if (axis=='y'){
    range_Y=(max_Reading_Y-min_Reading_Y)/2;
  } else if (axis=='z'){
    range_Z=(max_Reading_Z-min_Reading_Z)/2;
  }
}

void Mag_c::getAverage(){
  average_Range=(range_X+range_Y)/2;
}

void Mag_c::getScale(char axis){
  if (axis=='x'){
    scale_X=average_Range/range_X;
  } else if (axis=='y'){
    scale_Y=average_Range/range_Y;
  }
}

void Mag_c::calibratedReadings(float reading, char axis){
  if (axis=='x'){
    calibrated_X=scale_X*(reading-offset_X)/mag_sensitivity;
  } else if (axis=='y'){
    calibrated_Y=scale_Y*(reading-offset_Y)/mag_sensitivity;
  }  
}

//void Mag_c::scaleSensor() {
//  mag.read();
//  scaled_rot_x = mag.m.x * mag_sensitivity;
//  scaled_rot_z = mag.m.z * mag_sensitivity;
//  scaled_rot_y = mag.m.y * mag_sensitivity;
//}
//
//void Mag_c::printScaledSensor(){
//}
//
//void Mag_c::calibrate() {
//  scaleSensor();
//  
//}
//
//void Mag_c::readCalibrated(){
//  scaleSensor();
//  if(scaled_rot_x == 0){
//    if(scaled_rot_z < 0){
//      heading = PI/2;
//    }else{
//      heading = 0;
//    }
//  }else{
//    heading = atan2(scaled_rot_z, scaled_rot_x);
//  }
//  if(heading < -PI/2){ heading += 2*PI; }
//  if(heading > PI/2){ heading -= 2*PI; }
//  Serial.println(heading);
//}
//
//void Mag_c::filteredSensor() {
//  readCalibrated();
//  filtered_rot = (alpha * calibrated_rot) + ((1.0-alpha) * temp_filter);
//
//  temp_filter = calibrated_rot;
//}
//
//void Mag_c::scalars() {
////  long time_now = millis();
////  long diff_time = time_now - last_millis;
////  last_millis = time_now;
////  
////  float time_delta = (float)diff_time;
//
//  readCalibrated();
//
//  // calibrated sensor value * milliseconds * millidegrees per second * time elapsed * Pi/180
//  //both 0.001 parts are to convert from milliseconds/millidegrees to seconds/degrees.
//
////  G_z_deriv = (scaled_G_z - last_G_z)/(time_delta * 0.001);
////  rot_speed += G_z_deriv;
////  
////  if (heading>PI){
////    heading-=2*PI;
////  }
////  if (heading<-PI){
////    heading+=2*PI;
////  }
//}


Mag_c Mag(6842);
