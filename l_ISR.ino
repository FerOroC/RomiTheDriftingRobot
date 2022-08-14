ISR( PCINT0_vect ) {

  // First, Read in the new state of the encoder pins.

  // Mask for a specific pin from the port.
  // Non-standard pin, so we access the register
  // directly.  
  // Reading just PINE would give us a number
  // composed of all 8 bits.  We want only bit 2.
  // B00000100 masks out all but bit 2
  boolean newE0_B = PINE & (1<<PINE2);
  //boolean newE0_B = PINE & B00000100;  // Does same as above.

  // Standard read fro the other pin.
  boolean newE0_A = digitalRead( E0_A_PIN ); // 26 the same as A8

  // Some clever electronics combines the
  // signals and this XOR restores the 
  // true value.
  newE0_A ^= newE0_B;

  // Create a bitwise representation of our states
  // We do this by shifting the boolean value up by
  // the appropriate number of bits, as per our table
  // header:
  //
  // State :  (bit3)  (bit2)  (bit1)  (bit0)
  // State :  New A,  New B,  Old A,  Old B.
  byte state = 0;                   
  state = state | ( newE0_A  << 3 );
  state = state | ( newE0_B  << 2 );
  state = state | ( LeftMotor.oldE_A  << 1 );
  state = state | ( LeftMotor.oldE_B  << 0 );

  // This is an inefficient way of determining
  // the direction.  However it illustrates well
  // against the lecture slides.
switch (state) {
    case 1: LeftMotor.backward();
      break;
    case 2: LeftMotor.forward();
      break;
    case 4: LeftMotor.forward();
      break;
    case 7: LeftMotor.backward();
      break;
    case 8: LeftMotor.backward();
      break;
    case 11: LeftMotor.forward();
      break;
    case 13: LeftMotor.forward();
      break;
    case 14: LeftMotor.backward();
      break;
    default: 
      break;
  }
  
  // Save current state as old state for next call.
  LeftMotor.oldE_A = newE0_A;
  LeftMotor.oldE_B = newE0_B; 
}

ISR( INT6_vect ) {
  // First, Read in the new state of the encoder pins.
  // Standard pins, so standard read functions.
  boolean newE1_B = digitalRead( E1_B_PIN );
  boolean newE1_A = digitalRead( E1_A_PIN );

  // Some clever electronics combines the
  // signals and this XOR restores the
  // true value.
  newE1_A ^= newE1_B;

  // Create a bitwise representation of our states
  // We do this by shifting the boolean value up by
  // the appropriate number of bits, as per our table
  // header:
  //
  // State :  (bit3)  (bit2)  (bit1)  (bit0)
  // State :  New A,  New B,  Old A,  Old B.
  byte state = 0;
  state = state | ( newE1_A  << 3 );
  state = state | ( newE1_B  << 2 );
  state = state | ( RightMotor.oldE_A  << 1 );
  state = state | ( RightMotor.oldE_B  << 0 );

  // This is an inefficient way of determining
  // the direction.  However it illustrates well
  // against the lecture slides.
  switch (state) {
    case 1: RightMotor.backward();
      break;
    case 2: RightMotor.forward();
      break;
    case 4: RightMotor.forward();
      break;
    case 7: RightMotor.backward();
      break;
    case 8: RightMotor.backward();
      break;
    case 11: RightMotor.forward();
      break;
    case 13: RightMotor.forward();
      break;
    case 14: RightMotor.backward();
      break;
    default: 
      break;
  }

  // Save current state as old state for next call.
  RightMotor.oldE_A = newE1_A;
  RightMotor.oldE_B = newE1_B;

}
