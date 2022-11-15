//I2C SLAVE CODE 
#include <Wire.h>  

enum PinAssignments {
  encoderPinA = 2,  // right
  encoderPinB = 3,  // left
};

volatile int encoderPos = 0;      // a counter for the dial
static boolean rotating = false;  // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

byte SlaveReceived = 0;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin(9600);            //Begins Serial Communication at 9600 baud rate
  Wire.begin(8);                 //Begins I2C communication with Slave Address as 8 at pin (A4,A5)
  Wire.onReceive(receiveEvent);  //Function call when Slave receives value from master
  Wire.onRequest(requestEvent);  //Function call when Master request value from Slave
}


void loop(void) {
  rotating = true;  // reset the debouncer

  /*
  Serial.println("Slave Received From Master:");  //Prints in Serial Monitor
  Serial.println(SlaveReceived);
  delay(500);
  */
}


void receiveEvent(int howMany) {  //This Function is called when Slave receives value from master
  SlaveReceived = Wire.read();    //Used to read value received from master and store in variable SlaveReceived
}


void requestEvent() {           //This Function is called when Master wants value from slave
  byte SlaveSend = encoderPos;  // Convert potvalue digital value (0 to 1023) to (0 to 127)
  Serial.println(encoderPos);
  Wire.write(SlaveSend);  // sends one byte converted POT value to master
}

// Interrupt on A changing state
void doEncoderA() {
  // debounce
  if (rotating) delay(2);  // wait a little until the bouncing is done

  // Test transition, did things really change?
  if (digitalRead(encoderPinA) != A_set) {  // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if (A_set && !B_set)
      encoderPos += 1;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if (rotating) delay(2);
  if (digitalRead(encoderPinB) != B_set) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if (B_set && !A_set)
      encoderPos -= 1;

    rotating = false;
  }
}