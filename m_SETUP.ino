void setup() {
  setupEncoder0();
  setupEncoder1();
  
  Serial.begin(9600);
  delay(1000);
  // Print reset so we can catch any reset error.
  Serial.println(" ***Reset*** ");
   
  Wire.begin();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU");
    while (1);
  }
  imu.enableDefault(); 

  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }
  mag.enableDefault();
}
