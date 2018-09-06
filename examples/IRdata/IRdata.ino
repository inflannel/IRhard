/* IRextInt.h demo version 1.0.0 by u/inflannel, 5 September 2018.
 * Free to steal, copy, distribute, modify and most importantly, improve and expand.
 * The header includes a basic decoder for NEC codes from an infrared reciever, and nothing more.
 * It uses external interrupt (pin 2 or 3 on Arduino Uno) for the IR detector signal, and updates
 * global variables IR_key, IR_code and IR_rcid for each code recieved.
 */

#include "IRextInt.h"

void setup() {
  Serial.begin(9600);
  Serial.println("IRextInt.h Demo");
  IR_init(2); // IR_init(external interrupt pin [,remote control ID] [,verify key inverse])
              // int: external interrupt pin - connected to IR detector. Pin 2 or 3 on Uno.
              // unsigned int: remote control ID - If set, ignores codes with any other rcid.
              // bool: verify key inverse - if true, ignore codes unless code is key and ~key.
}
void loop() {
  if (IR_code) { // IR_code is 0 (false) until an IR code is read
    if ((IR_code == 7) && (IR_key != 7)) { // result of failed initialization
      Serial.println("Specified input pin is not a hardware interrupt pin.");
    }
    else {
      Serial.print("Button #");
      Serial.print(IR_key, DEC); // IR_key is an 8-bit button identifier
      Serial.print(" with code 0x");
      Serial.print(IR_code, HEX); // IR_code is a 16-bit comand, usually IR_key and ~IR_key
      Serial.print(" Remote ID 0x");
      Serial.println(IR_rcid, HEX); // IR_rcid is a 16-bit remote control ID number
    }
    IR_code = 0; // reset IR_code to 0 to enable another IR code to be read.
  }
}
