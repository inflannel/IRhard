/* IRhard library version 1.0.0 by u/inflannel, September 2018. <-- Check the date.
 *       By the time you see this, you may be looking for a different IRhard.
 * Free to steal, copy, distribute, modify and most importantly, improve and expand.
 * This is a basic infrared reciever for NEC codes only, and does not transmit anything.
 * It uses hardware interrupt for IR detector signal, such as pin 2 or 3 on Arduino Uno.
 *
 * Usage: IRhard instanceName(pin [,rcid] [,verify])
 * int: pin = hardware interrupt pin connected to IR detector. Pin 2 or 3 on Uno.
 * unsigned int: rcid = remote control ID - if specified, ignores codes with any other rcid.
 * bool: verify - if true, ignore codes unless code is key and ~key. Default is false.
 *
 *    The IR data is available in 3 instance members: code, key, and rcid. The complete
 * 32-bit NEC code includes a 16-bit remote control ID (rcid) and a 16-bit command code
 * (code), which is itself usually made up of an 8-bit key value (key) and its inverse.
 * Once a command is detected and stored in instance.code, further decoding is disabled
 * until you reset it to 0 (instance.code = 0;) to resume decoding.
 */
#ifndef IRhard_cpp
#define IRhard_cpp
#include "IRhard.h"

volatile static unsigned int *IRhard_codePointer, *IRhard_rcidPointer;
volatile static uint8_t *IRhard_keyPointer;
static unsigned int IRhard_RCID;
static bool IRhard_No_Validate;

void _IRhard_IR_int();

IRhard::IRhard(uint8_t pin                         ) { IRhardInit(pin,  0, false); }
IRhard::IRhard(uint8_t pin, unsigned int id        ) { IRhardInit(pin, id, false); }
IRhard::IRhard(uint8_t pin,                  bool b) { IRhardInit(pin,  0, b    ); }
IRhard::IRhard(uint8_t pin, unsigned int id, bool b) { IRhardInit(pin, id, b    ); }
IRhard::IRhard(uint8_t pin, bool b, unsigned int id) { IRhardInit(pin, id, b    ); }
void IRhard::IRhardInit(uint8_t pin, unsigned int id, bool b) {
  int IR_int_num = digitalPinToInterrupt(pin);
  if (IR_int_num < 0) { // specified pin can't be used for external interrupt
    code = 7;  // code set to an initial value of 7 if the wrong pin number is passed
    key = 77;  // key set to an initial value of 77 if the wrong pin number is passed
    rcid = 7;  // RC ID set to an initial value of 7 if the wrong pin number is passed
  }
  else { // An external interrupt pin is passed, as is the normal (working) case.
    pinMode(pin, INPUT);
    IRhard_RCID = id;  // user specified remote control ID, or 0 for "any remote"
    IRhard_No_Validate = !b; // true (skips validation) if user passes false (do not validate)
    code = 0; // the NEC code as 16 bits gets stored here
    key = 0;  // the button-specific 8 bits of the NEC code go here
    rcid = 0; // basicly, the remote control's ID number
    attachInterrupt(IR_int_num, _IRhard_IR_int, FALLING); // enable the external interrupt
  }
  IRhard_keyPointer  = &key;  // global pointer to instance.key, so the ISR can update it indirectly
  IRhard_codePointer = &code; // global pointer to instance.code, so the ISR can update it indirectly
  IRhard_rcidPointer = &rcid; // global pointer to instance.base, for indirect updating by the ISR
}
void _IRhard_IR_int() {
  if (*IRhard_codePointer == 0) { // don't do anything unless instance.code == 0 (don't overwrite code)
    static unsigned long oldTime = 0, acc = 0, abit = 1;
    unsigned long nowTime = micros();
    unsigned long wl = nowTime - oldTime; // wl = wavelength, from last pulse-start to this pulse-start
    if (wl > 844) { // got short wavelenghts at times, possibly interrupted pulses, and this hack made it work
      if (wl > 2500) { acc = 0; abit = 1; } // 1-bit is 2250us, so "too long" arbitrarily set to about 10% over, causing reset
      else {
        if (wl > 1688) { acc |= abit; } // test midway between 0-bit (1125us) and 1-bit (2250us)
        if (abit < 0x80000000) { abit <<= 1; } // if not 32 bits, adjust to keep reading
        else { // accumulated 32 bits, so handle it as a complete code
          if ((IRhard_RCID == 0) || ((acc & 0xFFFF) == IRhard_RCID)) { // correct remote.
            // If validating, make sure code byte matches the inverse byte (ie 0xED12: 0xED + 0x12 == 0xFF)
            if (IRhard_No_Validate || (((acc & 0xFF000000) >> 8) + (acc & 0x00FF0000) == 0x00FF0000)) {
              *IRhard_codePointer = (acc >> 16) & 0xFFFF; // store code (upper word) in the instance member .code
              *IRhard_rcidPointer = acc & 0xFFFF; // store the remote control ID (lower word) in the instance member .rcid
              *IRhard_keyPointer = (acc >> 16) & 0xFF; // store relevant byte (lower byte of the code) in instance member .key
            }
          }
          acc = 0; abit = 1; // reset accumulator and bit pointer for next time
        }
      }
      oldTime = nowTime; // update time for calculating wavelength next time
    }
  }
}
#endif
