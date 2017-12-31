#include <SoftwareSerial.h>

#define RELAY_PIN    4
#define GSMRX_PIN    2
#define GSMTX_PIN    3
#define GSMPOWER_PIN 8

// Configure software serial port
SoftwareSerial SIM900(GSMRX_PIN, GSMTX_PIN); 

void setup() {
  pinMode(RELAY_PIN, OUTPUT);          // sets the digital pin 13 as output

  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200); // for GSM shield
  // For serial monitor
  Serial.begin(19200); 
  // Give time to log on to network.
  SIM900power();
  
  SIM900.print("AT+CLIP=1\r"); // turn on caller ID notification
  delay(100);
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  pinMode(GSMPOWER_PIN, OUTPUT);
  digitalWrite(GSMPOWER_PIN, HIGH);
  delay(1500);
  digitalWrite(GSMPOWER_PIN, LOW);
  delay(5000);
}

char incoming_char=0;
void loop() {
  // Display any text that the GSM shield sends out on the serial monitor
  if(SIM900.available() >0) {
    // Get the character from the cellular serial por
    // With an incomming call, a "RING" message is sent out
    incoming_char=SIM900.read();
    //  Check if the shield is sending a "RING" message
    if (incoming_char=='R') {
      delay(10);
      Serial.print(incoming_char);
      incoming_char=SIM900.read();
      if (incoming_char =='I') {
        delay(10);
        Serial.print(incoming_char);
        incoming_char=SIM900.read();
        if (incoming_char=='N') {
          delay(10);
          Serial.print(incoming_char);
          incoming_char=SIM900.read();
          if (incoming_char=='G') {
            delay(10);
            Serial.print(incoming_char);
            // If the message received from the shield is RING
            gotRing();
          } else {
            Serial.print(incoming_char);
          }
        } else {
          Serial.print(incoming_char);
        }
      } else {
        Serial.print(incoming_char);
      }
    } else {
      Serial.print(incoming_char);
    }
  }
}

void gotRing() {
  Serial.println("");
  Serial.println("Activating relay");
  digitalWrite(RELAY_PIN, HIGH);       // sets the digital pin 13 on
  digitalWrite(LED_BUILTIN, HIGH);     // turn the LED on (HIGH is the voltage level)
  delay(3000);                         // waits for a second
  Serial.println("Dectivating relay");
  digitalWrite(RELAY_PIN, LOW);        // sets the digital pin 13 off
  digitalWrite(LED_BUILTIN, LOW);      // turn the LED off by making the voltage LOW
}

