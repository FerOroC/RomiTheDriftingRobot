#include <Wire.h>
#include <LSM6.h>

#define PI 3.1415926535897932384626433832795
#define Gravity 9.80665


float scaled_A_x;
float scaled_A_y;
float scaled_A_z;
float scaled_G_x;
float scaled_G_y;
float scaled_G_z;

LSM6 imu;


//https://github.com/paulodowd/EMATM0053_20_21/blob/main/Labsheets/Supplementary/SL3_InertialSensors.ipynb
//Click above to get sensitivity for sensor

class IMU_c {
  public:
  float IMU_A_sensitivity;
  float IMU_G_sensitivity;

  float scaled_A_x;
  float scaled_G_z;
  float calibrated_A_x;
  float calibrated_G_z;
  float last_G_z;

  float G_z_deriv;
  float rot_speed;

  float bias_rotation;
  float bias_linear;

  float filter[4];
  float temp_filter_G_z;

  float alpha = 0.8;
  float filtered_A_x;
  float filtered_G_z;
  
  long last_millis = millis();
  float heading;
  float heading_change;
  float acc;
  float vel;
  float dis;
  
  IMU_c( float A_sensor_sensitivity, float G_sensor_sensitivity ){
    IMU_A_sensitivity=A_sensor_sensitivity;
    IMU_G_sensitivity=G_sensor_sensitivity;
  };

  void calibrate();

  void readCalibrated();

  // scaleSensor scales raw sensor values by the sensor sensitivity
  void scaleSensor();

  // printScaledSensor prints the scaled raw sensor values
  void printScaledSensor();
 
  //Implements an exponential moving average filter to act as low pass filter for sensor readings
  void filteredSensor();
//
//  //Prints filtered data
//  void printFiltered();
//
  //Integrates rate of change of turning, outputs heading in radians bounded from -pi to pi. Also outputs x,y,z velocity and distance for linear acceleration.
  void IMUScalars();
};

void IMU_c::calibrate() {
  float arrLinSum = 0;
  float arrRotSum = 0;
  for (int i = 0; i < 100; i++){
    scaleSensor();
    arrLinSum += scaled_A_x;
    arrRotSum += scaled_G_z;
  }
  bias_rotation = arrRotSum/100;
  bias_linear = arrLinSum/100;

  for(int i = 0; i < 4; i++){
    readCalibrated();
    filter[i] = calibrated_A_x;
  }
}

void IMU_c::readCalibrated(){
  scaleSensor();
  calibrated_A_x = scaled_A_x - bias_linear;
  calibrated_G_z = scaled_G_z - bias_rotation;
}

void IMU_c::scaleSensor() {
  imu.read();
  scaled_A_x = imu.a.x * IMU_A_sensitivity;
  last_G_z = scaled_G_z;
  scaled_G_z = imu.g.z * IMU_G_sensitivity;
}

void IMU_c::printScaledSensor(){
  scaleSensor();
  Serial.print("Ax: ");
  Serial.println(scaled_A_x);
  Serial.print("Gz: ");
  Serial.println(scaled_G_z);
}
void IMU_c::filteredSensor() {
  readCalibrated();

  for(int i = 3; i > 0; i--){
    filter[i] = filter[i-1];
  }
  filter[0] = calibrated_A_x;
  float sum = 0;
  for(int i = 0; i < 4; i++){
    sum += filter[i];
  }
  filtered_A_x = sum/4;
  filtered_G_z = (alpha*calibrated_G_z)+((1.0-alpha)*temp_filter_G_z);

  temp_filter_G_z = calibrated_G_z;
}
//
//void IMU_c::printFiltered() {
//  filteredSensor();
//  Serial.print("A: ");
//  Serial.print(filtered_A_x);
//  Serial.print(" ");
//  Serial.print(filtered_A_y);
//  Serial.println(" ");
//  Serial.print(filtered_A_z);
//  Serial.print("\t G:  ");
//  Serial.print(filtered_G_x);
//  Serial.print(" ");
//  Serial.print(filtered_G_y);
//  Serial.print(" ");
//  Serial.println(filtered_G_z);
//}

void IMU_c::IMUScalars() {
  long time_now = millis();
  long diff_time = time_now - last_millis;
  last_millis = time_now;
  
  float time_delta = (float)diff_time;

  filteredSensor();

  // calibrated sensor value * milliseconds * millidegrees per second * time elapsed * Pi/180
  //both 0.001 parts are to convert from milliseconds/millidegrees to seconds/degrees.

  G_z_deriv = (scaled_G_z - last_G_z)/(time_delta * 0.001);
  
  heading_change = (filtered_G_z * 0.001 * time_delta * PI)/180;
  heading += heading_change;

  if (heading>PI){
    heading-=2*PI;
  }
  if (heading<-PI){
    heading+=2*PI;
  }

  acc = filtered_A_x * 0.001 * 9.81;
  vel += acc * time_delta;
  dis += vel * time_delta;
}


IMU_c IMUnit(0.061 , 8.75);
