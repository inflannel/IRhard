/* IRhard library version 1.0.0 by u/inflannel, September 2018.
 * Free to steal, copy, distribute, modify and most importantly, improve and expand.
 * This is a basic infrared reciever for NEC codes only, and does not transmit anything.
 * Uses hardware interrupt for IR detector signal, such as pin 2 or 3 on Arduino Uno.
 */
#include "IRhard.h"
IRhard IRinput(2); // instanceName(external interrupt pin [,remote control ID] [,verify key inverse])
                   // int: external interrupt pin - connected to IR detector. Pin 2 or 3 on Uno.
                   // unsigned int: remote control ID - If set, ignores codes with any other rcid.
                   // bool: verify key inverse - if true, ignore codes unless code is key and ~key.
void setup() {
    Serial.begin(9600);
    Serial.println("IRhard Demo");
}
void loop() {
  if (IRinput.code) { // .code is 0 (false) until an IR code is read
    if ((IRinput.code == 7) && (IRinput.key != 7)) { // result of failed initialization
      Serial.println("Specified input pin is not a hardware interrupt pin.");
    }
    else {
      Serial.print("Button #");
      Serial.print(IRinput.key, DEC); // .key is an 8-bit button identifier
      Serial.print(" with code 0x");
      Serial.print(IRinput.code, HEX); // .code is a 16-bit comand, generally .key and ~.key
      Serial.print(" Remote ID 0x");
      Serial.println(IRinput.rcid, HEX); // .rcid is a 16-bit remote control ID number
    }
    IRinput.code = 0; // reset code to 0 to enable another IR code to be read.
  }
}
